# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

kcoreaddons_add_plugin(google INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(google PRIVATE google.cpp googleinterface.cpp)

target_link_libraries(google PRIVATE accounts KF6::CoreAddons Qt::DBus)

ecm_add_qml_module(googlesetup URI "org.kde.konlineaccounts.google" QML_FILES Main.qml GENERATE_PLUGIN_SOURCE)
target_sources(googlesetup PRIVATE googlesetup.cpp)
target_link_libraries(googlesetup PRIVATE QCoro::Network KF6::I18n KPim6::GAPICore accounts)
ecm_finalize_qml_module(googlesetup)
