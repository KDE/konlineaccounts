/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>

#include <KSharedConfig>

class AvailableAccountsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IconNameRole = Qt::DecorationRole,
        SetupModuleRole = Qt::UserRole + 1,
        DescriptionRole,
        ProviderRole,
        ProvidesRole,
    };

    AvailableAccountsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Data {
        QString name;
        QString description;
        QString iconName;
        QString setupModule;
        QString provider;
        QStringList provides;
    };

    QList<Data> m_data;
};
