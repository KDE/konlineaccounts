# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

add_library(accounts)
qt_extract_metatypes(accounts)

ecm_generate_export_header(accounts
    BASE_NAME Accounts
    VERSION ${PROJECT_VERSION}
    # USE_VERSION_HEADER
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

ecm_add_qml_module(accounts URI "org.kde.konlineaccounts" DEPENDENCIES QtCore QtQuick GENERATE_PLUGIN_SOURCE)
ecm_finalize_qml_module(accounts)

target_sources(accounts PRIVATE account.cpp #[[accountsmodel.cpp accountsfiltermodel.cpp]] accountbuilder.cpp setupitem.cpp accessmanager.cpp)

target_link_libraries(accounts PUBLIC Qt::Core Qt::Gui Qt::DBus Qt::Quick KF6::ConfigCore KF6::CoreAddons KF6::WindowSystem)

install(TARGETS accounts ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
