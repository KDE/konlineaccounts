/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "availableaccountsmodel.h"

#include <KConfigGroup>
#include <KPluginMetaData>
#include <KSharedConfig>

using namespace Qt::Literals;

AvailableAccountsModel::AvailableAccountsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    const auto plugins = KPluginMetaData::findPlugins(u"konlineaccounts/providers"_s);

    for (auto plugin : plugins) {
        m_data << Data{
            .name = plugin.name(),
            .description = plugin.description(),
            .iconName = plugin.iconName(),
            .setupModule = plugin.value(u"X-KOnlineAccounts-SetupModule"_s),
            .provider = plugin.pluginId(),
            .provides = plugin.value(u"X-KOnlineAccounts-Provides"_s, QStringList()),
        };
    }
}

int AvailableAccountsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_data.count();
}

QVariant AvailableAccountsModel::data(const QModelIndex &index, int role) const
{
    switch (static_cast<Roles>(role)) {
    case NameRole: {
        return m_data[index.row()].name;
    }
    case IconNameRole: {
        return m_data[index.row()].iconName;
    }
    case SetupModuleRole: {
        return m_data[index.row()].setupModule;
    }
    case DescriptionRole: {
        return m_data[index.row()].description;
    }
    case ProviderRole: {
        return m_data[index.row()].provider;
    }
    case ProvidesRole: {
        return m_data[index.row()].provides;
    }
    }
    return {};
}

QHash<int, QByteArray> AvailableAccountsModel::roleNames() const
{
    return {
        {Roles::NameRole, "name"},
        {Roles::IconNameRole, "iconName"},
        {Roles::SetupModuleRole, "setupModule"},
        {Roles::DescriptionRole, "description"},
        {Roles::ProviderRole, "provider"},
        {Roles::ProvidesRole, "provides"},
    };
}
