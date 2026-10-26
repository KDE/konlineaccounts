/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "imaplogintest.h"

#include <KIMAP/CapabilitiesJob>
#include <KIMAP/Session>

#include "setup_debug.h"

#include <debug.h>

IMAPLoginTest::IMAPLoginTest(QObject *parent)
    : LoginTestBase(parent)
    , m_authentication({})
    , m_encryption({})
{
}

IMAPLoginTest::~IMAPLoginTest() = default;

void IMAPLoginTest::setHostname(const QString &hostname)
{
    m_hostname = hostname;
}

void IMAPLoginTest::setPort(const int port)
{
    m_port = port;
}

void IMAPLoginTest::setSocketType(const KAutoconfigServer::SocketType &socket)
{
    switch (socket) {
    case KAutoconfigServer::UnknownSocketType:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Socket type unknown, letting the job decide which one to use";
        m_encryption = {}; // TODO: whether to error out for unknown?
        break;
    case KAutoconfigServer::Plain:
        m_encryption = KIMAP::LoginJob::Unencrypted;
        break;
    case KAutoconfigServer::SSL:
        m_encryption = KIMAP::LoginJob::SSLorTLS;
        break;
    case KAutoconfigServer::STARTTLS:
        m_encryption = KIMAP::LoginJob::STARTTLS;
        break;
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Socket type" << socket << "not handled";
        break;
    }
}

void IMAPLoginTest::setAuthenticationType(const KAutoconfigServer::AuthenticationType &authentication)
{
    requestedAuthenticationType = authentication;

    switch (requestedAuthenticationType) {
    case KAutoconfigServer::PasswordCleartext:
        m_authentication = KIMAP::LoginJob::AuthenticationMode::Plain;
        break;
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unhandled authentication type" << authentication;
        break;
    }
}

void IMAPLoginTest::login()
{
    const auto session = new KIMAP::Session(m_hostname, m_port, this);
    const auto login = new KIMAP::LoginJob(session);
    login->setUserName(m_username);
    login->setPassword(m_password);
    if (m_encryption.has_value()) {
        login->setEncryptionMode(m_encryption.value());
    }
    if (m_authentication.has_value()) {
        login->setAuthenticationMode(m_authentication.value());
        /*
         * TODO:
         *  - query server capabilities
         *  - keep the ones that match `requestedAuthenticationType`
         *  - try them one by one
         */
    }
    login->start();

    connect(login, &KJob::finished, this, [this, login] {
        if (login->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "IMAP login failed:" << login->errorString();
            setError(login->error());
            setErrorText(login->errorString());
        } else {
            qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "IMAP login succeeded";
        }

        emitResult();
    });
}