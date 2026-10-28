/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "smtplogintestjob.h"
#include "setup_debug.h"

SMTPLoginTestJob::SMTPLoginTestJob(QObject *parent)
    : LoginTestBase(parent)
{
}

SMTPLoginTestJob::~SMTPLoginTestJob() = default;

void SMTPLoginTestJob::setHostname(const QString &hostname)
{
    m_hostname = hostname;
}

void SMTPLoginTestJob::setPort(const int port)
{
    m_port = port;
}

void SMTPLoginTestJob::setSocketType(const KAutoconfigServer::SocketType &socket)
{
    switch (socket) {
    case KAutoconfigServer::Plain:
        m_encryption = KSmtp::Session::Unencrypted;
        break;
    case KAutoconfigServer::SSL:
        m_encryption = KSmtp::Session::TLS;
        break;
    case KAutoconfigServer::STARTTLS:
        m_encryption = KSmtp::Session::STARTTLS;
        break;
    case KAutoconfigServer::UnknownSocketType:
    default:
        break;
    }
}

void SMTPLoginTestJob::setAuthenticationType(const KAutoconfigServer::AuthenticationType &authentication)
{
    switch (authentication) {
    case KAutoconfigServer::PasswordCleartext:
        m_authentication = KSmtp::LoginJob::Plain;
        break;
    case KAutoconfigServer::PasswordEncrypted:
        m_authentication = KSmtp::LoginJob::CramMD5;
        break;
    case KAutoconfigServer::Digest:
    case KAutoconfigServer::Basic:
    case KAutoconfigServer::UnknownAuthentication:
    default:
        break;
    }
}

void SMTPLoginTestJob::login()
{
    const auto session = new KSmtp::Session(m_hostname, m_port, this);
    if (m_encryption) {
        session->setEncryptionMode(*m_encryption);
    }

    const auto loginJob = new KSmtp::LoginJob(session);
    loginJob->setUserName(m_username);
    loginJob->setPassword(m_password);
    if (m_authentication) {
        loginJob->setPreferedAuthMode(*m_authentication);
    }

    connect(loginJob, &KJob::finished, this, [this, loginJob] {
        if (loginJob->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "SMTP login failed:" << loginJob->errorString();
            setError(loginJob->error());
            setErrorText(loginJob->errorString());
        } else {
            qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "SMTP login succeeded";
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
