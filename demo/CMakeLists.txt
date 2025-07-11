# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.25)
project(konlineaccounts-demo)

find_package(ECM 6.10.0 REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS Core DBus Widgets)
find_package(KF6 REQUIRED COMPONENTS CoreAddons Config I18n Kirigami)

add_executable(demo)

target_sources(demo PRIVATE main.cpp accountsmodel.cpp)

ecm_add_qml_module(demo
    URI "org.kde.konlineaccounts.demo"
    QML_FILES Main.qml
    DEPENDENCIES QtCore
)

target_link_libraries(demo PRIVATE Qt::Quick Qt::Widgets Qt::DBus)

install(TARGETS demo ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.konlineaccountsdemo.desktop DESTINATION ${KDE_INSTALL_APPDIR})
