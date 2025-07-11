/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QAbstractListModel>
#include <qqmlregistration.h>

#include <KService>

class SuggestedAppsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList types READ types WRITE setTypes NOTIFY typesChanged)

    enum Roles {
        NameRole = Qt::DisplayRole,
        AppIdRole = Qt::UserRole + 1,
        IconNameRole,
        DescriptionRole,
    };

public:
    SuggestedAppsModel();

    QStringList types() const;
    void setTypes(const QStringList &types);
    Q_SIGNAL void typesChanged();

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    KService::List m_apps;
    QStringList m_types;
};
