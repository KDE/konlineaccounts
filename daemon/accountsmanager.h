/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusContext>
#include <QDBusObjectPath>
#include <QObject>
#include <qqmlregistration.h>

#include <QWindow>

#include <KConfigWatcher>

#include "account.h"

#include "accountsmanagerprivate.h"

class PropertiesAdaptor;

class AccountsManager : public QObject, public QDBusContext
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.Manager")

    Q_PROPERTY(QList<QDBusObjectPath> accounts READ accounts)
public:
    AccountsManager();
    virtual ~AccountsManager()
    {
    }

    QList<QDBusObjectPath> accounts() const;

    Q_SCRIPTABLE void requestAccount(const QStringList &types);

    Q_INVOKABLE void accountSelected(const QString &callerId, const QString &name);
    Q_INVOKABLE void requestAccountCreation(const QStringList &types);

    Q_SCRIPTABLE void registerApp(const QString &appId);

    QList<Account *> accountsInternal() const;

Q_SIGNALS:
    Q_SCRIPTABLE void accountRemoved(const QDBusObjectPath &account);
    Q_SCRIPTABLE void accountAccessGranted(const QDBusObjectPath &account, const QString &xdgActivationToken);

private:
    void slotAccountCreationFinished(const QString &id, const QString &xdgActivationToken);
    void slotRemoveAccount(const QString &id);
    void configChanged(const KConfigGroup &group, const QByteArrayList &names);
    QString callerId() const;

    std::map<QString, std::unique_ptr<Account>> m_accounts;
    std::unique_ptr<AccountsManagerPrivate> m_private;
    QWindow *m_window = nullptr;
    QString m_requester;
};
