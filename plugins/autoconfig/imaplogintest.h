/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KAccountAutoconfigServer>
#include "logintestbase.h"

#include <KIMAP/LoginJob>

class IMAPLoginTest : public LoginTestBase
{
    Q_OBJECT

public:
    explicit IMAPLoginTest(QObject *parent = nullptr);
    ~IMAPLoginTest() override;

    void setHostname(const QString &hostname);
    void setPort(int port);
    void setSocketType(const KAccountAutoconfigServer::SocketType &socket);
    void setAuthenticationType(const KAccountAutoconfigServer::AuthenticationType &authentication);

protected:
    void login() override;

private:
    QString m_hostname;
    int m_port;
    KAccountAutoconfigServer::AuthenticationType requestedAuthenticationType;

    std::optional<KIMAP::LoginJob::AuthenticationMode> m_authentication;
    std::optional<KIMAP::LoginJob::EncryptionMode> m_encryption;
};