/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QDBusUnixFileDescriptor>

#include "account.h"

class AutoconfigInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.Autoconfig")

public:
    AutoconfigInterface(Account *account, KConfigGroup config);

private:
    KConfigGroup m_config;
    Account *m_account;
};
