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

using namespace Qt::StringLiterals;

AccountBuilder::AccountBuilder(const QString &providerId)
    : QObject()
{
    m_accountsConfig = KSharedConfig::openConfig(u"konlineaccountsrc"_s);

    QStringList accounts = m_accountsConfig->group(u"Accounts"_s).readEntry("accounts", QStringList());

    m_id = QUuid::createUuid().toString(QUuid::WithoutBraces).replace(u"-"_s, u"_"_s);
    accounts << m_id;

    m_accountsConfig->group(u"Accounts"_s).writeEntry("accounts", accounts, KConfig::Notify);

    config().writeEntry("provider", providerId);
}

void AccountBuilder::finish()
{
    m_accountsConfig->sync();

    KWaylandExtras::requestXdgActivationToken(qApp->focusWindow(), KWaylandExtras::lastInputSerial(qApp->focusWindow()), QString());

    connect(
        KWaylandExtras::self(),
        &KWaylandExtras::xdgActivationTokenArrived,
        this,
        [this](int /*serial*/, const QString &token) {
            QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.private.KOnlineAccounts"_s,
                                                              u"/org/kde/KOnlineAccounts/private"_s,
                                                              u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                              u"sendAccountCreationFinished"_s);
            msg.setArguments({m_id, token});
            QDBusReply<void> r = QDBusConnection::sessionBus().call(msg);
            qWarning() << "reply" << r.error();
        },
        Qt::SingleShotConnection);

    Q_EMIT finished();
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
    config().sync();
}
