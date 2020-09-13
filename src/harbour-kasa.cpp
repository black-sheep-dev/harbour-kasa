#include <QtQuick>

#include <sailfishapp.h>

#include "devicemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Kasa"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qmlRegisterType<Device>("org.nubecula.harbour.kasa", 1, 0, "Device");
    qmlRegisterType<DeviceListModel>("org.nubecula.harbour.kasa", 1, 0, "DeviceListModel");

    qmlRegisterSingletonType<DeviceManager>("org.nubecula.harbour.kasa",
                                               1,
                                               0,
                                               "DeviceManager",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        DeviceManager *manager = new DeviceManager;

        return manager;
    });

    return SailfishApp::main(argc, argv);
}
