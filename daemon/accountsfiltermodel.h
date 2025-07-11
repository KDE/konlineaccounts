/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <QSortFilterProxyModel>

#include "qqmlregistration.h"

class AccountsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QStringList types READ types WRITE setTypes NOTIFY typesChanged)
    Q_PROPERTY(QString notAlreadyAllowedFor READ notAlreadyAllowedFor WRITE setNotAlreadyAllowedFor NOTIFY notAlreadyAllowedForChanged)
public:
    AccountsFilterModel(QObject *parent = nullptr);

    QStringList types() const;
    void setTypes(const QStringList &types);
    Q_SIGNAL void typesChanged();

    QString notAlreadyAllowedFor() const;
    void setNotAlreadyAllowedFor(const QString &appId);
    Q_SIGNAL void notAlreadyAllowedForChanged();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QStringList m_types;
    QString m_notAlreadyAllowedFor;
};
