/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mastodoninterface.h"

using namespace Qt::Literals;

MastodonInterface::MastodonInterface(QObject *parent, KConfigGroup config)
    : QDBusAbstractAdaptor(parent)
    , m_config(config)
{
}

QString MastodonInterface::instanceUrl() const
{
    return m_config.readEntry("instanceUrl", QString());
}

QString MastodonInterface::username() const
{
    return m_config.readEntry("username", QString());
}

QString MastodonInterface::clientId() const
{
    return m_config.readEntry("clientId", QString());
}

QString MastodonInterface::clientSecret() const
{
    return m_config.readEntry("clientSecret", QString());
}

QString MastodonInterface::accessToken() const
{
    return m_config.readEntry("accessToken", QString());
}
