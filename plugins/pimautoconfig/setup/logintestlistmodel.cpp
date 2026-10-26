/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestlistmodel.h"

LoginTestListModel::LoginTestListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int LoginTestListModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant LoginTestListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const auto [server, loginResult] = m_items.at(index.row());
    switch (role) {
    case UrlRole:
        return server.url();
    case HostRole:
        return server.url().host();
    case PortRole:
        return server.url().port();
    case ProtocolRole:
        return server.protocol();
    case AuthenticationTypeRole:
        return server.authenticationType();
    case SocketTypeRole:
        return server.socketType();
    case LoginResultRole:
        return loginResult.first;
    case LoginResultErrorStringRole:
        return loginResult.second;
    default:
        return {};
    }
}

QHash<int, QByteArray> LoginTestListModel::roleNames() const
{
    return {
        {UrlRole, "url"},
        {HostRole, "host"},
        {PortRole, "port"},
        {ProtocolRole, "protocol"},
        {AuthenticationTypeRole, "authenticationType"},
        {SocketTypeRole, "socketType"},
        {LoginResultRole, "loginResult"},
        {LoginResultErrorStringRole, "loginResultErrorString"},
    };
}

void LoginTestListModel::setLoginTestResult(const QList<QPair<KAccountAutoconfigServer, QPair<bool, QString>>> &result)
{
    beginResetModel();
    m_items = result;
    endResetModel();
    Q_EMIT countChanged();
}

QList<QPair<KAccountAutoconfigServer, QPair<bool, QString>>> LoginTestListModel::getItems() const
{
    return m_items;
}