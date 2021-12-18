# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# VERSION
VERSION = 0.2.0-1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-kasa
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

QT += network

CONFIG += sailfishapp
CONFIG += c++17

SOURCES += src/harbour-kasa.cpp \
    src/api/apiinterface.cpp \
    src/devicemanager.cpp \
    src/entities/device.cpp \
    src/models/devicelistmodel.cpp

DISTFILES += qml/harbour-kasa.qml \
    icons/scaleable/harbour-kasa.svg \
    qml/components/Chart.qml \
    qml/components/ChartWrapper.qml \
    qml/cover/CoverPage.qml \
    qml/dialogs/CloudLoginDialog.qml \
    qml/pages/DeviceExpertSettingsPage.qml \
    qml/pages/DeviceInfoPage.qml \
    qml/pages/DeviceListPage.qml \
    qml/pages/DeviceSettingsPage.qml \
    qml/pages/EnergyConsumptionChartPage.qml \
    qml/pages/SendCommandPage.qml \
    qml/pages/SettingsPage.qml \
    rpm/harbour-kasa.changes \
    rpm/harbour-kasa.changes.run.in \
    rpm/harbour-kasa.spec \
    rpm/harbour-kasa.yaml \
    translations/*.ts \
    harbour-kasa.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 512x512

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
include(translations/translations.pri)

HEADERS += \
    src/api/apiinterface.h \
    src/devicemanager.h \
    src/entities/device.h \
    src/models/devicelistmodel.h

RESOURCES += \
    ressources.qrc
