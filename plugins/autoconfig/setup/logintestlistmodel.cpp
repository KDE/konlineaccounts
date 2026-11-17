/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestlistmodel.h"

#include "helpers.h"

#include <algorithm>

LoginTestListModel::LoginTestListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int LoginTestListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant LoginTestListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const auto [server, loginResult] = m_items.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QStringLiteral("Login %1 for server {url: %2, protocol: %3, authenticationType: %4, socketType: %5}")
            .arg(loginResult.first ? "succeeded" : "failed")
            .arg(server.url().toDisplayString())
            .arg(server.protocol())
            .arg(server.authenticationType())
            .arg(server.socketType());
    case UrlRole:
        return server.url();
    case HostRole:
        return server.url().host();
    case PortRole:
        return server.url().port();
    case ProtocolRole:
        return server.protocol();
    case AuthenticationTypeRole:
        return Helpers::authenticationTypeToVisibleString(server.authenticationType());
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

bool LoginTestListModel::nonEmptyAndAtLeastOneSuccess() const
{
    return std::ranges::any_of(m_items, [](const auto &item) {
        const auto [server, result] = item;
        const auto [loginSuccess, errorString] = result;
        return loginSuccess;
    });
}
