#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

#include "api/apiinterface.h"
#include "models/devicelistmodel.h"

class DeviceManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)

public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager() override;

    Q_INVOKABLE DeviceListModel *deviceListModel();
    Q_INVOKABLE void initialize();

    // properties
    bool debug() const;

signals:
    void statisticDataAvailable(const QString &hostname,
                                const QStringList &labels,
                                const QList<qreal> &values);

    // properties
    void debugChanged(bool debug);

public slots:
    // add and remove device
    void addDevice(const QString &hostname);
    void removeDevice(const QString &hostname);

    // api
    void connectToWifiAP(const QString &hostname, const QString &ssid, const QString &password);
    void getCloudInfo(const QString &hostname);
    void getEnergyDayStat(const QString &hostname,
                          int month = QDate::currentDate().month(),
                          int year = QDate::currentDate().year());
    void getEnergyMonthStat(const QString &hostname,
                            int year = QDate::currentDate().year());
    void getEnergyInfo(const QString &hostname);
    void getSystemInfo(const QString &hostname);
    void getTime(const QString &hostname);
    void getTimezone(const QString &hostname);
    void getWifiAPs(const QString &hostname);
    void refresh();
    void refresh(const QString &hostname);
    void restart(const QString &hostname);
    void reset(const QString &hostname);
    void resetEnergyStat(const QString &hostname);
    void setBrightness(const QString &hostname, quint8 brightness);
    void setCloudServer(const QString &hostname, const QString &url);
    void setDeviceAlias(const QString &hostname, const QString &alias);
    void setDeviceMacAddress(const QString &hostname, const QString &mac);
    void toggleLED(const QString &hostname);
    void toggleOn(const QString &hostname);
    void registerDeviceOnCloud(const QString &hostname,
                               const QString &username,
                               const QString &password);
    void sendCmd(const QString &hostname, const QString &cmd);
    void unregisterDeviceFromCloud(const QString &hostname);

    // properties
    void setDebug(bool debug);

private slots:
    void onConnectionError(const QString &hostname);
    void onReplyAvailable(const QString &hostname,
                          const QString &topic,
                          const QString &cmd,
                          const QJsonObject &payload = QJsonObject());

private:
    // device json helper
    Device *deviceFromJson(const QJsonObject &object);
    QJsonObject deviceToJson(Device *device) const;
    void readDevices();
    void readSettings();
    void writeDevices();
    void writeSettings();


    ApiInterface *m_api{new ApiInterface(this)};
    DeviceListModel *m_deviceListModel{new DeviceListModel(this)};
    QHash<QString, Device*> m_pendingDevices;
};

#endif // DEVICEMANAGER_H
