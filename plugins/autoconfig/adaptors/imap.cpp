/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "imap.h"

#include "debug.h"
#include "fdwriter.h"

#include <qt6keychain/keychain.h>

#include <QDBusConnection>
#include <QDBusMessage>

using namespace Qt::Literals;

IMAPAdaptor::IMAPAdaptor(Account *account, const KConfigGroup &config)
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

QString IMAPAdaptor::hostname() const
{
    CHECK_ACCESS
    return m_config.readEntry("hostname", QString());
}

int IMAPAdaptor::port() const
{
    CHECK_ACCESS
    return m_config.readEntry("port", -1); // TODO: which port to use by default? Should we error out?
}

QString IMAPAdaptor::username() const
{
    CHECK_ACCESS
    return m_config.readEntry("username", QString());
}

KAccountAutoconfigServer::SocketType IMAPAdaptor::socketType() const
{
    CHECK_ACCESS
    const auto value = m_config.readEntry("socket_type", static_cast<int>(KAccountAutoconfigServer::SocketType::UnknownSocketType));
    return static_cast<KAccountAutoconfigServer::SocketType>(value);
}

KAccountAutoconfigServer::AuthenticationType IMAPAdaptor::authenticationType() const
{
    CHECK_ACCESS
    const auto value = m_config.readEntry("authentication_type", static_cast<int>(KAccountAutoconfigServer::AuthenticationType::UnknownAuthentication));
    return static_cast<KAccountAutoconfigServer::AuthenticationType>(value);
}

QDBusUnixFileDescriptor IMAPAdaptor::password() const
{
    CHECK_ACCESS

    m_account->setDelayedReply(true);

    auto *job = new QKeychain::ReadPasswordJob(u"konlineaccounts"_s);
    job->setKey(u"account/" + m_account->id() + u"/autoconfig/password");

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
