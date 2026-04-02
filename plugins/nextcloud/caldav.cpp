/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "caldav.h"

#include "fdwriter.h"

#include <qt6keychain/keychain.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusVariant>

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

QDBusUnixFileDescriptor CalDAV::password() const
{
    CHECK_ACCESS

    m_account->setDelayedReply(true);

    QKeychain::ReadPasswordJob *job = new QKeychain::ReadPasswordJob(u"konlineaccounts"_s);
    job->setKey(u"account/" + m_account->id() + u"/nextcloud/caldav/password");

    connect(job, &QKeychain::Job::finished, this, [this, job, message = m_account->message()] {
        qWarning() << "reply" << job->textData();

        const auto result = FdWriter::write(job->textData().toUtf8());

        if (!result) {
            m_account->sendErrorReply(QDBusError::InternalError, u"Internal error"_s);
        } else {
            auto reply = message.createReply(QVariant::fromValue(QDBusVariant(job->textData())));
            QDBusConnection::sessionBus().send(reply);
        }
    });

    job->start();

    return {};
}
