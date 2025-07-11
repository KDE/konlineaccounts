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
import org.kde.konlineaccounts.google
import org.kde.konlineaccounts

SetupItem {
    id: root

    signal finished

    GoogleSetup {
        builder: root.builder
    }

    Controls.Label {
        text: "Continue in browser"
    }
}
