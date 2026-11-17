/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "serverlistmodel.h"
#include "helpers.h"

#include "setup_debug.h"

#include <qmetaobject.h>

ServerListModel::ServerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ServerListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant ServerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.parent().isValid() || index.column() != 0 || index.row() < 0 || index.row() >= m_items.size())
        return {};

    const auto &item = m_items.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QStringLiteral("KAccountAutoconfigServer{url: %1, protocol: %2, authenticationType: %3, socketType: %4}")
            .arg(item.url().toDisplayString())
            .arg(item.protocol())
            .arg(item.authenticationType())
            .arg(item.socketType());
    case UrlRole:
        return item.url();
    case HostRole:
        return item.url().host();
    case PortRole:
        return item.url().port();
    case ProtocolRole:
        return item.protocol();
    case AuthenticationTypeRole:
        return Helpers::authenticationTypeToVisibleString(item.authenticationType());
    case SocketTypeRole:
        return item.socketType();
    default:
        return {};
    }
}

QHash<int, QByteArray> ServerListModel::roleNames() const
{
    auto parentRoles = QAbstractListModel::roleNames();
    parentRoles.insert(QHash<int, QByteArray>{
        {UrlRole, "url"},
        {HostRole, "host"},
        {PortRole, "port"},
        {ProtocolRole, "protocol"},
        {AuthenticationTypeRole, "authenticationType"},
        {SocketTypeRole, "socketType"},
    });
    return parentRoles;
}

void ServerListModel::setServers(const QList<KAccountAutoconfigServer> &servers)
{
    beginResetModel();
    m_items = servers;
    endResetModel();
}
