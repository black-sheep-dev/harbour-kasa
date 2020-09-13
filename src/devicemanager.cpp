#include "devicemanager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QStandardPaths>

DeviceManager::DeviceManager(QObject *parent) :
    QObject(parent),
    m_api(new ApiInterface(this)),
    m_deviceListModel(new DeviceListModel(this))
{
    connect(m_api, &ApiInterface::replyAvailable, this, &DeviceManager::onReplyAvailable);

    readDevices();
}

DeviceManager::~DeviceManager()
{
    qDeleteAll(m_pendingDevices.values().begin(), m_pendingDevices.values().end());
}

DeviceListModel *DeviceManager::deviceListModel()
{
    return m_deviceListModel;
}

void DeviceManager::initialize()
{

}

void DeviceManager::addDevice(const QString &hostname)
{
    if (hostname.isEmpty())
        return;

    // check if hostname is already present
    if (m_deviceListModel->deviceByHostname(hostname))
        return;

    // check if new hostname is available
    Device *device = new Device();
    device->setHostname(hostname);

    m_pendingDevices.insert(hostname, device);

    getSystemInfo(hostname);
}

void DeviceManager::removeDevice(const QString &hostname)
{
    Device *found = nullptr;

    for (Device *device: m_deviceListModel->devices()) {
        if (device->hostname() == hostname) {
            found = device;
            break;
        }
    }

    if (!found)
        return;

    m_deviceListModel->removeDevice(found);

    writeDevices();
}

void DeviceManager::connectToWifiAP(const QString &hostname, const QString &ssid, const QString &password)
{
    QJsonObject payload;
    payload.insert(QStringLiteral("ssid"), ssid);
    payload.insert(QStringLiteral("password"), password);
    payload.insert(QStringLiteral("key_type"), 3);

    m_api->sendRequest(hostname,
                       QStringLiteral("netif"),
                       QStringLiteral("set_stainfo"),
                       payload);
}

void DeviceManager::getEnergyInfo(const QString &hostname)
{
    m_api->sendRequest(hostname, "{\"emeter\":{\"get_realtime\":{}}}");
}

void DeviceManager::getSystemInfo(const QString &hostname)
{
    m_api->sendRequest(hostname, "{\"system\":{\"get_sysinfo\":{}}}");
}

void DeviceManager::getWifiAPs(const QString &hostname)
{    
    m_api->sendRequest(hostname, "{\"netif\":{\"get_scaninfo\":{\"refresh\":1}}}");
}

void DeviceManager::refresh()
{
    for (Device *device: m_deviceListModel->devices()) {
        getSystemInfo(device->hostname());
    }
}

void DeviceManager::refresh(const QString &hostname)
{
    getSystemInfo(hostname);
}

void DeviceManager::restart(const QString &hostname)
{
    m_api->sendRequest(hostname, "{\"system\":{\"reboot\":{\"delay\":3}}}");
}

void DeviceManager::reset(const QString &hostname)
{
    m_api->sendRequest(hostname, "{\"system\":{\"reset\":{\"delay\":3}}}");
}

void DeviceManager::toggleLED(const QString &hostname)
{
    Device *device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("off"), device->ledOn() ? 1 : 0);

    m_api->sendRequest(hostname,
                       QStringLiteral("system"),
                       QStringLiteral("set_led_off"),
                       payload);
}

void DeviceManager::toggleOn(const QString &hostname)
{
    Device *device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    QJsonObject payload;
    payload.insert(QStringLiteral("state"), !device->on() ? 1 : 0);

    m_api->sendRequest(hostname,
                       QStringLiteral("system"),
                       QStringLiteral("set_relay_state"),
                       payload);
}

void DeviceManager::onReplyAvailable(const QString &hostname,
                                     const QString &topic,
                                     const QString &cmd,
                                     const QJsonObject &payload)
{
//    #ifdef QT_DEBUG
//        qDebug() << hostname;
//        qDebug() << topic;
//        qDebug() << cmd;
//        qDebug() << payload;
//    #endif

    // check if device is in pending queue
    if (topic == QStringLiteral("system")) {

        if (cmd == QStringLiteral("get_sysinfo")) {

            // check if hostname is a pending new device
            bool newDevice = true;
            Device *device = m_pendingDevices.value(hostname, nullptr);

            if (!device) {
                device = m_deviceListModel->deviceByHostname(hostname);
                newDevice = false;
            }

            // set data
            device->setAvailable(true);

            device->setDeviceID(payload.value(QStringLiteral("deviceId")).toString());
            device->setDeviceModel(payload.value(QStringLiteral("model")).toString());
            device->setDeviceName(payload.value(QStringLiteral("dev_name")).toString());
            device->setDeviceType(payload.value(QStringLiteral("type")).toString());
            device->setFirmwareVersion(payload.value(QStringLiteral("sw_ver")).toString());
            device->setHardwareVersion(payload.value(QStringLiteral("hw_ver")).toString());
            device->setHostname(hostname);
            device->setMacAddress(payload.value(QStringLiteral("mac")).toString());
            device->setName(payload.value(QStringLiteral("alias")).toString());
            device->setOnTime(payload.value(QStringLiteral("on_time")).toInt());
            device->setRssi(payload.value(QStringLiteral("rssi")).toInt());

            // states
            device->setLedOn(!payload.value(QStringLiteral("led_off")).toBool());
            device->setOn(bool(payload.value(QStringLiteral("relay_state")).toInt()));

            // set device features
            const QStringList features = payload.value(QStringLiteral("feature")).toString().split(":");

            if (!features.isEmpty()) {
                for (const QString &feature: features) {
                    if (feature == QStringLiteral("TIM"))
                        device->setFeatures(device->features() | Device::FeatureTimer);
                    else if (feature == QStringLiteral("ENE"))
                        device->setFeatures(device->features() | Device::FeatureEnergy);
                }
            }

            if (newDevice) {
                m_pendingDevices.remove(hostname);
                m_deviceListModel->addDevice(device);
                writeDevices();
            }

            // update energy info
            if (device->features() & Device::FeatureEnergy)
                getEnergyInfo(hostname);

        } else if (cmd == QStringLiteral("set_relay_state")) {
            Device *device = m_deviceListModel->deviceByHostname(hostname);
            device->setOn(!device->on());
        } else if (cmd == QStringLiteral("set_led_off")) {
            Device *device = m_deviceListModel->deviceByHostname(hostname);
            device->setLedOn(!device->ledOn());
        }

    } else if (topic == QStringLiteral("emeter")) {
        Device *device = m_deviceListModel->deviceByHostname(hostname);

        if (!device)
            return;

        device->setAvailable(true);
        device->setCurrent(qreal(payload.value(QStringLiteral("current")).toDouble()));
        device->setPower(qreal(payload.value(QStringLiteral("power")).toDouble()));
        device->setTotalConsumption(qreal(payload.value(QStringLiteral("total")).toDouble()));
        device->setVoltage(qreal(payload.value(QStringLiteral("voltage")).toDouble()));
    }
}

Device *DeviceManager::deviceFromJson(const QJsonObject &object)
{
    if (object.isEmpty())
        return nullptr;

    Device *device = new Device(this);
    device->setHostname(object.value(QStringLiteral("hostname")).toString());
    device->setName(object.value(QStringLiteral("name")).toString());

    return device;
}

QJsonObject DeviceManager::deviceToJson(Device *device) const
{
    if (!device)
        return QJsonObject();

    QJsonObject obj;
    obj.insert(QStringLiteral("hostname"), device->hostname());
    obj.insert(QStringLiteral("name"), device->name());

    return obj;
}

void DeviceManager::readDevices()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-kasa/devices.json");

    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonParseError error;

    const QJsonArray devices = QJsonDocument::fromJson(file.readAll(), &error).array();

    if (error.error > 0)
        return;

    for (int i = 0; i < devices.count(); ++i) {
        Device *device = deviceFromJson(devices.at(i).toObject());

        if (!device)
            continue;

        m_deviceListModel->addDevice(device);

        getSystemInfo(device->hostname());
    }

    file.close();
}

void DeviceManager::writeDevices()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-kasa/devices.json");

    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonArray devices;

    for (Device *device : m_deviceListModel->devices()) {
        devices.append(deviceToJson(device));
    }

    file.write(QJsonDocument(devices).toJson(QJsonDocument::Indented));

    file.close();
}
