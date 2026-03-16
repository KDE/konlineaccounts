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
import org.kde.kitemmodels

import org.kde.konlineaccounts.kcm

KCM.ScrollViewKCM {
    id: root

    required property string accountId

    property string accountName: kcm.accountName(accountId)

    title: accountName

    header: Controls.Label {
        text: i18n("These applications are authorized to use %1:", root.accountName)
    }

    view: ListView {
        id: appsList

        model: AuthorizedAppsModel {
            accountId: root.accountId
        }

        delegate: Controls.ItemDelegate {
            required property string name
            required property string iconName

            text: name
            icon.name: iconName
            width: ListView.view.width
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.largeSpacing * 2
            text: i18n("No applications are authorized to use %1", root.accountName)
            visible: appsList.count === 0
        }
    }
}
