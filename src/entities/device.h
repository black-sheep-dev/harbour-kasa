#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QDateTime>

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool available READ available WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(quint8 brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(bool cloudRegistration READ cloudRegistration WRITE setCloudRegistration NOTIFY cloudRegistrationChanged)
    Q_PROPERTY(QString cloudServer READ cloudServer WRITE setCloudServer NOTIFY cloudServerChanged)
    Q_PROPERTY(QString cloudUsername READ cloudUsername WRITE setCloudUsername NOTIFY cloudUsernameChanged)
    Q_PROPERTY(qreal current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(QString deviceID READ deviceID WRITE setDeviceID NOTIFY deviceIDChanged)
    Q_PROPERTY(QString deviceModel READ deviceModel WRITE setDeviceModel NOTIFY deviceModelChanged)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(quint8 deviceType READ deviceType WRITE setDeviceType NOTIFY deviceTypeChanged)
    Q_PROPERTY(QString deviceTypeName READ deviceTypeName WRITE setDeviceTypeName NOTIFY deviceTypeNameChanged)
    Q_PROPERTY(quint16 features READ features WRITE setFeatures NOTIFY featuresChanged)
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
    Q_ENUM(DeviceFeature)

    enum DeviceType {
        Unkown,
        HS100,
        HS110,
        KL110
    };
    Q_ENUM(DeviceType)

    explicit Device(QObject *parent = nullptr);

    bool available() const;
    quint8 brightness() const;
    bool cloudRegistration() const;
    QString cloudServer() const;
    QString cloudUsername() const;
    qreal current() const;
    QString deviceID() const;
    QString deviceModel() const;
    QString deviceName() const;
    quint8 deviceType() const;
    QString deviceTypeName() const;
    quint16 features() const;
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
    void brightnessChanged(quint8 brightness);
    void cloudRegistrationChanged(bool cloudRegistration);
    void cloudServerChanged(const QString &cloudServer);
    void cloudUsernameChanged(const QString &cloudUsername);
    void currentChanged(qreal current);
    void deviceIDChanged(const QString &deviceID);
    void deviceModelChanged(const QString &deviceModel);
    void deviceNameChanged(const QString &deviceName);
    void deviceTypeChanged(quint8 type);
    void deviceTypeNameChanged(const QString &name);
    void featuresChanged(quint16 features);
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
    void setBrightness(quint8 brightness);
    void setCloudRegistration(bool cloudRegistration);
    void setCloudServer(const QString &cloudServer);
    void setCloudUsername(const QString &cloudUsername);
    void setCurrent(qreal current);
    void setDeviceID(const QString &deviceID);
    void setDeviceModel(const QString &deviceModel);
    void setDeviceName(const QString &deviceName);
    void setDeviceType(quint8 type);
    void setDeviceTypeName(const QString &name);
    void setFeatures(quint16 features);
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
    bool m_available{false};
    quint8 m_brightness{0};
    bool m_cloudRegistration{false};
    QString m_cloudServer{QStringLiteral("devs.tplinkcloud.com")};
    QString m_cloudUsername;
    qreal m_current{0};
    QString m_deviceID;
    QString m_deviceModel;
    QString m_deviceName;
    quint8 m_deviceType{DeviceType::Unkown};
    QString m_deviceTypeName;
    quint16 m_features{FeatureNone};
    QString m_firmwareVersion;
    QString m_hardwareVersion;
    QString m_hostname;
    bool m_ledOn{false};
    QString m_macAddress;
    QString m_name;
    bool m_on{false};
    int m_onTime{0};
    qreal m_power{0.0};
    int m_rssi{0};
    QDateTime m_systemTime{QDateTime::currentDateTime()};
    qreal m_totalConsumption{0.0};
    qreal m_voltage{0.0};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Device::DeviceFeatures)

#endif // DEVICE_H
