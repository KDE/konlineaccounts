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

namespace
{
std::optional<KIMAP::LoginJob::EncryptionMode> socketTypeToEncryptionMode(const KAccountAutoconfigServer::SocketType &socket)
{
    switch (socket) {
    case KAccountAutoconfigServer::Plain:
        return KIMAP::LoginJob::Unencrypted;
    case KAccountAutoconfigServer::SSL:
        return KIMAP::LoginJob::SSLorTLS;
    case KAccountAutoconfigServer::STARTTLS:
        return KIMAP::LoginJob::STARTTLS;
    case KAccountAutoconfigServer::UnknownSocketType:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Socket type unknown, letting the job decide which one to use";
        return {}; // TODO: whether to error out for unknown?
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Socket type" << socket << "not handled";
        return {};
    }
}

std::optional<KIMAP::LoginJob::AuthenticationMode> authenticationTypeToAuthenticationMode(const KAccountAutoconfigServer::AuthenticationType &authentication)
{
    switch (authentication) {
    case KAccountAutoconfigServer::PasswordCleartext:
        return KIMAP::LoginJob::AuthenticationMode::Plain;
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unhandled authentication type" << authentication;
        return {};
    }
}

}

IMAPLoginTest::IMAPLoginTest(const KAccountAutoconfigServer &server, QObject *parent)
    : LoginTestJobBase(server, parent)
{
}

IMAPLoginTest::~IMAPLoginTest() = default;

void IMAPLoginTest::login()
{
    const auto session = new KIMAP::Session(m_server.url().host(), m_server.url().port(), this);
    const auto login = new KIMAP::LoginJob(session);
    login->setUserName(m_username);
    login->setPassword(m_password);

    if (const auto encryption = socketTypeToEncryptionMode(m_server.socketType())) {
        login->setEncryptionMode(encryption.value());
    }
    if (const auto authentication = authenticationTypeToAuthenticationMode(m_server.authenticationType())) {
        login->setAuthenticationMode(authentication.value());
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