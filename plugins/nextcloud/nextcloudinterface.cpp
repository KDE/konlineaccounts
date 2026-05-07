/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nextcloudinterface.h"

#include "fdwriter.h"

using namespace Qt::Literals;

NextcloudInterface::NextcloudInterface(Account *account, KConfigGroup config)
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

QString NextcloudInterface::url() const
{
    CHECK_ACCESS

    return m_config.readEntry("url", QString());
}

QString NextcloudInterface::username() const
{
    CHECK_ACCESS

    return m_config.readEntry("username", QString());
}

QDBusUnixFileDescriptor NextcloudInterface::password() const
{
    CHECK_ACCESS

    const QByteArray password = m_config.readEntry("password", QString()).toUtf8();

    const auto result = FdWriter::write(password);

    if (!result) {
        m_account->sendErrorReply(QDBusError::InternalError, u"Internal error"_s);
        return {};
    }

    return *result;
}

QString NextcloudInterface::storagePath() const
{
    CHECK_ACCESS

    return m_config.readEntry("storagePath", QString());
}
