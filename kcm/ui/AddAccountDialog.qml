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
import org.kde.kitemmodels

import org.kde.konlineaccounts.kcm

Kirigami.Dialog {
    id: root

    required property list<string> requestedTypes

    title: i18n("Create Account")

    ColumnLayout {
        Controls.StackView {
            id: stack

            Layout.minimumWidth: Kirigami.Units.gridUnit * 25
            Layout.minimumHeight: Kirigami.Units.gridUnit * 18
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: availableAccountsList
        }
    }

    onClosed: {
        stack.clear();
        stack.push(availableAccountsList);
    }

    Component {
        id: availableAccountsList

        ListView {

            implicitWidth: 500

            model: KSortFilterProxyModel {
                sourceModel: AvailableAccountsModel {}

                filterRowCallback: function (source_row, source_parent) {
                    if (root.requestedTypes.length === 0) {
                        return true;
                    }

                    for (const t of root.requestedTypes) {
                        const has = sourceModel.data(sourceModel.index(source_row, 0, source_parent), 260/*ProvidesRole*/).includes(t);

                        if (has) {
                            return true;
                        }
                    }
                    return false;
                }
            }

            delegate: Kirigami.SubtitleDelegate {
                required property string name
                required property string description
                required property string iconName
                required property string setupModule
                required property string provider
                required property list<string> provides

                text: name
                icon.name: iconName
                subtitle: description
                width: ListView.view.width

                onClicked: {
                    let comp = Qt.createComponent(setupModule, "Main");
                    let builder = kcm.createBuilder(provider);
                    stack.push(comp, {
                        builder: builder
                    });

                    builder.failed.connect(function (message) {
                        console.warn("failed", message);
                        stack.push("Error.qml", {
                            message: message
                        });
                        stack.currentItem.finished.connect(function () {
                            addAccountDialog.close();
                        });
                    });

                    builder.finished.connect(function () {
                        console.warn("done");
                        stack.push("App.qml", {
                            builder: builder,
                            types: provides
                        });
                        stack.currentItem.finished.connect(function () {
                            addAccountDialog.close();
                        });
                    });
                }
            }
        }
    }
}
