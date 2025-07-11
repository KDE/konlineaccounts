/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>

#include <KConfigWatcher>
#include <KSharedConfig>

#include "accounts_export.h"
#include <qqmlregistration.h>

class ACCOUNTS_EXPORT AccountsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IdRole = Qt::UserRole + 1,
        TypesRole,
        IconNameRole,
    };

    AccountsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void removeAccount(const QString &id);

    void slotConfigChanged(const KConfigGroup &group, const QByteArrayList &names);

private:
    KSharedConfig::Ptr m_config;
    QStringList m_accountIds;
    KConfigWatcher::Ptr m_configWatcher;
};
