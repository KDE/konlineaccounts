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
            onTriggered: {
                addAccountDialog.open();
            }
        }
    ]

    view: ListView {
        id: accountsList

        model: AccountsModel {
            id: accModel
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.largeSpacing * 2
            text: "No accounts yet"
            visible: accountsList.count === 0
        }

        delegate: Controls.ItemDelegate {
            id: accountDelegate
            required property string name
            required property string id
            required property string iconName

            contentItem: RowLayout {
                Kirigami.IconTitleSubtitle {
                    title: accountDelegate.name
                    icon.name: accountDelegate.iconName

                    Layout.fillWidth: true
                }

                Controls.ToolButton {
                    text: "Remove"
                    display: Controls.ToolButton.IconOnly
                    icon.name: "list-remove"
                    onClicked: accModel.removeAccount(accountDelegate.id)
                }
            }

            width: ListView.view.width

            onClicked: kcm.push("Account.qml", {
                accountId: id
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
    }

    AddAccountDialog {
        id: addAccountDialog
        requestedTypes: kcm.requestedTypes
    }
}
