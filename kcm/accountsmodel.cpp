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
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusReply>

#include "debug.h"

using namespace Qt::Literals;

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(u"org.kde.KOnlineAccounts"_s)) {
        m_accountIds = m_config->group(u"Accounts"_s).readEntry("accounts", QStringList());
    }

    bool ret = QDBusConnection::sessionBus().connect(u"org.kde.private.KOnlineAccounts"_s,
                                                     u"/org/kde/KOnlineAccounts/private"_s,
                                                     u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                     u"accountAdded"_s,
                                                     this,
                                                     SLOT(slotAccountAdded(QString)));
    Q_ASSERT(ret);

    ret = QDBusConnection::sessionBus().connect(u"org.kde.private.KOnlineAccounts"_s,
                                                u"/org/kde/KOnlineAccounts/private"_s,
                                                u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                u"accountRemoved"_s,
                                                this,
                                                SLOT(slotAccountRemoved(QString)));
    Q_ASSERT(ret);
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

        if (!reply.isValid()) {
            qCWarning(LOG_KONLINEACCOUNTS_KCM) << "Error reading account name" << reply.error().message();
            return {};
        }

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

        if (!reply.isValid()) {
            qCWarning(LOG_KONLINEACCOUNTS_KCM) << "Error reading account type" << reply.error().message();
            return {};
        }

        return reply.value().toStringList();
    }
    case IconNameRole: {
        QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.KOnlineAccounts"_s,
                                                          u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                          u"org.freedesktop.DBus.Properties"_s,
                                                          u"Get"_s);
        msg.setArguments({u"org.kde.KOnlineAccounts.Account"_s, u"icon"_s});

        QDBusReply<QVariant> reply = QDBusConnection::sessionBus().call(msg);

        if (!reply.isValid()) {
            qCWarning(LOG_KONLINEACCOUNTS_KCM) << "Error reading account icon" << reply.error().message();
            return {};
        }

        return reply.value().toString();
    }
    }
    return {};
}

void AccountsModel::removeAccount(const QString &id)
{
    QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.private.KOnlineAccounts"_s,
                                                      u"/org/kde/KOnlineAccounts/private"_s,
                                                      u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                      u"removeAccount"_s);
    msg.setArguments({id});

    QDBusReply<void> reply = QDBusConnection::sessionBus().call(msg);

    if (!reply.isValid()) {
        qCWarning(LOG_KONLINEACCOUNTS_KCM) << "Error removing account" << reply.error().message();
    }
}

bool AccountsModel::hasAccount(const QString &id)
{
    return m_accountIds.contains(id);
}

void AccountsModel::slotAccountAdded(const QString &id)
{
    qCDebug(LOG_KONLINEACCOUNTS_KCM) << "Account" << id << "was added";
    beginResetModel();
    m_config->reparseConfiguration();
    m_accountIds = m_config->group(u"Accounts"_s).readEntry("accounts", QStringList());
    endResetModel();
}

void AccountsModel::slotAccountRemoved(const QString &id)
{
    qCDebug(LOG_KONLINEACCOUNTS_KCM) << "Account" << id << "was removed";
    beginResetModel();
    m_config->reparseConfiguration();
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
