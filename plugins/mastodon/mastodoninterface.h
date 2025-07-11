/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QObject>

#include <KConfigGroup>

class MastodonInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.Mastodon")

    Q_PROPERTY(QString instanceUrl READ instanceUrl)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString clientId READ clientId)
    Q_PROPERTY(QString clientSecret READ clientSecret)
    Q_PROPERTY(QString accessToken READ accessToken)

public:
    MastodonInterface(QObject *parent, KConfigGroup config);

    QString instanceUrl() const;
    QString username() const;
    QString clientId() const;
    QString clientSecret() const;
    QString accessToken() const;

private:
    KConfigGroup m_config;
};
