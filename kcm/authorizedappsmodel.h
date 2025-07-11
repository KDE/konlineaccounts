/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QAbstractListModel>
#include <qqmlregistration.h>

#include <KConfigGroup>
#include <KService>

class AuthorizedAppsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        IconNameRole = Qt::UserRole + 1,
        AppIdRole,
    };

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString accountId() const;
    void setAccountId(const QString &accountId);
    Q_SIGNAL void accountIdChanged();

private:
    QString m_accountId;
    KConfigGroup m_config;
};
