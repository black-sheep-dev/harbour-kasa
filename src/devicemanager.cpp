#include "devicemanager.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <QStandardPaths>

DeviceManager::DeviceManager(QObject *parent) :
    QObject(parent)
{
    connect(m_api, &ApiInterface::replyAvailable, this, &DeviceManager::onReplyAvailable);
    connect(m_api, &ApiInterface::connectionError, this, &DeviceManager::onConnectionError);
    connect(m_api, &ApiInterface::debugChanged, this, &DeviceManager::debugChanged);

    // create config path
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/kasa");

    readSettings();
    readDevices();
}

DeviceManager::~DeviceManager()
{
    writeSettings();
    writeDevices();
    const QList<Device *> devices = m_pendingDevices.values();
    m_pendingDevices.clear();
    qDeleteAll(devices.begin(), devices.end());
}

DeviceListModel *DeviceManager::deviceListModel()
{
    return m_deviceListModel;
}

void DeviceManager::initialize()
{

}

bool DeviceManager::debug() const
{
    return m_api->debug();
}

void DeviceManager::addDevice(const QString &hostname)
{
    if (hostname.isEmpty())
        return;

    // check if hostname is already present
    if (m_deviceListModel->deviceByHostname(hostname))
        return;

    // check if new hostname is available
    auto device = new Device();
    device->setHostname(hostname);

    m_pendingDevices.insert(hostname, device);

    getSystemInfo(hostname);
}

void DeviceManager::removeDevice(const QString &hostname)
{
    Device *found = nullptr;

    for (auto device: m_deviceListModel->devices()) {
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

void DeviceManager::getCloudInfo(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"cnCloud":{"get_info":{}}})");
}

void DeviceManager::getEnergyDayStat(const QString &hostname, int month, int year)
{
    QJsonObject payload;
    payload.insert(QStringLiteral("month"), month);
    payload.insert(QStringLiteral("year"), year);

    m_api->sendRequest(hostname,
                       QStringLiteral("emeter"),
                       QStringLiteral("get_daystat"),
                       payload);
}

void DeviceManager::getEnergyMonthStat(const QString &hostname, int year)
{
    QJsonObject payload;
    payload.insert(QStringLiteral("year"), year);

    m_api->sendRequest(hostname,
                       QStringLiteral("emeter"),
                       QStringLiteral("get_monthstat"),
                       payload);
}

void DeviceManager::getEnergyInfo(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"emeter":{"get_realtime":{}}})");
}

void DeviceManager::getSystemInfo(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"system":{"get_sysinfo":{}}})");
}

void DeviceManager::getTime(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"time":{"get_time":{}}})");
}

void DeviceManager::getTimezone(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"time":{"get_timezone":{}}})");
}

void DeviceManager::getWifiAPs(const QString &hostname)
{    
    m_api->sendRequest(hostname, R"({"netif":{"get_scaninfo":{"refresh":1}}})");
}

void DeviceManager::refresh()
{
    for (const auto device: m_deviceListModel->devices()) {
        getSystemInfo(device->hostname());
    }
}

void DeviceManager::refresh(const QString &hostname)
{
    getSystemInfo(hostname);
}

void DeviceManager::restart(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"system":{"reboot":{"delay":3}}})");
}

void DeviceManager::reset(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"system":{"reset":{"delay":3}}})");
}

void DeviceManager::resetEnergyStat(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"emeter":{"erase_emeter_stat":{}}})");
}

void DeviceManager::setBrightness(const QString &hostname, quint8 brightness)
{
    QJsonObject payload;

    payload.insert(QStringLiteral("brightness"), brightness);

    m_api->sendRequest(hostname,
                       QStringLiteral("smartlife.iot.smartbulb.lightingservice"),
                       QStringLiteral("transition_light_state"),
                       payload);
}

void DeviceManager::setCloudServer(const QString &hostname, const QString &url)
{
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    if (device->cloudServer() == url)
        return;

    device->setCloudServer(url);

    QJsonObject payload;
    payload.insert(QStringLiteral("server"), url);

    m_api->sendRequest(hostname,
                       QStringLiteral("cnCloud"),
                       QStringLiteral("set_server_url"),
                       payload);
}

void DeviceManager::setDeviceAlias(const QString &hostname, const QString &alias)
{
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    if (device->name() == alias)
        return;

    device->setName(alias);

    QJsonObject payload;
    payload.insert(QStringLiteral("alias"), alias);

    m_api->sendRequest(hostname,
                       QStringLiteral("system"),
                       QStringLiteral("set_dev_alias"),
                       payload);
}

void DeviceManager::setDeviceMacAddress(const QString &hostname, const QString &mac)
{
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    if (device->macAddress() == mac)
        return;

    device->setMacAddress(mac);

    QJsonObject payload;
    payload.insert(QStringLiteral("mac"), mac);

    m_api->sendRequest(hostname,
                       QStringLiteral("system"),
                       QStringLiteral("set_mac_addr"),
                       payload);
}

void DeviceManager::toggleLED(const QString &hostname)
{
    auto device = m_deviceListModel->deviceByHostname(hostname);

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
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    QJsonObject payload;

    if (device->deviceType() == Device::KL110) {
        payload.insert(QStringLiteral("on_off"), !device->on() ? 1 : 0);

        m_api->sendRequest(hostname,
                           QStringLiteral("smartlife.iot.smartbulb.lightingservice"),
                           QStringLiteral("transition_light_state"),
                           payload);
    } else {
        payload.insert(QStringLiteral("state"), !device->on() ? 1 : 0);

        m_api->sendRequest(hostname,
                           QStringLiteral("system"),
                           QStringLiteral("set_relay_state"),
                           payload);
    }
}

void DeviceManager::registerDeviceOnCloud(const QString &hostname,
                                          const QString &username,
                                          const QString &password)
{
    if (username.isEmpty() || password.isEmpty())
        return;

    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    device->setCloudUsername(username);

    QJsonObject payload;
    payload.insert(QStringLiteral("username"), username);
    payload.insert(QStringLiteral("password"), password);

    m_api->sendRequest(hostname,
                       QStringLiteral("cnCloud"),
                       QStringLiteral("bind"),
                       payload);
}

void DeviceManager::sendCmd(const QString &hostname, const QString &cmd)
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Send command");
#endif

    m_api->sendRequest(hostname, cmd.toLatin1());
}

void DeviceManager::unregisterDeviceFromCloud(const QString &hostname)
{
    m_api->sendRequest(hostname, R"({"cnCloud":{"unbind":null}})");
}

void DeviceManager::setDebug(bool debug)
{
    m_api->setDebug(debug);
}

void DeviceManager::onConnectionError(const QString &hostname)
{
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        return;

    device->setAvailable(false);

#ifdef QT_DEBUG
    qDebug() << "DEVICE NOT AVAILABLE";
#endif
}

void DeviceManager::onReplyAvailable(const QString &hostname,
                                     const QString &topic,
                                     const QString &cmd,
                                     const QJsonObject &payload)
{
    #ifdef QT_DEBUG
        qDebug() << hostname;
        qDebug() << topic;
        qDebug() << cmd;
        qDebug() << payload;
    #endif

    // get device ... present ones / pending ones
    auto device = m_deviceListModel->deviceByHostname(hostname);

    if (!device)
        device = m_pendingDevices.value(hostname, nullptr);

    if (!device)
        return;

    // mark as available
    device->setAvailable(true);

    // check if device is in pending queue
    if (topic == QLatin1String("system")) {

        if (cmd == QLatin1String("get_sysinfo")) {

            // check if hostname is a pending new device
            bool newDevice = m_pendingDevices.keys().contains(hostname);

            // check device type
            const QString model = payload.value(QStringLiteral("model")).toString();
            device->setDeviceModel(model);

            quint8 deviceType{Device::Unkown};

            if (model.startsWith(QLatin1String("HS100")))
                deviceType = Device::HS100;
            else if (model.startsWith(QLatin1String("HS110")))
                deviceType = Device::HS110;
            else if (model.startsWith(QLatin1String("KL110")))
                deviceType = Device::KL110;

            device->setDeviceType(deviceType);

            // device model dependend data
            if (deviceType == Device::KL110) {
                device->setDeviceName(payload.value(QStringLiteral("description")).toString());
                device->setMacAddress(payload.value(QStringLiteral("mic_mac")).toString());
                device->setDeviceTypeName(payload.value(QStringLiteral("mic_type")).toString());

                // states
                const QJsonObject lightState = payload.value(QStringLiteral("light_state")).toObject();
                device->setOn(bool(lightState.value(QStringLiteral("on_off")).toBool()));

                const QJsonObject dft = lightState.value(QStringLiteral("dft_on_state")).toObject();
                device->setBrightness(quint8(dft.value(QStringLiteral("brightness")).toInt()));

            } else {
                device->setDeviceName(payload.value(QStringLiteral("dev_name")).toString());
                device->setDeviceTypeName(payload.value(QStringLiteral("type")).toString());
                device->setMacAddress(payload.value(QStringLiteral("mac")).toString());

                device->setOn(bool(payload.value(QStringLiteral("relay_state")).toInt()));
            }

            // set data
            device->setDeviceID(payload.value(QStringLiteral("deviceId")).toString());

            device->setFirmwareVersion(payload.value(QStringLiteral("sw_ver")).toString());
            device->setHardwareVersion(payload.value(QStringLiteral("hw_ver")).toString());
            device->setHostname(hostname); 
            device->setName(payload.value(QStringLiteral("alias")).toString());
            device->setOnTime(payload.value(QStringLiteral("on_time")).toInt());
            device->setRssi(payload.value(QStringLiteral("rssi")).toInt());

            // states
            device->setLedOn(!payload.value(QStringLiteral("led_off")).toBool());

            // set device features
            const QStringList features = payload.value(QStringLiteral("feature")).toString().split(":");

            if (!features.isEmpty()) {
                for (const auto &feature: features) {
                    if (feature == QLatin1String("TIM"))
                        device->setFeatures(device->features() | Device::FeatureTimer);
                    else if (feature == QLatin1String("ENE"))
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

            // get cloud info
            getCloudInfo(hostname);

        } else if (cmd == QLatin1String("set_relay_state")) {
            device->setOn(!device->on());

        } else if (cmd == QLatin1String("set_led_off")) {
            device->setLedOn(!device->ledOn());

        }

    } else if (topic == QLatin1String("smartlife.iot.smartbulb.lightingservice")) {

        if (cmd == QLatin1String("transition_light_state")) {
            device->setOn(!device->on());

        }

    } else if (topic == QLatin1String("emeter")) {

        if (cmd == QLatin1String("get_realtime")) {
            device->setCurrent(qreal(payload.value(QStringLiteral("current")).toDouble()));
            device->setPower(qreal(payload.value(QStringLiteral("power")).toDouble()));
            device->setTotalConsumption(qreal(payload.value(QStringLiteral("total")).toDouble()));
            device->setVoltage(qreal(payload.value(QStringLiteral("voltage")).toDouble()));

        } else if (cmd == QLatin1String("get_daystat")) {
            QStringList labels;
            QList<qreal> values;

            for (const auto &item: payload.value("day_list").toArray()) {
                const QJsonObject obj = item.toObject();

                const int day = obj.value(QStringLiteral("day")).toInt();

                // fill with empty data
                int i = 0;
                while (labels.count() < day) {
                    labels.append(QString::number(i));
                    values.append(0);
                    i++;
                }

                // add other data
                labels.append(QString::number(day));
                values.append(qreal(obj.value(QStringLiteral("energy")).toDouble()));
            }

            emit statisticDataAvailable(hostname, labels, values);

        } else if (cmd == QLatin1String("get_monthstat")) {
            QStringList labels;
            QList<qreal> values;

            for (const auto &item: payload.value("month_list").toArray()) {
                const QJsonObject obj = item.toObject();

                const int month = obj.value(QStringLiteral("month")).toInt();

                // fill with empty data
                int i = 0;
                while (labels.count() < month) {
                    labels.append(QString::number(i));
                    values.append(0);
                    i++;
                }

                // add other data
                labels.append(QString::number(month));
                values.append(qreal(obj.value(QStringLiteral("energy")).toDouble()));
            }

            emit statisticDataAvailable(hostname, labels, values);
        }


    } else if (topic == QLatin1String("cnCloud")) {

        if (cmd == QLatin1String("get_info")) {
            device->setCloudRegistration(bool(payload.value(QStringLiteral("binded")).toInt()));
            device->setCloudServer(payload.value(QStringLiteral("server")).toString());
            device->setCloudUsername(payload.value(QStringLiteral("username")).toString());

        } else if (cmd == QLatin1String("bind")) {
            if (payload.value(QStringLiteral("err_code")).toInt() != 0) {
                device->setCloudUsername(QString());
                device->setCloudRegistration(false);
            }

        } else if (cmd == QLatin1String("unbind")) {
            if (payload.value(QStringLiteral("err_code")).toInt() == 0) {
                device->setCloudRegistration(false);
                device->setCloudUsername(QString());
            }
        }

    } else if (topic == QLatin1String("time")) {

        if (cmd == QLatin1String("get_time")) {
            const QDateTime datetime(QDate(payload.value(QStringLiteral("year")).toInt(),
                                           payload.value(QStringLiteral("month")).toInt(),
                                           payload.value(QStringLiteral("mday")).toInt()),
                                     QTime(payload.value(QStringLiteral("hour")).toInt(),
                                           payload.value(QStringLiteral("min")).toInt(),
                                           payload.value(QStringLiteral("sec")).toInt()));

            device->setSystemTime(datetime);
        }
    }
}

Device *DeviceManager::deviceFromJson(const QJsonObject &object)
{
    if (object.isEmpty())
        return nullptr;

    auto device = new Device(this);
    device->setHostname(object.value(QStringLiteral("hostname")).toString());
    device->setName(object.value(QStringLiteral("name")).toString());
    device->setDeviceType(quint8(object.value(QStringLiteral("type")).toInt(Device::Unkown)));

    return device;
}

QJsonObject DeviceManager::deviceToJson(Device *device) const
{
    if (!device)
        return QJsonObject();

    QJsonObject obj;
    obj.insert(QStringLiteral("hostname"), device->hostname());
    obj.insert(QStringLiteral("name"), device->name());
    obj.insert(QStringLiteral("type"), device->deviceType());

    return obj;
}

void DeviceManager::readDevices()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/org.nubecula/kasa/devices.json");

    if (!QFile(path).exists()) {
        path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/harbour-kasa/devices.json");
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonParseError error{};

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

void DeviceManager::readSettings()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/kasa/kasa.conf";

    if (!QFile(path).exists()) {
           path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-kasa/harbour-kasa.conf";
    }

    QSettings settings(path, QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("APP"));
    m_api->setDebug(settings.value(QStringLiteral("debug"), false).toBool());
    settings.endGroup();
}

void DeviceManager::writeDevices()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/org.nubecula/kasa/devices.json"));

    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonArray devices;

    for (Device *device : m_deviceListModel->devices()) {
        devices.append(deviceToJson(device));
    }

    file.write(QJsonDocument(devices).toJson(QJsonDocument::Indented));

    file.close();
}

void DeviceManager::writeSettings()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/kasa/kasa.conf", QSettings::NativeFormat);

    settings.beginGroup(QStringLiteral("APP"));
    settings.setValue(QStringLiteral("debug"), m_api->debug());
    settings.endGroup();
}
