#include "devicelistmodel.h"

#include <QDebug>

DeviceListModel::DeviceListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

DeviceListModel::~DeviceListModel()
{
    qDeleteAll(m_devices.begin(), m_devices.end());
}

QList<Device *> DeviceListModel::devices() const
{
    return m_devices;
}

Device *DeviceListModel::deviceAt(int index)
{
    if (index >= m_devices.count() || index < 0)
        return nullptr;

    return m_devices.at(index);
}

Device *DeviceListModel::deviceByHostname(const QString &hostname)
{
    for (Device *device: m_devices) {
        if (device->hostname() == hostname)
            return device;
    }

    return nullptr;
}

void DeviceListModel::addDevice(Device *device)
{
    if (!device)
        return;

    beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
    m_devices.append(device);
    endInsertRows();

    connect(device, &Device::changed, this, &DeviceListModel::updateDevice);
}

void DeviceListModel::removeDevice(Device *device)
{
    if (!device)
        return;

    const int index = m_devices.indexOf(device);

    if (index < 0)
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_devices.takeAt(index)->deleteLater();
    endRemoveRows();
}

void DeviceListModel::reset()
{
    beginResetModel();
    m_devices.clear();
    endResetModel();
}

void DeviceListModel::setDevices(const QList<Device *> &devices)
{
    beginResetModel();
    m_devices.clear();
    m_devices = devices;
    endResetModel();
}

void DeviceListModel::updateDevice()
{
    Device *device = qobject_cast<Device *>(sender());

    if (!device)
        return;

    const QModelIndex idx = index(m_devices.indexOf(device));

    if (!idx.isValid())
        return;

    emit dataChanged(idx, idx);
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_devices.count();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Device *device = m_devices.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return device->name();

    case AvailableRole:
        return device->available();

    case CurrentRole:
        return device->current();

    case DeviceIDRole:
        return device->deviceID();

    case DeviceModelRole:
        return device->deviceModel();

    case DeviceNameRole:
        return device->deviceName();

    case DeviceTypeRole:
        return device->deviceType();

    case FeaturesRole:
        return device->features();

    case FirmwareVersionRole:
        return device->firmwareVersion();

    case HardwareVersionRole:
        return device->hardwareVersion();

    case HostnameRole:
        return device->hostname();

    case LedOnRole:
        return device->ledOn();

    case MacAddressRole:
        return device->macAddress();

    case NameRole:
        return device->name();

    case OnRole:
        return device->on();

    case OnTimeRole:
        return device->onTime();

    case PowerRole:
        return device->power();

    case RssiRole:
        return device->rssi();

    case SystemTimeRole:
        return device->systemTime();

    case TotalConsumtionRole:
        return device->totalConsumption();

    case VoltageRole:
        return device->voltage();

    default:
        return QVariant();
    }
}

bool DeviceListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Device *device = m_devices.at(index.row());

    switch (role) {
    case OnRole:
        device->setOn(value.toBool());
        break;

    case LedOnRole:
        device->setLedOn(value.toBool());
        break;

    default:
        break;
    }

    return true;
}

QHash<int, QByteArray> DeviceListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[AvailableRole]        = "available";
    roles[CurrentRole]          = "current";
    roles[DeviceIDRole]         = "device_id";
    roles[DeviceModelRole]      = "device_model";
    roles[DeviceNameRole]       = "device_name";
    roles[DeviceTypeRole]       = "device_type";
    roles[FeaturesRole]         = "features";
    roles[FirmwareVersionRole]  = "fw_version";
    roles[HardwareVersionRole]  = "hw_version";
    roles[HostnameRole]         = "hostname";
    roles[LedOnRole]            = "led_on";
    roles[MacAddressRole]       = "mac_address";
    roles[NameRole]             = "name";
    roles[OnRole]               = "on";
    roles[OnTimeRole]           = "on_time";
    roles[PowerRole]            = "power";
    roles[RssiRole]             = "rssi";
    roles[SystemTimeRole]       = "system_time";
    roles[TotalConsumtionRole]  = "total_consumption";
    roles[VoltageRole]          = "voltage";

    return roles;
}
