/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>

class AccountsManagerPrivate : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.ManagerPrivate")

public:
    Q_SCRIPTABLE void sendAccountCreationFinished(const QString &id, const QString &xdgActivationToken);
    Q_SCRIPTABLE void removeAccount(const QString &id);

Q_SIGNALS:
    void accountCreationFinished(const QString &id, const QString &xdgActivationToken);
    void accountRemovalRequested(const QString &id);
};
