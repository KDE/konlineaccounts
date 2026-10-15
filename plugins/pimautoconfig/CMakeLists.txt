# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

add_definitions(-DTRANSLATION_DOMAIN=\"konlineaccounts_pimautoconfig\")

kcoreaddons_add_plugin(pimautoconfig INSTALL_NAMESPACE "konlineaccounts/providers")

target_sources(pimautoconfig PRIVATE pimautoconfig.cpp pimautoconfiginterface.cpp)

target_link_libraries(pimautoconfig PRIVATE accounts)

ecm_qt_declare_logging_category(pimautoconfig
    HEADER debug.h
    IDENTIFIER LOG_KONLINEACCOUNTS_PIMAUTOCONFIG
    CATEGORY_NAME konlineaccounts.pimautoconfig
    DESCRIPTION "KOnlineAccounts PIM Autoconfig"
    EXPORT KOnlineAccounts
)

add_subdirectory(setup)
