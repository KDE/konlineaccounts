/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "caldav.h"

using namespace Qt::Literals;

CalDAV::CalDAV(Account *account, KConfigGroup config)
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

QString CalDAV::url() const
{
    CHECK_ACCESS

    return m_config.readEntry("url", QString());
}

QString CalDAV::username() const
{
    CHECK_ACCESS

    return m_config.readEntry("username", QString());
}

QString CalDAV::password() const
{
    CHECK_ACCESS

    return m_config.readEntry("password", QString());
}
