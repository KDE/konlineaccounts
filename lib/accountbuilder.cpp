/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accountbuilder.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QGuiApplication>

#include <KConfigGroup>
#include <KWaylandExtras>

#include "debug.h"

using namespace Qt::StringLiterals;

AccountBuilder::AccountBuilder(const QString &providerId)
    : QObject()
{
    m_accountsConfig = KSharedConfig::openConfig(u"konlineaccountsrc"_s);

    QStringList accounts = m_accountsConfig->group(u"Accounts"_s).readEntry("accounts", QStringList());

    m_id = QUuid::createUuid().toString(QUuid::WithoutBraces).replace(u"-"_s, u"_"_s);
    accounts << m_id;

    m_accountsConfig->group(u"Accounts"_s).writeEntry("accounts", accounts);

    config().writeEntry("provider", providerId);
}

void AccountBuilder::finish()
{
    Q_EMIT finished();
}

void AccountBuilder::apply()
{
    m_accountsConfig->sync();
    config().sync();
}

void AccountBuilder::fail(const QString &errorMessage)
{
    Q_EMIT failed(errorMessage);
}

KConfigGroup AccountBuilder::config() const
{
    return m_accountsConfig->group(u"Accounts"_s).group(m_id);
}

QStringList AccountBuilder::authorizedApps() const
{
    return m_authorizedApps;
}

void AccountBuilder::setAuthorizedApps(const QStringList &apps)
{
    m_authorizedApps = apps;

    Q_EMIT authorizedAppsChanged();

    config().writeEntry("allowedApplications", apps);
}

QString AccountBuilder::accountId() const
{
    return m_id;
}
