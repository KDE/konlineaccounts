# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

set(KACCOUNTAUTOCONFIG_LIB_VERSION "1.0.0")
find_package(KF6AccountAutoconfig ${KACCOUTAUTOCONFIG_LIB_VERSION} CONFIG REQUIRED)

find_package(KF6 REQUIRED COMPONENTS Kirigami)

set(AUTOCONFIG_SETUP_SRCS
        pimautoconfigsetup.cpp
        serverlistmodel.cpp
        serverlistmodel.h
        logintestlistmodel.cpp
        logintestlistmodel.h
        kaccountautoconfigserverforeign.h
)

set(LOGIN_TESTS_SRCS
        logintests/logintestjob.cpp
        logintests/logintestjob.h
        logintests/logintestjobbase.cpp
        logintests/logintestjobbase.h
)

ecm_add_qml_module(pimautoconfigsetup
        URI "org.kde.konlineaccounts.pimautoconfig"
        QML_FILES Main.qml ProtocolServerSelect.qml
        GENERATE_PLUGIN_SOURCE
        DEPENDENCIES QtCore org.kde.konlineaccounts
)
target_sources(pimautoconfigsetup PRIVATE ${AUTOCONFIG_SETUP_SRCS} ${LOGIN_TESTS_SRCS})
target_link_libraries(pimautoconfigsetup
        PRIVATE
        KF6::I18n
        KF6::AccountAutoconfig
        Qt6Keychain::Qt6Keychain
        KF6::Kirigami
        accounts
)
ecm_finalize_qml_module(pimautoconfigsetup)

ecm_qt_declare_logging_category(pimautoconfigsetup
        HEADER setup_debug.h
        IDENTIFIER LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP
        CATEGORY_NAME konlineaccounts.pimautoconfig.setup
        DESCRIPTION "KOnlineAccounts PIM Autoconfig Setup"
        EXPORT KOnlineAccounts
)
