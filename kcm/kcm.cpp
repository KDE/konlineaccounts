/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kcm.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQuickRenderControl>
#include <QUrlQuery>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>
#include <KWaylandExtras>

#include "accountsmodel.h"
#include "authorizedappsmodel.h"
#include "availableaccountsmodel.h"
#include "debug.h"
#include "suggestedappsmodel.h"

using namespace Qt::Literals;

K_PLUGIN_CLASS_WITH_JSON(AccountsSettings, "kcm_konlineaccounts.json")

AccountsSettings::AccountsSettings(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KQuickConfigModule(parent, data)
{
    if (args.length() == 2 && args.first().toString() == u"add") {
        m_requestedTypes = args[1].toString().split(u';');
    }

    if (args.length() == 2 && args.first().toString() == u"show") {
        m_requestedAccount = args[1].toString();
    }

    qmlRegisterType<AccountsModel>("org.kde.konlineaccounts.kcm", 1, 0, "AccountsModel");
    qmlRegisterType<AvailableAccountsModel>("org.kde.konlineaccounts.kcm", 1, 0, "AvailableAccountsModel");
    qmlRegisterType<SuggestedAppsModel>("org.kde.konlineaccounts.kcm", 1, 0, "SuggestedAppsModel");
    qmlRegisterType<AuthorizedAppsModel>("org.kde.konlineaccounts.kcm", 1, 0, "AuthorizedAppsModel");

    setButtons(KQuickConfigModule::NoAdditionalButton);

    connect(
        this,
        &KQuickConfigModule::mainUiReady,
        this,
        [this] {
            Q_EMIT requestedTypesChanged();
            Q_EMIT requestedAccountChanged();
        },
        Qt::QueuedConnection);
}

QStringList AccountsSettings::requestedTypes() const
{
    return m_requestedTypes;
}

QString AccountsSettings::requestedAccount() const
{
    return m_requestedAccount;
}

AccountBuilder *AccountsSettings::createBuilder(const QString &provider)
{
    return new AccountBuilder(provider);
}

void AccountsSettings::finishSetup(QQuickItem *context, AccountBuilder *builder)
{
    const auto window = QQuickRenderControl::renderWindowFor(context->window());

    builder->apply();

    KWaylandExtras::xdgActivationToken(window, KWaylandExtras::lastInputSerial(window), QString()).then(this, [builder](const QString &token) {
        QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.private.KOnlineAccounts"_s,
                                                          u"/org/kde/KOnlineAccounts/private"_s,
                                                          u"org.kde.KOnlineAccounts.ManagerPrivate"_s,
                                                          u"sendAccountCreationFinished"_s);
        msg.setArguments({builder->accountId(), token});
        QDBusReply<void> reply = QDBusConnection::sessionBus().call(msg);

        if (!reply.isValid()) {
            qCWarning(LOG_KONLINEACCOUNTS_KCM) << "Failed to finish account setup" << reply.error().message();
        }
    });
}

QString AccountsSettings::accountName(const QString &accountId)
{
    if (accountId.isEmpty()) {
        return accountId;
    }

    QDBusMessage msg = QDBusMessage::createMethodCall(u"org.kde.KOnlineAccounts"_s,
                                                      u"/org/kde/KOnlineAccounts/Accounts/" + accountId,
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

bool AccountsSettings::serviceRunning() const
{
    return QDBusConnection::sessionBus().interface()->isServiceRegistered(u"org.kde.KOnlineAccounts"_s);
}

#include "kcm.moc"
