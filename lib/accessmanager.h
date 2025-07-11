/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QHash>
#include <QString>

#include "accounts_export.h"

class ACCOUNTS_EXPORT AccessManager
{
public:
    static AccessManager &instance();

    void registerApp(const QString &serviceName, const QString &appId);
    QString appIdForService(const QString &serviceName);

private:
    AccessManager() = default;

    QHash<QString, QString> m_appIdentities;
};
