/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "logintestbase.h"

#include <KAccountAutoconfig/KAutoconfigServer>
#include <KPOP/Pop3Settings>

class POPLoginTestJob : public LoginTestBase
{
    Q_OBJECT
public:
    explicit POPLoginTestJob(QObject *parent = nullptr);
    ~POPLoginTestJob() override;

    void setHostname(const QString &hostname);
    void setPort(int port);
    void setSocketType(const KAutoconfigServer::SocketType &socket);
    void setAuthenticationType(const KAutoconfigServer::AuthenticationType &authentication);

protected:
    void login() override;

private:
    QString m_hostname;
    int m_port = 110;
    Pop3Settings m_settings;
};
