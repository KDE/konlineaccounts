/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "smtplogintestjob.h"

#include "logintestjobbase.h"
#include "setup_debug.h"

#include <KAccountAutoconfigServer>
#include <KSMTP/LoginJob>
#include <KSMTP/Session>

namespace
{
std::optional<KSmtp::Session::EncryptionMode> socketTypeToEncryptionMode(const KAccountAutoconfigServer::SocketType &socket)
{
    switch (socket) {
    case KAccountAutoconfigServer::Plain:
        return KSmtp::Session::Unencrypted;
    case KAccountAutoconfigServer::SSL:
        return KSmtp::Session::TLS;
    case KAccountAutoconfigServer::STARTTLS:
        return KSmtp::Session::STARTTLS;
    case KAccountAutoconfigServer::UnknownSocketType:
    default:
        return {};
    }
}

std::optional<KSmtp::LoginJob::AuthMode> authenticationTypeToAuthMode(const KAccountAutoconfigServer::AuthenticationType &authentication)
{
    switch (authentication) {
    case KAccountAutoconfigServer::PasswordCleartext:
        return KSmtp::LoginJob::Plain;
    case KAccountAutoconfigServer::PasswordEncrypted:
        return KSmtp::LoginJob::CramMD5;
    case KAccountAutoconfigServer::Digest:
    case KAccountAutoconfigServer::Basic:
    case KAccountAutoconfigServer::UnknownAuthentication:
    default:
        return {};
    }
}
}

SMTPLoginTestJob::SMTPLoginTestJob(const KAccountAutoconfigServer &server, QObject *parent)
    : LoginTestJobBase(server, parent)
{
}

SMTPLoginTestJob::~SMTPLoginTestJob() = default;

void SMTPLoginTestJob::login()
{
    const auto session = new KSmtp::Session(m_server.url().host(), m_server.url().port(), this);
    if (const auto encryption = socketTypeToEncryptionMode(m_server.socketType()); encryption.has_value()) {
        session->setEncryptionMode(encryption.value());
    }

    const auto loginJob = new KSmtp::LoginJob(session);
    loginJob->setUserName(m_username);
    loginJob->setPassword(m_password);

    if (const auto authMode = authenticationTypeToAuthMode(m_server.authenticationType()); authMode.has_value()) {
        loginJob->setPreferedAuthMode(authMode.value());
    }

    connect(loginJob, &KJob::finished, this, [this, loginJob] {
        if (loginJob->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP) << "SMTP login failed:" << loginJob->errorString();
            setError(loginJob->error());
            setErrorText(loginJob->errorString());
        } else {
            qCInfo(LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP) << "SMTP login succeeded";
        }
        emitResult();
    });
    connect(session, &KSmtp::Session::connectionError, this, &SMTPLoginTestJob::handleSmtpConnectionError);
    connect(session, &KSmtp::Session::stateChanged, this, [this, session, loginJob](const KSmtp::Session::State state) {
        if (state == KSmtp::Session::Ready) {
            disconnect(session, &KSmtp::Session::connectionError, this, &SMTPLoginTestJob::handleSmtpConnectionError);
            loginJob->start();
        }
    });
    session->open();
}

void SMTPLoginTestJob::handleSmtpConnectionError(const QString &error)
{
    setErrorText(error);
    setError(UserDefinedError);
    emitResult();
}
