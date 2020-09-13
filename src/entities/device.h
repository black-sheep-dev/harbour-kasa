#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QDateTime>

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(qreal current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(QString deviceID READ deviceID WRITE setDeviceID NOTIFY deviceIDChanged)
    Q_PROPERTY(QString deviceModel READ deviceModel WRITE setDeviceModel NOTIFY deviceModelChanged)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QString deviceType READ deviceType WRITE setDeviceType NOTIFY deviceTypeChanged)
    Q_PROPERTY(int features READ features WRITE setFeatures NOTIFY featuresChanged)
    Q_PROPERTY(QString firmwareVersion READ firmwareVersion WRITE setFirmwareVersion NOTIFY firmwareVersionChanged)
    Q_PROPERTY(QString hardwareVersion READ hardwareVersion WRITE setHardwareVersion NOTIFY hardwareVersionChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(bool ledOn READ ledOn WRITE setLedOn NOTIFY ledOnChanged)
    Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool on READ on WRITE setOn NOTIFY onChanged)
    Q_PROPERTY(int onTime READ onTime WRITE setOnTime NOTIFY onTimeChanged)
    Q_PROPERTY(qreal power READ power WRITE setPower NOTIFY powerChanged)
    Q_PROPERTY(int rssi READ rssi WRITE setRssi NOTIFY rssiChanged)
    Q_PROPERTY(QDateTime systemTime READ systemTime WRITE setSystemTime NOTIFY systemTimeChanged)
    Q_PROPERTY(qreal totalConsumption READ totalConsumption WRITE setTotalConsumption NOTIFY totalConsumptionChanged)
    Q_PROPERTY(qreal voltage READ voltage WRITE setVoltage NOTIFY voltageChanged)

public:
    enum DeviceFeature {
        FeatureNone         = 0x00,
        FeatureTimer        = 0x01,
        FeatureEnergy       = 0x02
    };
    Q_DECLARE_FLAGS(DeviceFeatures, DeviceFeature)
    Q_FLAG(DeviceFeatures)
    Q_ENUMS(DeviceFeature)

    explicit Device(QObject *parent = nullptr);

    bool available() const;
    qreal current() const;
    QString deviceID() const;
    QString deviceModel() const;
    QString deviceName() const;
    QString deviceType() const;
    int features() const;
    QString firmwareVersion() const;
    QString hardwareVersion() const;
    QString hostname() const;
    bool ledOn() const;
    QString macAddress() const;
    QString name() const;
    bool on() const;
    int onTime() const;
    qreal power() const;
    int rssi() const;
    QDateTime systemTime() const;
    qreal totalConsumption() const;
    qreal voltage() const;

signals:
    void changed();

    void availableChanged(bool available);
    void currentChanged(qreal current);
    void deviceIDChanged(const QString &deviceID);
    void deviceModelChanged(const QString &deviceModel);
    void deviceNameChanged(const QString &deviceName);
    void deviceTypeChanged(const QString &deviceType);
    void featuresChanged(int features);
    void firmwareVersionChanged(const QString &firmwareVersion);
    void hardwareVersionChanged(const QString &hardwareVersion);
    void hostnameChanged(const QString &hostname);
    void idChanged(const QString &id);
    void ledOnChanged(bool ledOn);
    void macAddressChanged(const QString &macAddress);
    void nameChanged(const QString &name);
    void onChanged(bool on);
    void onTimeChanged(int onTime);
    void powerChanged(qreal power);
    void rssiChanged(int rssi);
    void systemTimeChanged(const QDateTime &systemTime);
    void totalConsumptionChanged(qreal totalConsumption);
    void voltageChanged(qreal voltage);

public slots:
    void setAvailable(bool available);
    void setCurrent(qreal current);
    void setDeviceID(const QString &deviceID);
    void setDeviceModel(const QString &deviceModel);
    void setDeviceName(const QString &deviceName);
    void setDeviceType(const QString &deviceType);
    void setFeatures(int features);
    void setFirmwareVersion(const QString &firmwareVersion);
    void setHardwareVersion(const QString &hardwareVersion);
    void setHostname(const QString &hostname);
    void setLedOn(bool on = true);
    void setMacAddress(const QString &macAddress);
    void setName(const QString &name);
    void setOn(bool on = true);
    void setOnTime(int onTime);
    void setPower(qreal power);
    void setRssi(int rssi);
    void setSystemTime(const QDateTime &datetime);
    void setTotalConsumption(qreal totalConsumption);
    void setVoltage(qreal voltage);

private:
    bool m_available;
    qreal m_current;
    QString m_deviceID;
    QString m_deviceModel;
    QString m_deviceName;
    QString m_deviceType;
    int m_features;
    QString m_firmwareVersion;
    QString m_hardwareVersion;
    QString m_hostname;
    bool m_ledOn;
    QString m_macAddress;
    QString m_name;
    bool m_on;
    int m_onTime;
    qreal m_power;
    int m_rssi;
    QDateTime m_systemTime;
    qreal m_totalConsumption;
    qreal m_voltage;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::DeviceFeatures)

#endif // DEVICE_H
