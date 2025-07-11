/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kcm.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>

#include "accountsmodel.h"
#include "authorizedappsmodel.h"
#include "availableaccountsmodel.h"
#include "suggestedappsmodel.h"

using namespace Qt::Literals;

K_PLUGIN_CLASS_WITH_JSON(AccountsSettings, "kcm_konlineaccounts.json")

AccountsSettings::AccountsSettings(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KQuickConfigModule(parent, data)
{
    if (args.length() == 2 && args.first().toString() == u"add") {
        m_requestedTypes = args[1].toString().split(u';');
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
        },
        Qt::QueuedConnection);
}

QStringList AccountsSettings::requestedTypes() const
{
    return m_requestedTypes;
}

AccountBuilder *AccountsSettings::createBuilder(const QString &provider)
{
    return new AccountBuilder(provider);
}

#include "kcm.moc"
