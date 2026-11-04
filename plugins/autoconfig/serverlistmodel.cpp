/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "serverlistmodel.h"

ServerListModel::ServerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ServerListModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant ServerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const auto &item = m_items.at(index.row());
    switch (role) {
    case UrlRole:
        return item.url();
    case HostRole:
        return item.url().host();
    case PortRole:
        return item.url().port();
    case ProtocolRole:
        return item.protocol();
    case AuthenticationTypeRole:
        return item.authenticationType();
    case SocketTypeRole:
        return item.socketType();
    default:
        return {};
    }
}

QHash<int, QByteArray> ServerListModel::roleNames() const
{
    return {
        {UrlRole, "url"},
        {HostRole, "host"},
        {PortRole, "port"},
        {ProtocolRole, "protocol"},
        {AuthenticationTypeRole, "authenticationType"},
        {SocketTypeRole, "socketType"},
    };
}

void ServerListModel::setServers(const QList<KAccountAutoconfigServer> &servers)
{
    beginResetModel();
    m_items = servers;
    endResetModel();
    Q_EMIT countChanged();
}

bool ServerListModel::isEmpty() const
{
    return m_items.isEmpty();
}

KAccountAutoconfigServer &ServerListModel::first()
{
    return m_items.first();
}