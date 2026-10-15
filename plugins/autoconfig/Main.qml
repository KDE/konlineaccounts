/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.ki18n
import org.kde.konlineaccounts.autoconfig
import org.kde.konlineaccounts

SetupItem {
    id: root

    KI18nContext {
        id: _tr
        translationDomain: "konlineaccounts_autoconfig"
    }

    AutoconfigSetup {
        id: autoconfig
    }

    ColumnLayout {
        anchors.fill: parent

        Kirigami.FormLayout {
            id: fl

            Layout.fillWidth: true
            Layout.fillHeight: true

            Controls.TextField {
                id: email
                placeholderText: "some_email@example.com"
                Kirigami.FormData.label: _tr.i18n("Email:")
            }

            Controls.TextField {
                id: password
                placeholderText: "some_password"
                echoMode: TextInput.Password
                Kirigami.FormData.label: _tr.i18n("Password:")
            }

            Controls.BusyIndicator {
                id: busy

                running: false
            }
        }

        Controls.Button {
            text: _tr.i18n("Add")

            enabled: !busy.running

            Layout.alignment: Qt.AlignRight

            onClicked: {
                busy.running = true;
                autoconfig.registerAutoconfig(email.text);
            }
        }
    }
}
