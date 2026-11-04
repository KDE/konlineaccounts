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

        footer: Kirigami.ActionToolBar {
            alignment: Qt.AlignRight
            actions: [
                Kirigami.Action {
                    visible: autoconfig.state === AutoconfigSetup.EmailForm
                    enabled: autoconfig.state === AutoconfigSetup.EmailForm && !autoconfigJobRunningIndicator.running
                    text: _tr.i18n("Add")
                    icon.name: "list-add"
                    shortcut: "Return"
                    onTriggered: {
                        autoconfigJobRunningIndicator.running = true;
                        autoconfig.registerAutoconfig(email.text, password.text);
                    }
                },
                Kirigami.Action {
                    id: saveAction
                    visible: autoconfig.state === AutoconfigSetup.ServerSelection
                    enabled: autoconfig.state === AutoconfigSetup.ServerSelection && !saveJobRunningIndicator.running
                    text: _tr.i18nc("@action:button", "Save")
                    icon.name: "document-save"
                    tooltip: _tr.i18nc("@tooltip", "Saves the selected server configuration to a configuration file")
                    shortcut: "Return"
                    onTriggered: {
                        saveJobRunningIndicator.running = true;
                        autoconfig.dumpServersToConfig();
                    }
                }
            ]
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
                    Component.onCompleted: {
                        Qt.callLater(() => forceActiveFocus())
                    }
                }

                Controls.TextField {
                    id: password
                    placeholderText: _tr.i18n("some_password")
                    echoMode: TextInput.Password
                    Kirigami.FormData.label: _tr.i18n("Password:")
                }

                Controls.BusyIndicator {
                    id: autoconfigJobRunningIndicator
                    visible: running
                    running: false
                }
            }

            ColumnLayout {
                width: parent.width

                component ProtocolServerSelect: Kirigami.AbstractCard {
                    id: card

                    required property var serverList
                    required property int protocol

                    visible: !serverList.empty

                    property bool expanded: true

                    header: Kirigami.Heading {
                        Layout.fillWidth: true
                        Controls.CheckBox {
                            text: _tr.i18n(Qt.enumValueToString(KAccountAutoconfigServer.Protocol, protocol) + " servers")
                            checked: true
                            onCheckedChanged: {
                                card.expanded = checked;
                                autoconfig.setProtocolEnabled(protocol, checked)
                            }
                        }
                    }

                    contentItem: Item {
                        implicitHeight: card.expanded ? contentColumn.implicitHeight : 0

                        ColumnLayout {
                            id: contentColumn
                            visible: card.expanded

                            Controls.ButtonGroup {
                                id: group
                            }

                            ListView {
                                model: card.serverList
                                implicitHeight: contentHeight
                                delegate: Controls.RadioButton {
                                    required property url url
                                    required property string host
                                    required property int port
                                    required property string authenticationType
                                    required property int index
                                    text: _tr.i18n(`${host}:${port} - auth by ${Qt.enumValueToString(KAccountAutoconfigServer.AuthenticationType, authenticationType)}`)
                                    checked: index === 0
                                    Controls.ButtonGroup.group: group
                                    leftPadding: indicator.width
                                    onClicked: {
                                        autoconfig.setUrlSelectedForProtocol(protocol, url)
                                    }
                                }
                            }
                        }
                    }
                }

                Kirigami.Heading {
                    Layout.fillWidth: true
                    text: _tr.i18n("For email: ") + autoconfig.email
                }

                ProtocolServerSelect {
                    serverList: autoconfig.imapServers
                    protocol: KAccountAutoconfigServer.IMAP
                }
                ProtocolServerSelect {
                    serverList: autoconfig.pop3Servers
                    protocol: KAccountAutoconfigServer.POP3
                }
                ProtocolServerSelect {
                    serverList: autoconfig.smtpServers
                    protocol: KAccountAutoconfigServer.SMTP
                }
                ProtocolServerSelect {
                    serverList: autoconfig.calDavServers
                    protocol: KAccountAutoconfigServer.CalDAV
                }
                ProtocolServerSelect {
                    serverList: autoconfig.cardDavServers
                    protocol: KAccountAutoconfigServer.CardDAV
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
