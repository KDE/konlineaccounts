# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

add_definitions(-DTRANSLATION_DOMAIN=\"konlineaccounts_autoconfig\")

kcoreaddons_add_plugin(autoconfig INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(autoconfig PRIVATE autoconfig.cpp autoconfiginterface.cpp)

target_link_libraries(autoconfig PRIVATE accounts)

ecm_qt_declare_logging_category(autoconfig
    HEADER debug.h
    IDENTIFIER LOG_KONLINEACCOUNTS_AUTOCONFIG
    CATEGORY_NAME konlineaccounts.autoconfig
    DESCRIPTION "KOnlineAccounts Autoconfig"
    EXPORT KOnlineAccounts
)

add_subdirectory(setup)
