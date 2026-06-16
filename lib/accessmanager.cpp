/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accessmanager.h"

#include "debug.h"
#include <optional>

AccessManager &AccessManager::instance()
{
    static AccessManager manager;
    return manager;
}

void AccessManager::registerApp(const QString &serviceName, const QString &appId)
{
    m_appIdentities[serviceName] = appId;
}

std::optional<QString> AccessManager::appIdForService(const QString &serviceName)
{
    if (!m_appIdentities.contains(serviceName)) {
        qCWarning(LOG_KONLINEACCOUNTS_LIB) << "DBus service" << serviceName << "not known, did you call registerApp()?";
        return std::nullopt;
    }
    return m_appIdentities[serviceName];
}

std::optional<QString> AccessManager::serviceNameForAppId(const QString &serviceName)
{
    for (auto [key, value] : std::as_const(m_appIdentities).asKeyValueRange()) {
        if (value == serviceName) {
            return key;
        }
    }

    return std::nullopt;
}
