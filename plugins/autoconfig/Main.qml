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

    Loader {
        id: loader

        anchors.fill: parent

        sourceComponent: form

        AutoconfigSetup {
            id: autoconfig

            builder: root.builder

            onStateChanged: {
                if (state === AutoconfigSetup.ServerSelection)
                    loader.sourceComponent = servers;
            }
        }

        Component {
            id: form

            Kirigami.ScrollablePage {
                anchors.fill: parent

                Kirigami.FormLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Controls.TextField {
                        id: email
                        placeholderText: _tr.i18n("some_email@example.com")
                        Kirigami.FormData.label: _tr.i18n("Email:")
                    }

                    Controls.TextField {
                        id: password
                        placeholderText: _tr.i18n("some_password")
                        echoMode: TextInput.Password
                        Kirigami.FormData.label: _tr.i18n("Password:")
                    }

                    Controls.BusyIndicator {
                        id: busy
                        visible: running
                        running: false
                    }
                }

                footer: Kirigami.ActionToolBar {
                    alignment: Qt.AlignRight
                    actions: [
                        Kirigami.Action {
                            text: _tr.i18n("Add")
                            icon.name: "list-add"
                            enabled: !busy.running
                            shortcut: "Return"
                            onTriggered: {
                                busy.running = true;
                                autoconfig.registerAutoconfig(email.text, password.text);
                            }
                        }
                    ]
                }
            }
        }

        Component {
            id: servers
            Kirigami.ScrollablePage {
                anchors.fill: parent

                footer: Kirigami.ActionToolBar {
                    alignment: Qt.AlignRight
                    actions: [
                        Kirigami.Action {
                            id: saveAction
                            text: _tr.i18nc("@action:button", "Save")
                            icon.name: "document-save"
                            tooltip: _tr.i18nc("@tooltip", "Saves the selected server configuration to a configuration file")
                            shortcut: "Return"
                            enabled: !busy.running
                            onTriggered: {
                                busy.running = true;
                                autoconfig.dumpServersToConfig();
                            }
                        }
                    ]
                }

                ColumnLayout {
                    width: parent.width

                    component ProtocolServerSelect: Kirigami.AbstractCard {
                        id: card

                        required property var serverList
                        required property string protocol

                        visible: !serverList.empty

                        property bool expanded: true

                        header: Kirigami.Heading {
                            Layout.fillWidth: true
                            Controls.CheckBox {
                                text: protocol + _tr.i18n(" servers")
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
                                        text: _tr.i18n(`${host}:${port} - auth by ${authenticationType}`)
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
                        protocol: "IMAP"
                    }
                    ProtocolServerSelect {
                        serverList: autoconfig.pop3Servers
                        protocol: "POP3"
                    }
                    ProtocolServerSelect {
                        serverList: autoconfig.smtpServers
                        protocol: "SMTP"
                    }
                    ProtocolServerSelect {
                        serverList: autoconfig.calDavServers
                        protocol: "CalDAV"
                    }
                    ProtocolServerSelect {
                        serverList: autoconfig.cardDavServers
                        protocol: "CardDAV"
                    }

                    Controls.BusyIndicator {
                        id: busy
                        running: false
                        visible: running
                    }

                }
            }
        }
    }
}
