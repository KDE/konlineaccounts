/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "account.h"
#include "kaccountautoconfigserver.h"

#include <KConfigGroup>
#include <QDBusAbstractAdaptor>
#include <QDBusUnixFileDescriptor>

class IMAPAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.IMAP")

    Q_PROPERTY(QString hostname READ hostname)
    Q_PROPERTY(int port READ port)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(KAccountAutoconfigServer::SocketType socketType READ socketType)
    Q_PROPERTY(KAccountAutoconfigServer::AuthenticationType socketType READ authenticationType)

public:
    IMAPAdaptor(Account *account, const KConfigGroup &config);

    [[nodiscard]] QString hostname() const;
    [[nodiscard]] int port() const;
    [[nodiscard]] QString username() const;
    [[nodiscard]] KAccountAutoconfigServer::SocketType socketType() const;
    [[nodiscard]] KAccountAutoconfigServer::AuthenticationType authenticationType() const;

    Q_SCRIPTABLE [[nodiscard]] QDBusUnixFileDescriptor password() const;

private:
    KConfigGroup m_config;
    Account *m_account;
};
