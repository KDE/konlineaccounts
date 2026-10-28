/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "logintestjobbase.h"

#include <KAccountAutoconfigServer>
#include <KSMTP/LoginJob>
#include <KSMTP/Session>

class SMTPLoginTestJob : public LoginTestJobBase
{
    Q_OBJECT
public:
    explicit SMTPLoginTestJob(QObject *parent = nullptr);
    ~SMTPLoginTestJob() override;

    void setHostname(const QString &hostname);
    void setPort(int port);
    void setSocketType(const KAccountAutoconfigServer::SocketType &socket);
    void setAuthenticationType(const KAccountAutoconfigServer::AuthenticationType &authentication);

protected:
    void login() override;

private:
    QString m_hostname;
    int m_port = 110;
    std::optional<KSmtp::LoginJob::AuthMode> m_authentication;
    std::optional<KSmtp::Session::EncryptionMode> m_encryption;

    void handleSmtpConnectionError(const QString &error);

};
