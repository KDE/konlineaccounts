/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.konlineaccounts.kcm
import org.kde.konlineaccounts

ColumnLayout {
    id: root

    signal finished

    required property string message

    Controls.Label {
        text: i18n("Setup failed: %1", message)
    }

    Controls.Button {
        text: i18n("Ok")

        Layout.alignment: Qt.AlignRight

        onClicked: {
            root.finished();
        }
    }
}
