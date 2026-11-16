/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QDBusUnixFileDescriptor>

#include "account.h"

class CardDAVAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.CardDAV")

    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QString username READ username)

public:
    CardDAVAdaptor(Account *account, const KConfigGroup &config);

    [[nodiscard]] QString url() const;
    [[nodiscard]] QString username() const;

    Q_SCRIPTABLE [[nodiscard]] QDBusUnixFileDescriptor password() const;

private:
    KConfigGroup m_config;
    Account *m_account;
};
