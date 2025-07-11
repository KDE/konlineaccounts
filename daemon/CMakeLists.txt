# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(konlineaccountsd)

ecm_add_qml_module(konlineaccountsd URI "org.kde.konlineaccounts.daemon" QML_FILES Main.qml DEPENDENCIES QtCore)

target_sources(konlineaccountsd PRIVATE main.cpp accountsmanager.cpp accountsmanagerprivate.cpp accountsmodel.cpp accountsfiltermodel.cpp propertiesadaptor.cpp)

target_link_libraries(konlineaccountsd PRIVATE Qt::Core Qt::DBus Qt::Widgets KF6::CoreAddons KF6::ConfigCore KF6::KIOGui KF6::WindowSystem KF6::I18n KF6::I18nQml KF6::DBusAddons KF6::Crash accounts)

install(TARGETS konlineaccountsd DESTINATION ${KDE_INSTALL_BINDIR})
