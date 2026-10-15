# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

ecm_add_qml_module(pimautoconfigsetup
        URI "org.kde.konlineaccounts.pimautoconfig"
        QML_FILES ui/Main.qml
        GENERATE_PLUGIN_SOURCE
        DEPENDENCIES QtCore org.kde.konlineaccounts
)
target_sources(pimautoconfigsetup PRIVATE pimautoconfigsetup.cpp)
target_link_libraries(pimautoconfigsetup PRIVATE KF6::I18n accounts)
ecm_finalize_qml_module(pimautoconfigsetup)

ecm_qt_declare_logging_category(pimautoconfigsetup
        HEADER setup_debug.h
        IDENTIFIER LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP
        CATEGORY_NAME konlineaccounts.pimautoconfig.setup
        DESCRIPTION "KOnlineAccounts PIM Autoconfig Setup"
        EXPORT KOnlineAccounts
)
