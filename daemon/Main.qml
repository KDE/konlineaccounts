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

ApplicationWindow {
    id: root

    width: 500
    height: 500
    visible: true

    required property AccountsManager manager
    required property string applicationName
    required property list<string> types
    required property string callerId

    header: Kirigami.Heading {
        text: i18n("Choose account to use with %1", root.applicationName)
    }

    ListView {
        anchors.fill: parent

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

        footer: Button {
            text: "Create New"
            icon.name: "list-add"
            onClicked: root.manager.requestAccountCreation(root.types)
        }
    }
}
