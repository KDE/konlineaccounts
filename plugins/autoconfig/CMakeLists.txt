# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

add_definitions(-DTRANSLATION_DOMAIN=\"konlineaccounts_autoconfig\")

kcoreaddons_add_plugin(autoconfig INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(autoconfig PRIVATE autoconfig.cpp autoconfiginterface.cpp)

target_link_libraries(autoconfig PRIVATE accounts)

ecm_add_qml_module(autoconfigsetup URI "org.kde.konlineaccounts.autoconfig" QML_FILES Main.qml GENERATE_PLUGIN_SOURCE DEPENDENCIES QtCore org.kde.konlineaccounts)
target_sources(autoconfigsetup PRIVATE autoconfigsetup.cpp)
target_link_libraries(autoconfigsetup PRIVATE KF6::I18n accounts)
ecm_finalize_qml_module(autoconfigsetup)

ecm_qt_declare_logging_category(autoconfig
    HEADER debug.h
    IDENTIFIER LOG_KONLINEACCOUNTS_AUTOCONFIG
    CATEGORY_NAME konlineaccounts.autoconfig
    DESCRIPTION "KOnlineAccounts Autoconfig"
    EXPORT KOnlineAccounts
)

ecm_qt_declare_logging_category(autoconfigsetup
    HEADER setup_debug.h
    IDENTIFIER LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP
    CATEGORY_NAME konlineaccounts.autoconfig.setup
    DESCRIPTION "KOnlineAccounts Autoconfig Setup"
    EXPORT KOnlineAccounts
)
