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

            ColumnLayout {
                anchors.fill: parent

                Kirigami.FormLayout {
                    id: form_info

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
        }

        Component {
            id: servers
            Kirigami.ScrollablePage {
                anchors.fill: parent

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
                                text: protocol + " servers"
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
                                        required property int index
                                        text: `${host}:${port}`
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
                        text: "For email: " + autoconfig.email
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

                    Controls.Button {
                        text: _tr.i18n("Save")

                        enabled: !busy.running

                        Layout.alignment: Qt.AlignRight

                        onClicked: {
                            busy.running = true;
                            autoconfig.dumpServersToConfig();
                        }
                    }
                }
            }
        }
    }
}
