/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "googleinterface.h"

using namespace Qt::Literals;

GoogleInterface::GoogleInterface(QObject *parent, KConfigGroup config)
    : QDBusAbstractAdaptor(parent)
    , m_config(config)
{
}

QString GoogleInterface::clientId() const
{
    return m_config.readEntry("clientId", QString());
}

QString GoogleInterface::clientSecret() const
{
    return m_config.readEntry("clientSecret", QString());
}

QString GoogleInterface::accessToken() const
{
    return m_config.readEntry("accessToken", QString());
}

QString GoogleInterface::refreshToken() const
{
    return m_config.readEntry("refreshToken", QString());
}

QStringList GoogleInterface::scopes() const
{
    return m_config.readEntry("scopes", QStringList());
}
