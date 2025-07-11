/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "authorizedappsmodel.h"

#include <KSharedConfig>

using namespace Qt::StringLiterals;

QVariant AuthorizedAppsModel::data(const QModelIndex &index, int role) const
{
    const QString appId = m_config.readEntry("allowedApplications", QStringList()).at(index.row());

    const auto service = KService::serviceByDesktopName(appId);

    switch (static_cast<Roles>(role)) {
    case NameRole:
        return service ? service->name() : u"Foo"_s;
    case AppIdRole:
        return appId;
    case IconNameRole:
        return service ? service->icon() : u"cuttlefish"_s;
    }

    return QVariant();
}

int AuthorizedAppsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_config.readEntry("allowedApplications", QStringList()).size();
}

QHash<int, QByteArray> AuthorizedAppsModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {AppIdRole, "appId"},
        {IconNameRole, "iconName"},
    };
}

QString AuthorizedAppsModel::accountId() const
{
    return m_accountId;
}

void AuthorizedAppsModel::setAccountId(const QString &accountId)
{
    m_accountId = accountId;
    Q_EMIT accountIdChanged();

    m_config = KSharedConfig::openConfig(u"konlineaccountsrc"_s)->group(u"Accounts"_s).group(accountId);
}
