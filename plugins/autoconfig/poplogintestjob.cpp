/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "poplogintestjob.h"
#include "setup_debug.h"
#include <KPOP/LoginJob>

POPLoginTestJob::POPLoginTestJob(QObject *parent)
    : LoginTestBase(parent)
{
}

POPLoginTestJob::~POPLoginTestJob() = default;

void POPLoginTestJob::setHostname(const QString &hostname)
{
    m_settings.setHost(hostname);
}

void POPLoginTestJob::setPort(const int port)
{
    m_settings.setPort(port);
}

void POPLoginTestJob::setSocketType(const KAutoconfigServer::SocketType &socket)
{
    switch (socket) {
    case KAutoconfigServer::Plain:
        m_settings.setUseSSL(false);
        m_settings.setUseTLS(false);
        break;
    case KAutoconfigServer::SSL:
        m_settings.setUseSSL(true);
        m_settings.setUseTLS(false);
        break;
    case KAutoconfigServer::STARTTLS:
        m_settings.setUseSSL(false);
        m_settings.setUseTLS(true);
        break;
    case KAutoconfigServer::UnknownSocketType:
    default:
        break;
    }
}

void POPLoginTestJob::setAuthenticationType(const KAutoconfigServer::AuthenticationType &authentication)
{
    switch (authentication) {
    case KAutoconfigServer::PasswordCleartext:
        m_settings.setAuthenticationMethod(Pop3Settings::Clear);
        break;
    case KAutoconfigServer::PasswordEncrypted:
        m_settings.setAuthenticationMethod(Pop3Settings::CramMD5);
        break;
    case KAutoconfigServer::Digest:
    case KAutoconfigServer::Basic:
    case KAutoconfigServer::UnknownAuthentication:
    default:
        break;
    }
}

void POPLoginTestJob::login()
{
    m_settings.setLogin(m_username);
    const auto session = new KPOP::Session(m_settings, m_password);
    const auto loginJob = new KPOP::LoginJob(session);
    connect(loginJob, &KJob::finished, this, [this, loginJob] {
        if (loginJob->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "POP login failed:" << loginJob->errorString();
            setError(loginJob->error());
            setErrorText(loginJob->errorString());
        } else {
            qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "POP login succeeded";
        }
        emitResult();
    });
    loginJob->start();
}


