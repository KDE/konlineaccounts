/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCM

import org.kde.konlineaccounts.kcm

KCM.ScrollViewKCM {
    id: kaccountsRoot

    implicitHeight: Kirigami.Units.gridUnit * 28
    implicitWidth: Kirigami.Units.gridUnit * 28

    actions: [
        Kirigami.Action {
            text: i18n("Add Account…")
            icon.name: "list-add-symbolic"
            enabled: kcm.serviceRunning
            onTriggered: {
                addAccountDialog.open();
            }
        }
    ]

    headerPaddingEnabled: false // Let the InlineMessage touch the edges
    header: Kirigami.InlineMessage {
        readonly property url bugReportURL: "https://invent.kde.org/system/konlineaccounts/-/work_items"

        type: Kirigami.MessageType.Warning
        position: Kirigami.InlineMessage.Position.Header
        showCloseButton: false
        visible: true

        text: xi18nc("@info:usagetip", "This feature is under development and is still considered experimental. Report all issues <link url='%1'>here</link>.", bugReportURL)

        onLinkActivated: Qt.openUrlExternally(bugReportURL)
    }

    view: ListView {
        id: accountsList

        model: AccountsModel {
            id: accModel
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.largeSpacing * 2
            icon.name: "data-warning"
            text: i18n("Online accounts service is not running")
            visible: !kcm.serviceRunning
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.largeSpacing * 2
            text: i18n("No accounts configured")
            visible: kcm.serviceRunning && accountsList.count === 0
        }

        delegate: Controls.ItemDelegate {
            id: accountDelegate

            required property string name
            required property string id
            required property string iconName

            text: name
            icon.name: iconName

            width: ListView.view.width

            onClicked: kcm.push("Account.qml", {
                accountId: id,
                accountsModel: accModel,
            })
        }
    }

    Connections {
        target: kcm

        function onRequestedTypesChanged() {
            if (kcm.requestedTypes.length > 0) {
                addAccountDialog.open();
            }
        }

        function onRequestedAccountChanged() {
            if (kcm.requestedAccount !== "") {
                if (accModel.hasAccount(kcm.requestedAccount)) {
                    kcm.push("Account.qml", {
                        accountId: kcm.requestedAccount,
                        accountsModel: accModel,
                    })
                } else {
                    console.warn("No such account", kcm.requestedAccount)
                }
            }
        }
    }

    AddAccountDialog {
        id: addAccountDialog
        requestedTypes: kcm.requestedTypes
    }
}
