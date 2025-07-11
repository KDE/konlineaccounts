/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "account.h"

#include <QDBusMessage>

#include <KSharedConfig>

#include "accessmanager.h"

using namespace Qt::Literals;

Account::Account(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : QObject(parent)
{
    m_id = args.first().toString();
    auto config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);
    KConfigGroup accountsGroup = config->group(u"Accounts"_s);
    m_config = accountsGroup.group(m_id);
    m_metaData = md;
}

KConfigGroup Account::config() const
{
    return m_config;
}

void Account::grantAccess(const QString &appId)
{
    QStringList apps = m_config.readEntry("allowedApplications", QStringList());
    if (!apps.contains(appId)) {
        apps << appId;
        m_config.writeEntry("allowedApplications", apps);
    }
}

bool Account::hasAccess(const QString &appId) const
{
    return m_config.readEntry("allowedApplications", QStringList()).contains(appId);
}

bool Account::currentCallerHasAccess() const
{
    Q_ASSERT(calledFromDBus());
    return hasAccess(AccessManager::instance().appIdForService(message().service()));
}

QString Account::id() const
{
    return m_id;
}

QString Account::displayName() const
{
    return m_metaData.name();
}

QStringList Account::types() const
{
    return m_metaData.value(u"X-KOnlineAccounts-Provides"_s, QStringList());
}

QString Account::icon() const
{
    return m_metaData.iconName();
}
