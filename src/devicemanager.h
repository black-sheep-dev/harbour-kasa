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

public slots:
    // add and remove device
    Q_INVOKABLE void addDevice(const QString &hostname);
    Q_INVOKABLE void removeDevice(const QString &hostname);

    // api
    Q_INVOKABLE void connectToWifiAP(const QString &hostname, const QString &ssid, const QString &password);
    void getEnergyInfo(const QString &hostname);
    void getSystemInfo(const QString &hostname);
    void getWifiAPs(const QString &hostname);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void refresh(const QString &hostname);
    Q_INVOKABLE void restart(const QString &hostname);
    Q_INVOKABLE void reset(const QString &hostname);
    Q_INVOKABLE void toggleLED(const QString &hostname);
    Q_INVOKABLE void toggleOn(const QString &hostname);

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
