#include <QtQuick>

#include <sailfishapp.h>

#include "kasacontrol.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-kasa.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QCoreApplication::setApplicationName(QStringLiteral("Kasa"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qmlRegisterSingletonType<KasaControl>("org.nubecula.harbour.kasa",
                                                   1,
                                                   0,
                                                   "KasaControl",
                                                   [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)

            KasaControl *control = new KasaControl;

            return control;
        });

    return SailfishApp::main(argc, argv);
}
