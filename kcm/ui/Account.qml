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

    header: Controls.Label {
        text: "Apps with access"
    }

    view: ListView {
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
    }
}
