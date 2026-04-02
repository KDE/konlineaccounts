/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QDBusUnixFileDescriptor>
#include <QObject>

#include <KConfigGroup>

#include "account.h"

class GoogleInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.Google")

    Q_PROPERTY(QString clientId READ clientId)
    Q_PROPERTY(QString clientSecret READ clientSecret)
    Q_PROPERTY(QDBusUnixFileDescriptor accessToken READ accessToken)
    Q_PROPERTY(QDBusUnixFileDescriptor refreshToken READ refreshToken)
    Q_PROPERTY(QStringList scopes READ scopes)

public:
    GoogleInterface(Account *account, KConfigGroup config);

    QString clientId() const;
    QString clientSecret() const;
    QDBusUnixFileDescriptor accessToken() const;
    QDBusUnixFileDescriptor refreshToken() const;
    QStringList scopes() const;

private:
    KConfigGroup m_config;
    Account *m_account;
};
