#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

#include "api/apiinterface.h"
#include "models/devicelistmodel.h"

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

    Q_INVOKABLE DeviceListModel *deviceListModel();
    Q_INVOKABLE void initialize();

signals:
    Q_INVOKABLE void statisticDataAvailable(const QString &hostname,
                                            const QStringList &labels,
                                            const QList<qreal> &values);

public slots:
    // add and remove device
    Q_INVOKABLE void addDevice(const QString &hostname);
    Q_INVOKABLE void removeDevice(const QString &hostname);

    // api
    Q_INVOKABLE void connectToWifiAP(const QString &hostname, const QString &ssid, const QString &password);
    void getCloudInfo(const QString &hostname);
    Q_INVOKABLE void getEnergyDayStat(const QString &hostname,
                                      int month = QDate::currentDate().month(),
                                      int year = QDate::currentDate().year());
    Q_INVOKABLE void getEnergyMonthStat(const QString &hostname,
                                        int year = QDate::currentDate().year());
    void getEnergyInfo(const QString &hostname);
    void getSystemInfo(const QString &hostname);
    void getWifiAPs(const QString &hostname);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void refresh(const QString &hostname);
    Q_INVOKABLE void restart(const QString &hostname);
    Q_INVOKABLE void reset(const QString &hostname);
    Q_INVOKABLE void setCloudServer(const QString &hostname, const QString &url);
    Q_INVOKABLE void setDeviceAlias(const QString &hostname, const QString &alias);
    Q_INVOKABLE void setDeviceMacAddress(const QString &hostname, const QString &mac);
    Q_INVOKABLE void toggleLED(const QString &hostname);
    Q_INVOKABLE void toggleOn(const QString &hostname);
    Q_INVOKABLE void regisertDeviceOnCloud(const QString &hostname,
                                           const QString &username,
                                           const QString &password);
    Q_INVOKABLE void unregisterDeviceFromCloud(const QString &hostname);

private slots:
    void onReplyAvailable(const QString &hostname,
                          const QString &topic,
                          const QString &cmd,
                          const QJsonObject &payload = QJsonObject());

private:
    // device json helper
    Device *deviceFromJson(const QJsonObject &object);
    QJsonObject deviceToJson(Device *device) const;
    void readDevices();
    void writeDevices();

    ApiInterface *m_api;
    DeviceListModel *m_deviceListModel;
    QHash<QString, Device*> m_pendingDevices;
};

#endif // DEVICEMANAGER_H
