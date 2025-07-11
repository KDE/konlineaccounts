/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>
#include <qqmlregistration.h>

#include <KSharedConfig>

#include "accountsmanager.h"

class AccountsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(AccountsManager *manager READ manager WRITE setManager NOTIFY managerChanged)

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IdRole = Qt::UserRole + 1,
        TypesRole,
        IconNameRole,
        AccountRole,
    };

    AccountsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    AccountsManager *manager() const;
    void setManager(AccountsManager *manager);
    Q_SIGNAL void managerChanged();

private:
    KSharedConfig::Ptr m_config;
    QList<Account *> m_accounts;
    AccountsManager *m_manager;
};
