/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls

import org.kde.kirigami as Kirigami

import org.kde.konlineaccounts
import org.kde.konlineaccounts.daemon

Kirigami.ApplicationWindow {
    id: root

    title: i18nc("@title:window", "Online Accounts")

    width: 500
    height: 500
    visible: true

    required property AccountsManager manager
    required property string applicationName
    required property list<string> types
    required property string callerId

    pageStack.initialPage: thePage

    Component {
        id: thePage

        Kirigami.ScrollablePage {

            title: i18n("Choose account to use with %1", root.applicationName)

            ListView {
                id: accountsList

                Kirigami.PlaceholderMessage {
                    anchors.centerIn: parent
                    width: parent.width - Kirigami.Units.largeSpacing * 2
                    text: i18n("No accounts suitable for %1 yet", root.applicationName)
                    visible: accountsList.count === 0
                }

                model: AccountsFilterModel {
                    sourceModel: AccountsModel {
                        manager: root.manager
                    }
                    types: root.types
                    notAlreadyAllowedFor: root.callerId
                }

                delegate: ItemDelegate {

                    required property string name
                    required property string id
                    required property string iconName

                    text: name
                    icon.name: iconName
                    width: ListView.view.width

                    onClicked: root.manager.accountSelected(root.callerId, id)
                }
            }

            footer: Button {
                text: i18nc("@action:button", "Create New")
                icon.name: "list-add"
                onClicked: root.manager.requestAccountCreation(root.types)
            }
        }
    }
}
