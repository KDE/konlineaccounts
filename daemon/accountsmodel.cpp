/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accountsmodel.h"

#include <KConfigGroup>
#include <KSharedConfig>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>

using namespace Qt::Literals;

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AccountsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_accounts.count();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    switch (static_cast<Roles>(role)) {
    case NameRole: {
        return m_accounts[index.row()]->displayName();
    }
    case IdRole: {
        return m_accounts[index.row()]->id();
    }
    case TypesRole: {
        return m_accounts[index.row()]->types();
    }
    case IconNameRole: {
        return m_accounts[index.row()]->icon();
    }
    case AccountRole: {
        return QVariant::fromValue(m_accounts[index.row()]);
    }
    }
    return {};
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    return {
        {Roles::NameRole, "name"},
        {Roles::IdRole, "id"},
        {Roles::TypesRole, "types"},
        {Roles::IconNameRole, "iconName"},
    };
}

AccountsManager *AccountsModel::manager() const
{
    return m_manager;
}

void AccountsModel::setManager(AccountsManager *manager)
{
    m_manager = manager;
    Q_EMIT managerChanged();

    beginResetModel();
    m_accounts = manager->accountsInternal();
    endResetModel();

    // TODO watch for changes?
}
