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

    required property AccountBuilder builder
    required property list<string> types

    property list<string> apps

    Controls.Label {
        text: i18n("Give access to relevant apps:")
        leftPadding: Kirigami.Units.largeSpacing
    }

    ListView {
        id: suggestedAppsList

        Layout.fillWidth: true
        Layout.fillHeight: true

        model: SuggestedAppsModel {
            types: root.types
        }

        delegate: Kirigami.CheckSubtitleDelegate {
            required property string name
            required property string iconName
            required property string description
            required property string appId

            text: name
            icon.name: iconName
            subtitle: description
            width: ListView.view.width

            onToggled: {
                if (checked) {
                    root.apps.push(appId);
                } else {
                    var idx = root.apps.indexOf(appId);
                    if (idx > -1) {
                        root.apps.splice(idx, 1);
                    }
                }
            }
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - Kirigami.Units.largeSpacing * 2
            text: i18n("No relevant apps found")
            visible: suggestedAppsList.count === 0
        }
    }

    Controls.Button {
        text: i18n("Finish")

        Layout.alignment: Qt.AlignRight

        onClicked: {
            root.builder.authorizedApps = root.apps;
            root.finished();
        }
    }
}
