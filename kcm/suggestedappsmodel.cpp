/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "suggestedappsmodel.h"

#include <KApplicationTrader>
#include <KService>

using namespace Qt::StringLiterals;

SuggestedAppsModel::SuggestedAppsModel()
    : QAbstractListModel()
{
}

QVariant SuggestedAppsModel::data(const QModelIndex &index, int role) const
{
    switch (static_cast<Roles>(role)) {
    case NameRole:
        return m_apps[index.row()]->name();
    case AppIdRole:
        return m_apps[index.row()]->desktopEntryName();
    case IconNameRole:
        return m_apps[index.row()]->icon();
    case DescriptionRole:
        return m_apps[index.row()]->comment();
    }

    return QVariant();
}

int SuggestedAppsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_apps.count();
}

QHash<int, QByteArray> SuggestedAppsModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {AppIdRole, "appId"},
        {IconNameRole, "iconName"},
        {DescriptionRole, "description"},
    };
}

QStringList SuggestedAppsModel::types() const
{
    return m_types;
}

// TODO should this rather fetch the types from the daemon
void SuggestedAppsModel::setTypes(const QStringList &types)
{
    beginResetModel();
    m_types = types;

    m_apps = KApplicationTrader::query([this](const KService::Ptr &service) -> bool {
        return std::any_of(m_types.constBegin(), m_types.constEnd(), [&service](const QString &type) {
            return service->property<QStringList>(u"X-KDE-OnlineAccounts-Types"_s).contains(type);
        });
    });

    Q_EMIT typesChanged();
    endResetModel();
}
