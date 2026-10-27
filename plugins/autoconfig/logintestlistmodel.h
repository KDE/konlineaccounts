/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KAccountAutoconfigServer>

#include <QAbstractListModel>

class LoginTestListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LoginTestListModel(QObject *parent);

    enum LoginTestDisplayRoles {
        UrlRole = Qt::UserRole + 1,
        HostRole,
        PortRole,
        ProtocolRole,
        AuthenticationTypeRole,
        SocketTypeRole,
        LoginResultRole,
    };

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    void setLoginTestResult(const QList<QPair<KAccountAutoconfigServer, bool>> &result);

    [[nodiscard]] QList<QPair<KAccountAutoconfigServer, bool>> getItems() const;

Q_SIGNALS:
    void countChanged();

private:
    QList<QPair<KAccountAutoconfigServer, bool>> m_items;
};
