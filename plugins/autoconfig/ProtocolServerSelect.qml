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
import org.kde.ki18n
import org.kde.konlineaccounts.autoconfig
import org.kde.konlineaccounts

Kirigami.AbstractCard {
    id: root

    KI18nContext {
        id: _tr
        translationDomain: "konlineaccounts_autoconfig"
    }

    required property var serverList
    required property int protocol

    signal protocolEnableChanged(int protocol, bool enabled)
    signal urlSelectedForProtocol(int protocol, url selectedUrl)

    visible: repeater.count > 0

    clip: true

    header: Kirigami.Heading {
        Layout.fillWidth: true
        Controls.CheckBox {
            id: checkbox
            text: _tr.i18n(Qt.enumValueToString(KAccountAutoconfigServer.Protocol, protocol) + " servers")
            checked: true
            onCheckedChanged: {
                root.protocolEnableChanged(protocol, checked)
            }
        }
    }

    contentItem: Item {
        implicitHeight: checkbox.checked ? contentColumn.height : 0

        Behavior on implicitHeight {
            id: heightAnimation
            NumberAnimation {
                duration: Kirigami.Units.shortDuration
                easing.type: Easing.InOutQuad
            }
        }

        ColumnLayout {
            id: contentColumn

            // Workaround the first radio button still showing
            // up cut off when the card is collapsed
            Item { height: 1 }

            Controls.ButtonGroup {
                id: group
            }

            Repeater {
                id: repeater
                model: root.serverList

                Controls.RadioButton {
                    required property url url
                    required property string host
                    required property int port
                    required property string authenticationType
                    required property int index
                    text: _tr.i18n(`${host}:${port} - ${authenticationType} authentication`)
                    checked: index === 0
                    Controls.ButtonGroup.group: group
                    leftPadding: indicator.width
                    onClicked: {
                        root.urlSelectedForProtocol(protocol, url)
                    }
                }
            }
        }
    }
}
