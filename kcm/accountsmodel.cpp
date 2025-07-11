/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accountsmodel.h"

#include <KConfigGroup>
#include <KConfigWatcher>
#include <KSharedConfig>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>

using namespace Qt::Literals;

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);
    m_accountIds = m_config->group(u"Accounts"_s).readEntry("accounts", QStringList());
    m_configWatcher = KConfigWatcher::create(m_config);
    connect(m_configWatcher.get(), &KConfigWatcher::configChanged, this, &AccountsModel::slotConfigChanged);
}

int AccountsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_accountIds.count();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    const QString id = m_accountIds[index.row()];

    KConfigGroup accountGroup = m_config->group(u"Accounts"_s).group(id);

    switch (static_cast<Roles>(role)) {
    case NameRole: {
        QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.KOnlineAccounts"_s,
                                                          u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                          u"org.freedesktop.DBus.Properties"_s,
                                                          u"Get"_s);
        msg.setArguments({u"org.kde.KOnlineAccounts.Account"_s, u"displayName"_s});

        QDBusReply<QVariant> reply = QDBusConnection::sessionBus().call(msg);

        qWarning() << "r" << reply.error();

        return reply.value().toString();
    }
    case IdRole: {
        return id;
    }
    case TypesRole: {
        QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.KOnlineAccounts"_s,
                                                          u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                          u"org.freedesktop.DBus.Properties"_s,
                                                          u"Get"_s);
        msg.setArguments({u"org.kde.KOnlineAccounts.Account"_s, u"types"_s});

        QDBusReply<QVariant> reply = QDBusConnection::sessionBus().call(msg);

        qWarning() << "r" << reply.error();

        return reply.value().toStringList();
    }
    case IconNameRole: {
        QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.KOnlineAccounts"_s,
                                                          u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                          u"org.freedesktop.DBus.Properties"_s,
                                                          u"Get"_s);
        msg.setArguments({u"org.kde.KOnlineAccounts.Account"_s, u"icon"_s});

        QDBusReply<QVariant> reply = QDBusConnection::sessionBus().call(msg);

        qWarning() << "r" << reply.error();

        return reply.value().toString();
    }
    }
    return {};
}

void AccountsModel::removeAccount(const QString &id)
{
    int index = m_accountIds.indexOf(id);

    if (index < 0) {
        return;
    }

    beginRemoveRows({}, index, index);

    QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.private.KOnlineAccounts"_s,
                                                      u"/org/kde/KOnlineAccounts/private"_s,
                                                      u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                      u"removeAccount"_s);
    msg.setArguments({id});

    QDBusReply<void> reply = QDBusConnection::sessionBus().call(msg);

    qWarning() << "r" << reply.error();

    endRemoveRows();
}

void AccountsModel::slotConfigChanged(const KConfigGroup & /*group*/, const QByteArrayList & /*names*/)
{
    beginResetModel();
    m_accountIds = m_config->group(u"Accounts"_s).readEntry("accounts", QStringList());
    endResetModel();
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    return {
        {Roles::NameRole, "name"},
        {Roles::IdRole, "id"},
        {Roles::TypesRole, "types"},
        {Roles::IconNameRole, "iconName"},
    };
}
