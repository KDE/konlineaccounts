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
        builder: root.builder
    }

    Kirigami.ScrollablePage {
        anchors.fill: parent

        footer: RowLayout {
            Layout.fillWidth: true
            Controls.Button {
                id: addButton
                Layout.alignment: Qt.AlignRight
                visible: autoconfig.state === AutoconfigSetup.EmailForm
                enabled: autoconfig.state === AutoconfigSetup.EmailForm && !autoconfigJobRunningIndicator.running
                text: _tr.i18n("Add")
                icon.name: "list-add"
                hoverEnabled: true
                Controls.ToolTip.visible: hovered
                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                Controls.ToolTip.text: _tr.i18nc("@info:tooltip", "Search for servers that can be configured with the given email address")
                onClicked: {
                    autoconfigJobRunningIndicator.running = true;
                    autoconfig.registerAutoconfig(email.text, password.text);
                }
            }
            Controls.Button {
                Layout.alignment: Qt.AlignRight
                id: loginTestButton
                visible: autoconfig.state === AutoconfigSetup.ServerSelection
                enabled: autoconfig.state === AutoconfigSetup.ServerSelection && !loginTestJobRunningIndicator.running && autoconfig.atLeastOneProtocolEnabled
                text: _tr.i18nc("@action:button", "Login")
                hoverEnabled: true
                Controls.ToolTip.visible: hovered
                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                Controls.ToolTip.text: _tr.i18nc("@info:tooltip", enabled
                    ? "Attempt to login on the selected servers"
                    : "Select at least one server before tring to log in"
                )
                onClicked: {
                    loginTestJobRunningIndicator.running = true;
                    autoconfig.performLoginTests();
                }
            }
            Controls.Button {
                Layout.alignment: Qt.AlignRight
                id: saveButton
                visible: autoconfig.state === AutoconfigSetup.LoginResult
                enabled: autoconfig.state === AutoconfigSetup.LoginResult && !saveRunningIndicator.running && autoconfig.loginTestResult.nonEmptyAndAtLeastOneSuccess
                text: _tr.i18nc("@action:button", "Save")
                icon.name: "document-save"
                hoverEnabled: true
                Controls.ToolTip.visible: hovered
                Controls.ToolTip.delay: Kirigami.Units.toolTipDelay
                Controls.ToolTip.text: _tr.i18nc("@info:tooltip", enabled
                    ? "Saves the selected server configuration to a configuration file"
                    : "At least one server needs a successful login in order to save the configuration"
                )
                function save() {
                    saveRunningIndicator.running = true;
                    autoconfig.dumpServersToConfig();
                }
                onClicked: saveButton.save()
            }
        }

        StackLayout {
            anchors.fill: parent
            currentIndex: autoconfig.state

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
                    text: _tr.i18n("For email: ") + autoconfig.email
                }

                ProtocolServerSelect {
                    serverList: autoconfig.imapServers
                    protocol: KAccountAutoconfigServer.IMAP
                    onProtocolEnableChanged: (protocol, enabled) => {
                        autoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        autoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: autoconfig.pop3Servers
                    protocol: KAccountAutoconfigServer.POP3
                    onProtocolEnableChanged: (protocol, enabled) => {
                        autoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        autoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: autoconfig.smtpServers
                    protocol: KAccountAutoconfigServer.SMTP
                    onProtocolEnableChanged: (protocol, enabled) => {
                        autoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        autoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: autoconfig.calDavServers
                    protocol: KAccountAutoconfigServer.CalDAV
                    onProtocolEnableChanged: (protocol, enabled) => {
                        autoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        autoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }
                ProtocolServerSelect {
                    serverList: autoconfig.cardDavServers
                    protocol: KAccountAutoconfigServer.CardDAV
                    onProtocolEnableChanged: (protocol, enabled) => {
                        autoconfig.setProtocolEnabled(protocol, enabled)
                    }
                    onUrlSelectedForProtocol: (protocol, selectedUrl) => {
                        autoconfig.setUrlSelectedForProtocol(protocol, selectedUrl)
                    }
                }

                Controls.BusyIndicator {
                    id: loginTestJobRunningIndicator
                    running: false
                    visible: running
                }
            }

            ColumnLayout {
                width: parent.width

                Kirigami.Heading {
                    Layout.fillWidth: true
                    text: _tr.i18n("Summary for email: ") + autoconfig.email
                }

                Repeater {
                    model: autoconfig.loginTestResult

                    delegate: Kirigami.AbstractCard
                    {
                        required property string host
                        required property int port
                        required property string protocol
                        required property string authenticationType
                        required property string socketType
                        required property bool loginResult
                        required property string loginResultErrorString

                        header: RowLayout {
                            Kirigami.Heading {
                                Layout.fillWidth: true
                                text: Qt.enumValueToString(KAccountAutoconfigServer.Protocol, protocol)
                            }
                        }

                        contentItem: Kirigami.Padding {
                            padding: Kirigami.Units.smallSpacing
                            contentItem: ColumnLayout {
                                RowLayout {
                                    Text {
                                        Layout.fillWidth: true
                                        text: _tr.i18n(`Host: <b>${host}</b>`)
                                    }
                                    Text {
                                        text: _tr.i18n(`Port: <b>${port}</b>`)
                                    }
                                }
                                RowLayout {
                                    Text {
                                        Layout.fillWidth: true
                                        text: _tr.i18n(`Authentication: <b>${Qt.enumValueToString(KAccountAutoconfigServer.AuthenticationType, authenticationType)}</b>`)
                                    }
                                    Text {
                                        text: _tr.i18n(`Socket: <b>${Qt.enumValueToString(KAccountAutoconfigServer.SocketType, socketType)}</b>`)
                                    }
                                }
                                Kirigami.InlineMessage {
                                    Layout.fillWidth: true
                                    type: Kirigami.MessageType.Error
                                    visible: !loginResult
                                    text: _tr.i18n(loginResultErrorString)
                                }
                            }
                        }
                    }
                }

                Controls.BusyIndicator {
                    id: saveRunningIndicator
                    running: false
                    visible: running
                }
            }
        }
    }
}
