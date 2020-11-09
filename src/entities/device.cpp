#include "device.h"

Device::Device(QObject *parent) :
    QObject(parent)
{

}

bool Device::available() const
{
    return m_available;
}

quint8 Device::brightness() const
{
    return m_brightness;
}

bool Device::cloudRegistration() const
{
    return m_cloudRegistration;
}

QString Device::cloudServer() const
{
    return m_cloudServer;
}

QString Device::cloudUsername() const
{
    return m_cloudUsername;
}

qreal Device::current() const
{
    return m_current;
}

QString Device::deviceID() const
{
    return m_deviceID;
}

QString Device::deviceModel() const
{
    return m_deviceModel;
}

QString Device::deviceName() const
{
    return m_deviceName;
}

quint8 Device::deviceType() const
{
    return m_deviceType;
}

QString Device::deviceTypeName() const
{
    return m_deviceTypeName;
}

quint16 Device::features() const
{
    return m_features;
}

QString Device::firmwareVersion() const
{
    return m_firmwareVersion;
}

QString Device::hardwareVersion() const
{
    return m_hardwareVersion;
}

QString Device::hostname() const
{
    return m_hostname;
}

bool Device::ledOn() const
{
    return m_ledOn;
}

QString Device::macAddress() const
{
    return m_macAddress;
}

QString Device::name() const
{
    return m_name;
}

bool Device::on() const
{
    return m_on;
}

int Device::onTime() const
{
    return m_onTime;
}

qreal Device::power() const
{
    return m_power;
}

int Device::rssi() const
{
    return m_rssi;
}

QDateTime Device::systemTime() const
{
    return m_systemTime;
}

qreal Device::totalConsumption() const
{
    return m_totalConsumption;
}

qreal Device::voltage() const
{
    return m_voltage;
}

void Device::setAvailable(bool available)
{
    if (m_available == available)
        return;

    m_available = available;
    emit availableChanged(m_available);
    emit changed();
}

void Device::setBrightness(quint8 brightness)
{
    if (m_brightness == brightness)
        return;

    m_brightness = brightness;
    emit brightnessChanged(m_brightness);
    emit changed();
}

void Device::setCloudRegistration(bool cloudRegistration)
{
    if (m_cloudRegistration == cloudRegistration)
        return;

    m_cloudRegistration = cloudRegistration;
    emit cloudRegistrationChanged(m_cloudRegistration);
}

void Device::setCloudServer(const QString &cloudServer)
{
    if (m_cloudServer == cloudServer)
        return;

    m_cloudServer = cloudServer;
    emit cloudServerChanged(m_cloudServer);
}

void Device::setCloudUsername(const QString &cloudUsername)
{
    if (m_cloudUsername == cloudUsername)
        return;

    m_cloudUsername = cloudUsername;
    emit cloudUsernameChanged(m_cloudUsername);
}

void Device::setCurrent(qreal current)
{
    if (qFuzzyCompare(m_current, current))
        return;

    m_current = current;
    emit currentChanged(m_current);
}

void Device::setDeviceID(const QString &deviceID)
{
    if (m_deviceID == deviceID)
        return;

    m_deviceID = deviceID;
    emit deviceIDChanged(m_deviceID);
}

void Device::setDeviceModel(const QString &deviceModel)
{
    if (m_deviceModel == deviceModel)
        return;

    m_deviceModel = deviceModel;
    emit deviceModelChanged(m_deviceModel);
}

void Device::setDeviceName(const QString &deviceName)
{
    if (m_deviceName == deviceName)
        return;

    m_deviceName = deviceName;
    emit deviceNameChanged(m_deviceName);
}

void Device::setDeviceType(quint8 type)
{
    if (m_deviceType == type)
        return;

    m_deviceType = type;
    emit deviceTypeChanged(m_deviceType);
    emit changed();
}

void Device::setDeviceTypeName(const QString &name)
{
    if (m_deviceTypeName == name)
        return;

    m_deviceTypeName = name;
    emit deviceTypeNameChanged(m_deviceTypeName);
}

void Device::setFeatures(quint16 features)
{
    if (m_features == features)
        return;

    m_features = features;
    emit featuresChanged(m_features);
}

void Device::setFirmwareVersion(const QString &firmwareVersion)
{
    if (m_firmwareVersion == firmwareVersion)
        return;

    m_firmwareVersion = firmwareVersion;
    emit firmwareVersionChanged(m_firmwareVersion);
}

void Device::setHardwareVersion(const QString &hardwareVersion)
{
    if (m_hardwareVersion == hardwareVersion)
        return;

    m_hardwareVersion = hardwareVersion;
    emit hardwareVersionChanged(m_hardwareVersion);
}

void Device::setHostname(const QString &hostname)
{
    if (m_hostname == hostname)
        return;

    m_hostname = hostname;
    emit hostnameChanged(m_hostname);
    emit changed();
}

void Device::setLedOn(bool on)
{
    if (m_ledOn == on)
        return;

    m_ledOn = on;
    emit ledOnChanged(m_ledOn);
}

void Device::setMacAddress(const QString &macAddress)
{
    if (m_macAddress == macAddress)
        return;

    m_macAddress = macAddress;
    emit macAddressChanged(m_macAddress);
}

void Device::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
    emit changed();
}

void Device::setOn(bool on)
{
    if (m_on == on)
        return;

    m_on = on;
    emit onChanged(m_on);
    emit changed();
}

void Device::setOnTime(int onTime)
{
    if (m_onTime == onTime)
        return;

    m_onTime = onTime;
    emit onTimeChanged(m_onTime);
}

void Device::setPower(qreal power)
{
    if (qFuzzyCompare(m_power, power))
        return;

    m_power = power;
    emit powerChanged(m_power);
}

void Device::setRssi(int rssi)
{
    if (m_rssi == rssi)
        return;

    m_rssi = rssi;
    emit rssiChanged(m_rssi);
}

void Device::setSystemTime(const QDateTime &datetime)
{
    if (m_systemTime == datetime)
        return;

    m_systemTime = datetime;
    emit systemTimeChanged(m_systemTime);
}

void Device::setTotalConsumption(qreal totalConsumption)
{
    if (qFuzzyCompare(m_totalConsumption, totalConsumption))
        return;

    m_totalConsumption = totalConsumption;
    emit totalConsumptionChanged(m_totalConsumption);
}

void Device::setVoltage(qreal voltage)
{
    if (qFuzzyCompare(m_voltage, voltage))
        return;

    m_voltage = voltage;
    emit voltageChanged(m_voltage);
}
