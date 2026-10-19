/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

pragma ComponentBehavior: Bound

import QtQml
import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.ki18n
import org.kde.konlineaccounts.pimautoconfig
import org.kde.konlineaccounts

SetupItem {
    id: root

    KI18nContext {
        id: _tr
        translationDomain: "konlineaccounts_pimautoconfig"
    }

    PIMAutoconfigSetup {
        id: pimautoconfig
        builder: root.builder
    }

    Kirigami.ScrollablePage {
        anchors.fill: parent

        footer: RowLayout {
            Layout.fillWidth: true
            Controls.Button {
                id: addButton
                Layout.alignment: Qt.AlignRight
                visible: pimautoconfig.state === PIMAutoconfigSetup.EmailForm
                enabled: pimautoconfig.state === PIMAutoconfigSetup.EmailForm && !autoconfigJobRunningIndicator.running
                text: _tr.i18n("Add")
                icon.name: "list-add"
                hoverEnabled: true
                Controls.ToolTip.visible: hovered
                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                Controls.ToolTip.text: _tr.i18nc("@info:tooltip", "Search for servers that can be configured with the given email address")
                onClicked: {
                    autoconfigJobRunningIndicator.running = true;
                    pimautoconfig.registerAutoconfig(email.text, password.text);
                }
            }
            Controls.Button {
                id: saveButton
                Layout.alignment: Qt.AlignRight
                visible: pimautoconfig.state === PIMAutoconfigSetup.ServerSelection
                enabled: pimautoconfig.state === PIMAutoconfigSetup.ServerSelection && !saveJobRunningIndicator.running
                text: _tr.i18nc("@action:button", "Save")
                icon.name: "document-save"
                hoverEnabled: true
                Controls.ToolTip.visible: hovered
                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                Controls.ToolTip.text: _tr.i18nc("@info:tooltip", "Saves the selected server configuration to a configuration file")
                onClicked: {
                    saveJobRunningIndicator.running = true;
                    pimautoconfig.dumpServersToConfig();
                }
            }
        }

        StackLayout {
            anchors.fill: parent
            currentIndex: pimautoconfig.state

            Kirigami.FormLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Controls.TextField {
                    id: email
                    placeholderText: _tr.i18n("some_email@example.com")
                    Kirigami.FormData.label: _tr.i18n("Email:")
                    focus: true
                    onAccepted: password.forceActiveFocus()
                }

                Controls.TextField {
                    id: password
                    placeholderText: _tr.i18n("some_password")
                    echoMode: TextInput.Password
                    Kirigami.FormData.label: _tr.i18n("Password:")
                    onAccepted: addButton.click()
                }

                Controls.BusyIndicator {
                    id: autoconfigJobRunningIndicator
                    visible: running
                    running: false
                }
            }

            ColumnLayout {
                width: parent.width

                Kirigami.Heading {
                    Layout.fillWidth: true
                    text: _tr.i18n("For email: ") + pimautoconfig.email
                }

                ProtocolServerSelect {
                    serverList: pimautoconfig.imapServers
                    protocol: KAccountAutoconfigServer.IMAP
                    onProtocolEnableChanged: (protocol, enabled) => {
                        pimautoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        pimautoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: pimautoconfig.pop3Servers
                    protocol: KAccountAutoconfigServer.POP3
                    onProtocolEnableChanged: (protocol, enabled) => {
                        pimautoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        pimautoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: pimautoconfig.smtpServers
                    protocol: KAccountAutoconfigServer.SMTP
                    onProtocolEnableChanged: (protocol, enabled) => {
                        pimautoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        pimautoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: pimautoconfig.calDavServers
                    protocol: KAccountAutoconfigServer.CalDAV
                    onProtocolEnableChanged: (protocol, enabled) => {
                        pimautoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        pimautoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: pimautoconfig.cardDavServers
                    protocol: KAccountAutoconfigServer.CardDAV
                    onProtocolEnableChanged: (protocol, enabled) => {
                        pimautoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        pimautoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }

                Controls.BusyIndicator {
                    id: saveJobRunningIndicator
                    running: false
                    visible: running
                }
            }
        }
    }
}
