/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KAccountAutoconfigServer>

#include <QAbstractListModel>

class ServerListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged)

public:
    explicit ServerListModel(QObject *parent = nullptr);

    enum ServerDisplayRoles {
        UrlRole = Qt::UserRole + 1,
        HostRole,
        PortRole,
        ProtocolRole,
        AuthenticationTypeRole,
        SocketTypeRole,
    };

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setServers(const QList<KAccountAutoconfigServer> &servers);
    [[nodiscard]] bool isEmpty() const;
    KAccountAutoconfigServer &first();

Q_SIGNALS:
    void emptyChanged();

private:
    QList<KAccountAutoconfigServer> m_items;
};
