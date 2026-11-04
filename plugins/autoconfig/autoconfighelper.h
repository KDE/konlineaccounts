/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KAccountAutoconfigServer>

class AutoconfigHelper
{
public:
    static KAccountAutoconfigServer::Protocol parseProtocol(const QString &protocol)
    {
        std::map<QString, KAccountAutoconfigServer::Protocol> stringToProtocol{
            {QStringLiteral("IMAP"), KAccountAutoconfigServer::Protocol::IMAP},
            {QStringLiteral("POP3"), KAccountAutoconfigServer::Protocol::POP3},
            {QStringLiteral("SMTP"), KAccountAutoconfigServer::Protocol::SMTP},
            {QStringLiteral("CalDAV"), KAccountAutoconfigServer::Protocol::CalDAV},
            {QStringLiteral("CardDAV"), KAccountAutoconfigServer::Protocol::CardDAV},
        };
        Q_ASSERT(stringToProtocol.contains(protocol));
        return stringToProtocol[protocol];
    }

    static QString protocolToString(const KAccountAutoconfigServer::Protocol &protocol)
    {
        std::map<KAccountAutoconfigServer::Protocol, QString> protocolToString{
            {KAccountAutoconfigServer::Protocol::IMAP, QStringLiteral("IMAP")},
            {KAccountAutoconfigServer::Protocol::POP3, QStringLiteral("POP3")},
            {KAccountAutoconfigServer::Protocol::SMTP, QStringLiteral("SMTP")},
            {KAccountAutoconfigServer::Protocol::CalDAV, QStringLiteral("CalDAV")},
            {KAccountAutoconfigServer::Protocol::CardDAV, QStringLiteral("CardDAV")},
        };
        Q_ASSERT(protocolToString.contains(protocol));
        return protocolToString[protocol];
    }

    static QString authenticationTypeToString(const KAccountAutoconfigServer::AuthenticationType &authentication)
    {
        std::map<KAccountAutoconfigServer::AuthenticationType, QString> authenticationTypeToString{
            {KAccountAutoconfigServer::AuthenticationType::UnknownAuthentication, QStringLiteral("UnknownAuthentication")},
            {KAccountAutoconfigServer::AuthenticationType::Basic, QStringLiteral("Basic")},
            {KAccountAutoconfigServer::AuthenticationType::Digest, QStringLiteral("Digest")},
            {KAccountAutoconfigServer::AuthenticationType::PasswordCleartext, QStringLiteral("PasswordCleartext")},
            {KAccountAutoconfigServer::AuthenticationType::PasswordEncrypted, QStringLiteral("PasswordEncrypted")},
        };
        Q_ASSERT(authenticationTypeToString.contains(authentication));
        return authenticationTypeToString[authentication];
    }

    static QString socketTypeToString(const KAccountAutoconfigServer::SocketType &socket)
    {
        std::map<KAccountAutoconfigServer::SocketType, QString> socketTypeToString{
            {KAccountAutoconfigServer::SocketType::UnknownSocketType, QStringLiteral("UnknownSocketType")},
            {KAccountAutoconfigServer::SocketType::Plain, QStringLiteral("Plain")},
            {KAccountAutoconfigServer::SocketType::SSL, QStringLiteral("SSL")},
            {KAccountAutoconfigServer::SocketType::STARTTLS, QStringLiteral("STARTTLS")},
        };
        Q_ASSERT(socketTypeToString.contains(socket));
        return socketTypeToString[socket];
    }
};