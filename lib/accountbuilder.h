/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>
#include <qqmlregistration.h>

#include <KSharedConfig>

#include "accounts_export.h"

class ACCOUNTS_EXPORT AccountBuilder : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QStringList authorizedApps READ authorizedApps WRITE setAuthorizedApps NOTIFY authorizedAppsChanged)
public:
    AccountBuilder(const QString &providerId);

    void finish();
    void fail(const QString &errorMessage);

    KConfigGroup config() const;

    QStringList authorizedApps() const;
    void setAuthorizedApps(const QStringList &apps);
    Q_SIGNAL void authorizedAppsChanged();

Q_SIGNALS:
    void finished();
    void failed(const QString &errorMessage);

private:
    KSharedConfig::Ptr m_accountsConfig;
    QString m_id;
    QStringList m_authorizedApps;
};
