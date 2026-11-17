/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "setup_debug.h"

#include "helpers.h"

#include <qmetaobject.h>

QString Helpers::authenticationTypeToVisibleString(const KAccountAutoconfigServer::AuthenticationType &authentication)
{
    switch (authentication) {
    case KAccountAutoconfigServer::UnknownAuthentication:
        return QStringLiteral("Unknown");
    case KAccountAutoconfigServer::Basic:
        return QStringLiteral("Basic");
    case KAccountAutoconfigServer::Digest:
        return QStringLiteral("Digest");
    case KAccountAutoconfigServer::OAuth2:
        return QStringLiteral("OAuth2");
    case KAccountAutoconfigServer::PasswordCleartext:
        return QStringLiteral("Cleartext password");
    case KAccountAutoconfigServer::PasswordEncrypted:
        return QStringLiteral("Encrypted password");
    case KAccountAutoconfigServer::NTLM:
        return QStringLiteral("NTLM");
    case KAccountAutoconfigServer::GSSAPI:
        return QStringLiteral("GSSAPI");
    case KAccountAutoconfigServer::TlsClientCert:
        return QStringLiteral("TLS client certificate");
    case KAccountAutoconfigServer::ClientIpAddress:
        return QStringLiteral("Client IP address");
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unsupported authentication type" << authentication;
        return QString::fromUtf8(QMetaEnum::fromType<KAccountAutoconfigServer::AuthenticationType>().valueToKey(authentication));
    }
}
