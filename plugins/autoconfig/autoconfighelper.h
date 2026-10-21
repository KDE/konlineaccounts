/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "kautoconfigserver.h"

class AutoconfigHelper
{
public:
    static KAutoconfigServer::Protocol parseProtocol(const QString &protocol)
    {
        std::map<QString, KAutoconfigServer::Protocol> stringToProtocol{
            {QStringLiteral("IMAP"), KAutoconfigServer::Protocol::IMAP},
            {QStringLiteral("POP3"), KAutoconfigServer::Protocol::POP3},
            {QStringLiteral("SMTP"), KAutoconfigServer::Protocol::SMTP},
            {QStringLiteral("CalDAV"), KAutoconfigServer::Protocol::CalDAV},
            {QStringLiteral("CardDAV"), KAutoconfigServer::Protocol::CardDAV},
        };
        Q_ASSERT(stringToProtocol.contains(protocol));
        return stringToProtocol[protocol];
    }

    static QString protocolToString(const KAutoconfigServer::Protocol &protocol)
    {
        std::map<KAutoconfigServer::Protocol, QString> protocolToString{
            {KAutoconfigServer::Protocol::IMAP, QStringLiteral("IMAP")},
            {KAutoconfigServer::Protocol::POP3, QStringLiteral("POP3")},
            {KAutoconfigServer::Protocol::SMTP, QStringLiteral("SMTP")},
            {KAutoconfigServer::Protocol::CalDAV, QStringLiteral("CalDAV")},
            {KAutoconfigServer::Protocol::CardDAV, QStringLiteral("CardDAV")},
        };
        Q_ASSERT(protocolToString.contains(protocol));
        return protocolToString[protocol];
    }
};