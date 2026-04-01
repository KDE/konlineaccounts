/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mastodoninterface.h"

using namespace Qt::Literals;

MastodonInterface::MastodonInterface(Account *account, KConfigGroup config)
    : QDBusAbstractAdaptor(account)
    , m_config(config)
    , m_account(account)
{
}

#define CHECK_ACCESS                                                                                                                                           \
    if (!m_account->currentCallerHasAccess()) {                                                                                                                \
        m_account->sendErrorReply(QDBusError::AccessDenied, u"Caller is not authorized to read this property"_s);                                              \
        return {};                                                                                                                                             \
    }

QString MastodonInterface::instanceUrl() const
{
    CHECK_ACCESS

    return m_config.readEntry("instanceUrl", QString());
}

QString MastodonInterface::username() const
{
    CHECK_ACCESS

    return m_config.readEntry("username", QString());
}

QString MastodonInterface::clientId() const
{
    CHECK_ACCESS

    return m_config.readEntry("clientId", QString());
}

QString MastodonInterface::clientSecret() const
{
    CHECK_ACCESS

    return m_config.readEntry("clientSecret", QString());
}

QString MastodonInterface::accessToken() const
{
    CHECK_ACCESS

    return m_config.readEntry("accessToken", QString());
}
