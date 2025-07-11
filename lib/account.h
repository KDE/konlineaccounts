/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusContext>
#include <QObject>

#include <KConfigGroup>
#include <KPluginMetaData>

#include "accounts_export.h"

class ACCOUNTS_EXPORT Account : public QObject, public QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.Account")

    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(QStringList types READ types NOTIFY typesChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
public:
    Account(QObject *parent, const KPluginMetaData &md, const QVariantList &args);

    // override for custom dynamic display name
    virtual QString displayName() const;
    Q_SIGNAL void displayNameChanged();

    // override for custom dynamic types
    virtual QStringList types() const;
    Q_SIGNAL void typesChanged();

    // override for custom dynamic icon
    virtual QString icon() const;
    Q_SIGNAL void iconChanged();

    KConfigGroup config() const;

    void grantAccess(const QString &appId);
    bool hasAccess(const QString &appId) const;
    bool currentCallerHasAccess() const;

    QString id() const;

private:
    KConfigGroup m_config;
    QString m_id;
    KPluginMetaData m_metaData;
};
