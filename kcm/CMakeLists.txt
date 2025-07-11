# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-License-Identifier: BSD-2-Clause

kcmutils_add_qml_kcm(kcm_konlineaccounts SOURCES kcm.cpp accountsmodel.cpp availableaccountsmodel.cpp suggestedappsmodel.cpp authorizedappsmodel.cpp)

target_link_libraries(kcm_konlineaccounts PRIVATE
    Qt::Core
    Qt::DBus
    KF6::CoreAddons
    KF6::I18n
    KF6::KCMUtils
    KF6::Service
    accounts
)
