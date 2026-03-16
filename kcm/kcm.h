/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ACCOUNTSSETTINGS_H
#define ACCOUNTSSETTINGS_H

#include <QObject>
#include <QVariant>

#include <KQuickConfigModule>

#include "accountbuilder.h"

class AccountsSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(QStringList requestedTypes READ requestedTypes NOTIFY requestedTypesChanged)
    Q_PROPERTY(QString requestedAccount READ requestedAccount NOTIFY requestedAccountChanged)
    Q_PROPERTY(bool serviceRunning READ serviceRunning NOTIFY serviceRunningChanged)

public:
    explicit AccountsSettings(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    QStringList requestedTypes() const;
    Q_SIGNAL void requestedTypesChanged();

    QString requestedAccount() const;
    Q_SIGNAL void requestedAccountChanged();

    bool serviceRunning() const;
    Q_SIGNAL void serviceRunningChanged();

    Q_INVOKABLE AccountBuilder *createBuilder(const QString &provider);

    Q_INVOKABLE QString accountName(const QString &accountId);

private:
    QStringList m_requestedTypes;
    QString m_requestedAccount;
};

#endif // ACCOUNTSSETTINGS_H
