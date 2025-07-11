# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

kcoreaddons_add_plugin(mastodon INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(mastodon PRIVATE mastodon.cpp mastodoninterface.cpp)

target_link_libraries(mastodon PRIVATE accounts KF6::CoreAddons Qt::DBus)

ecm_add_qml_module(mastodonsetup URI "org.kde.konlineaccounts.mastodon" QML_FILES Main.qml GENERATE_PLUGIN_SOURCE DEPENDENCIES QtCore org.kde.konlineaccounts)
target_sources(mastodonsetup PRIVATE mastodonsetup.cpp)
target_link_libraries(mastodonsetup PRIVATE Qt::NetworkAuth QCoro::Core QCoro::Network KF6::I18n KF6::KIOCore accounts)
ecm_finalize_qml_module(mastodonsetup)
