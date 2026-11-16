/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "carddav.h"

#include "debug.h"
#include "fdwriter.h"
#include "helpers.h"

#include <qt6keychain/keychain.h>

#include <QDBusConnection>
#include <QDBusMessage>

using namespace Qt::Literals;

CardDAVAdaptor::CardDAVAdaptor(Account *account, const KConfigGroup &config)
    : QDBusAbstractAdaptor(account)
    , m_config(config)
    , m_account(account)
{
}

QString CardDAVAdaptor::url() const
{
    CHECK_ACCESS

    return m_config.readEntry("url", QString());
}

QString CardDAVAdaptor::username() const
{
    CHECK_ACCESS

    return m_config.readEntry("username", QString());
}

QDBusUnixFileDescriptor CardDAVAdaptor::password() const
{
    CHECK_ACCESS

    m_account->setDelayedReply(true);

    auto *job = new QKeychain::ReadPasswordJob(u"konlineaccounts"_s);
    job->setKey(u"account/" + m_account->id() + u"/nextcloud/password");

    connect(job, &QKeychain::Job::finished, this, [job, message = m_account->message()] {
        if (job->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG) << "Failed to read password from keychain" << job->errorString();
            auto reply = message.createErrorReply(QDBusError::InternalError, job->errorString());
            QDBusConnection::sessionBus().send(reply);
            return;
        }

        const auto result = FdWriter::write(job->textData().toUtf8());

        if (!result) {
            auto reply = message.createErrorReply(QDBusError::InternalError, u"Internal error"_s);
            QDBusConnection::sessionBus().send(reply);
            return;
        }

        auto reply = message.createReply(QVariant::fromValue(result.value()));
        QDBusConnection::sessionBus().send(reply);
    });

    job->start();

    return {};
}
