#include <QtQuick>

#include <sailfishapp.h>

#include "devicemanager.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> v(SailfishApp::createView());

    app->setApplicationVersion(APP_VERSION);
    app->setApplicationName("Kasa");
    app->setOrganizationDomain("org.nubecula");
    app->setOrganizationName("org.nubecula");

#ifdef QT_DEBUG
    #define uri "org.nubecula.harbour.kasa"
#else
    const auto uri = "org.nubecula.harbour.kasa";
#endif


    qmlRegisterType<Device>(uri, 1, 0, "Device");
    qmlRegisterType<DeviceListModel>(uri, 1, 0, "DeviceListModel");

    qmlRegisterSingletonType<DeviceManager>(uri,
                                            1,
                                            0,
                                            "DeviceManager",
                                            [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        auto manager = new DeviceManager;

        return manager;
    });

    v->setSource(SailfishApp::pathTo("qml/harbour-kasa.qml"));
    v->show();

    return app->exec();
}
