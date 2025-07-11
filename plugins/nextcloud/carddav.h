/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QObject>

#include <KConfigGroup>

class CardDAV : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.KOnlineAccounts.CardDAV")

    Q_PROPERTY(QString url READ url)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString password READ password)

public:
    CardDAV(QObject *parent, KConfigGroup config);

    QString url() const;
    QString username() const;
    QString password() const;

private:
    KConfigGroup m_config;
};
