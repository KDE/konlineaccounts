/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "accountsfiltermodel.h"

#include "accountsmodel.h"

AccountsFilterModel::AccountsFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

QStringList AccountsFilterModel::types() const
{
    return m_types;
}

void AccountsFilterModel::setTypes(const QStringList &types)
{
    if (m_types != types) {
        m_types = types;
        Q_EMIT typesChanged();
    }
}

QString AccountsFilterModel::notAlreadyAllowedFor() const
{
    return m_notAlreadyAllowedFor;
}

void AccountsFilterModel::setNotAlreadyAllowedFor(const QString &appId)
{
    if (m_notAlreadyAllowedFor != appId) {
        m_notAlreadyAllowedFor = appId;
        Q_EMIT notAlreadyAllowedForChanged();
    }
}

bool AccountsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool ret = true;

    if (!m_types.isEmpty()) {
        ret = false;

        for (const QString &type : std::as_const(m_types)) {
            ret |= sourceModel()->data(index, AccountsModel::Roles::TypesRole).toStringList().contains(type);
        }
    }

    if (!m_notAlreadyAllowedFor.isEmpty()) {
        ret &= !sourceModel()->data(index, AccountsModel::Roles::AccountRole).value<Account *>()->hasAccess(m_notAlreadyAllowedFor);
    }

    return ret;
}
