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

class CalDAV : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.CalDAV")

    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QDBusUnixFileDescriptor password READ password)

public:
    CalDAV(Account *account, KConfigGroup config);

    QString url() const;
    QString username() const;
    QDBusUnixFileDescriptor password() const;

private:
    KConfigGroup m_config;
    Account *m_account;
};
