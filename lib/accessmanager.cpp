/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accessmanager.h"

AccessManager &AccessManager::instance()
{
    static AccessManager manager;
    return manager;
}

void AccessManager::registerApp(const QString &serviceName, const QString &appId)
{
    m_appIdentities[serviceName] = appId;
}

QString AccessManager::appIdForService(const QString &serviceName)
{
    return m_appIdentities[serviceName];
}
