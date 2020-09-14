import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.kasa 1.0

import "../components/"
import "qrc:/js/Chart.js" as Charts

Page {
    property var device
    property bool month: true

    id: page

    allowedOrientations: Orientation.Portrait

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Month View")
                onClicked: {
                    month = true;
                    DeviceManager.getEnergyDayStat(device.hostname)
                }
            }

            MenuItem {
                text: qsTr("Year View")
                onClicked: {
                    month = false;
                    DeviceManager.getEnergyMonthStat(device.hostname)
                }
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width

            PageHeader {
                title: qsTr("Energy Consumption")
            }

            SectionHeader {
                text: month ? qsTr("Month View") : qsTr("Year View")
            }

            ChartWrapper {
                id: chartBar

                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                height: width

                chartType: Charts.ChartType.BAR

                color: Theme.highlightColor
                scaleColor: Theme.primaryColor

                opacity: 0.8
            }
        }
    }

    Connections {
        target: DeviceManager
        onStatisticDataAvailable: {
            if (hostname !== device.hostname)
                return

            chartBar.labels = labels
            chartBar.values = values

            chartBar.chartData = chartBar.prepareBarChartData()
            chartBar.update()
        }
    }

    Component.onCompleted: {
        DeviceManager.getEnergyDayStat(device.hostname)
    }
}
