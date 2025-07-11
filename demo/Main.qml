/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.konlineaccounts.demo

QQC2.ApplicationWindow {
    visible: true

    ColumnLayout {
        anchors.fill: parent

        QQC2.ComboBox {
            id: typeCombo

            Layout.fillWidth: true
            model: ["mastodon", "nextcloud", "caldav", "carddav", "google"]
        }

        ListView {

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: AccountsModel {
                id: accModel

                type: typeCombo.currentText
            }

            delegate: QQC2.ItemDelegate {
                required property string name
                required property string iconName

                text: name
                icon.name: iconName
                width: ListView.view.width
            }
        }

        QQC2.Button {
            Layout.fillWidth: true

            text: "Add new"
            onClicked: accModel.requestNew()
        }
    }
}
