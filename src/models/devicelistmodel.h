#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QAbstractListModel>

#include "src/entities/device.h"

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DeviceRoles {
        AvailableRole           = Qt::UserRole + 1,
        CurrentRole,
        DeviceIDRole,
        DeviceModelRole,
        DeviceNameRole,
        DeviceTypeRole,
        FeaturesRole,
        FirmwareVersionRole,
        HardwareVersionRole,
        HostnameRole,
        LedOnRole,
        MacAddressRole,
        NameRole,
        OnRole,
        OnTimeRole,
        PowerRole,
        RssiRole,
        SystemTimeRole,
        TotalConsumtionRole,
        VoltageRole
    };
    Q_ENUM(DeviceRoles)

    explicit DeviceListModel(QObject *parent = nullptr);
     ~DeviceListModel() override;

    Q_INVOKABLE QList<Device *> devices() const;
    Q_INVOKABLE Device *deviceAt(int index);
    Q_INVOKABLE Device *deviceByHostname(const QString &hostname);

public slots:
    void addDevice(Device *device);
    void removeDevice(Device *device);
    void reset();
    void setDevices(const QList<Device *> &devices);
    void updateDevice();

private:
    QList<Device *> m_devices;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // DEVICELISTMODEL_H
