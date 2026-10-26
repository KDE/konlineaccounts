# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

find_package(KF6 REQUIRED COMPONENTS Kirigami)

add_definitions(-DTRANSLATION_DOMAIN=\"konlineaccounts_autoconfig\")

kcoreaddons_add_plugin(autoconfig INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(autoconfig PRIVATE autoconfig.cpp autoconfiginterface.cpp)

target_link_libraries(autoconfig PRIVATE accounts KPim6::AccountAutoconfig)

set(LOGIN_TESTS_SRCS
        logintestjob.cpp
        logintestjob.h
        logintestbase.cpp
        logintestbase.h
)

ecm_add_qml_module(autoconfigsetup URI "org.kde.konlineaccounts.autoconfig" QML_FILES Main.qml GENERATE_PLUGIN_SOURCE DEPENDENCIES QtCore org.kde.konlineaccounts)
target_sources(autoconfigsetup PRIVATE autoconfigsetup.cpp ${LOGIN_TESTS_SRCS})
#target_link_libraries(autoconfigsetup PRIVATE Qt::NetworkAuth QCoro::Core QCoro::Network KF6::I18n KF6::KIOCore Qt6Keychain::Qt6Keychain accounts)
target_link_libraries(autoconfigsetup
        PRIVATE
        KF6::I18n
        KPim6::AccountAutoconfig
        Qt6Keychain::Qt6Keychain
        Kirigami
        accounts
)
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
