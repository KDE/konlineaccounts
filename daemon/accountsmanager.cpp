/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accountsmanager.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <KConfig>
#include <KConfigGroup>
#include <KIO/CommandLauncherJob>
#include <KLocalizedQmlContext>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <KService>
#include <KSharedConfig>
#include <KWaylandExtras>

#include "accessmanager.h"
#include "account.h"
#include "propertiesadaptor.h"

using namespace Qt::Literals;

AccountsManager::AccountsManager()
    : QObject()
{
    m_private = std::make_unique<AccountsManagerPrivate>();

    QDBusConnection otherSessionBus = QDBusConnection::connectToBus(QDBusConnection::SessionBus, u"other"_s);
    otherSessionBus.registerObject(u"/org/kde/KOnlineAccounts/private"_s, m_private.get(), QDBusConnection::ExportScriptableContents);
    otherSessionBus.registerService(u"org.kde.private.KOnlineAccounts"_s);

    auto config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);

    KConfigGroup accountsGroup = config->group(u"Accounts"_s);
    const QStringList accounts = accountsGroup.readEntry("accounts", QStringList());

    for (const QString &id : accounts) {
        auto group = accountsGroup.group(id);
        KPluginMetaData md(u"konlineaccounts/providers/" + group.readEntry("provider", QString()));
        auto result = KPluginFactory::instantiatePlugin<Account>(md, nullptr, {id, QVariant::fromValue(this)});

        // TODO handle error

        auto account = std::unique_ptr<Account>(result.plugin);

        QDBusConnection::sessionBus().registerObject(u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                     account.get(),
                                                     QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);

        m_accounts[id] = std::move(account);
    }

    connect(m_private.get(), &AccountsManagerPrivate::accountCreationFinished, this, &AccountsManager::slotAccountCreationFinished);
    connect(m_private.get(), &AccountsManagerPrivate::accountRemovalRequested, this, &AccountsManager::slotRemoveAccount);

    new PropertiesAdaptor(this);
}

void AccountsManager::slotAccountCreationFinished(const QString &id, const QString &xdgActivationToken)
{
    auto config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);
    config->reparseConfiguration();
    KConfigGroup accountsGroup = config->group(u"Accounts"_s);
    auto group = accountsGroup.group(id);
    KPluginMetaData md(u"konlineaccounts/providers/" + group.readEntry("provider", QString()));
    auto result = KPluginFactory::instantiatePlugin<Account>(md, nullptr, {id, QVariant::fromValue(this)});
    auto account = std::unique_ptr<Account>(result.plugin);
    QDBusConnection::sessionBus().registerObject(u"/org/kde/KOnlineAccounts/Accounts/" + id,
                                                 account.get(),
                                                 QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);

    if (!m_requester.isEmpty()) {
        const QString appId = m_requester;
        m_requester = QString();

        account->grantAccess(appId);

        // TODO emit targeted signal
        Q_EMIT accountAccessGranted(QDBusObjectPath(u"/org/kde/KOnlineAccounts/Accounts/" + id), xdgActivationToken);
    }

    m_accounts[id] = std::move(account);
}

QList<QDBusObjectPath> AccountsManager::accounts() const
{
    QList<QDBusObjectPath> result;
    result.reserve(m_accounts.size());

    auto config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);
    config->reparseConfiguration();
    KConfigGroup accountsGroup = config->group(u"Accounts"_s);

    for (auto &[id, account] : m_accounts) {
        auto group = accountsGroup.group(id);

        if (account->hasAccess(callerId())) {
            result << QDBusObjectPath(u"/org/kde/KOnlineAccounts/Accounts/" + id);
        } else {
            qWarning() << callerId() << "is not allowed to read this account";
        }
    }

    return result;
}

void AccountsManager::requestAccount(const QStringList &types)
{
    QQmlApplicationEngine *engine = new QQmlApplicationEngine;

    KLocalization::setupLocalizedContext(engine);

    const QString appId = callerId();

    m_requester = appId; // TODO make something more robust

    const KService::Ptr app = KService::serviceByDesktopName(appId);

    // TODO null check

    engine->rootContext()->setContextProperty(u"_manager"_s, this);
    engine->setInitialProperties({
        {u"manager"_s, QVariant::fromValue(this)},
        {u"applicationName"_s, app ? app->name() : u"Foo"_s},
        {u"types"_s, types},
        {u"callerId"_s, appId},
    });
    engine->loadFromModule("org.kde.konlineaccounts.daemon", "Main");

    m_window = qobject_cast<QWindow *>(engine->rootObjects()[0]);
}

void AccountsManager::requestAccountCreation(const QStringList &type)
{
    auto job = new KIO::CommandLauncherJob(u"systemsettings"_s, {u"kcm_konlineaccounts"_s, u"--args"_s, u"add " + type.join(u';')});
    job->setDesktopName(u"systemsettings"_s);
    job->start();
}

void AccountsManager::registerApp(const QString &appId)
{
    auto creds = QDBusConnection::sessionBus().interface()->serviceCredentials(message().service()).value();

    // TODO use processFD?
    int pid = creds.value(u"ProcessID"_s).toInt();

    QString flatpakInfo = u"/proc/" + QString::number(pid) + u"/root/.flatpak-info";

    QString realAppId = appId;

    if (QFileInfo::exists(flatpakInfo)) {
        // Ideally this would use KConfig to parse the ini file, but KConfig fails to
        // open files in /proc/<>/root

        QFile file(flatpakInfo);
        bool ok = file.open(QIODevice::ReadOnly);
        Q_ASSERT(ok);

        while (!file.atEnd()) {
            const QByteArray line = file.readLine().trimmed();

            if (line.startsWith("name=")) {
                realAppId = QString::fromUtf8(line.mid(line.indexOf("=") + 1));
                break;
            }
        }
    }

    AccessManager::instance().registerApp(message().service(), realAppId);
}

void AccountsManager::accountSelected(const QString &callerId, const QString &id)
{
    m_accounts[id]->grantAccess(callerId);
    m_accounts[id]->config().sync();

    KWaylandExtras::requestXdgActivationToken(m_window, KWaylandExtras::lastInputSerial(m_window), QString());

    connect(
        KWaylandExtras::self(),
        &KWaylandExtras::xdgActivationTokenArrived,
        this,
        [this, id](int /*serial*/, const QString &token) {
            // TODO emit targeted signal
            Q_EMIT accountAccessGranted(QDBusObjectPath(u"/org/kde/KOnlineAccounts/Accounts/" + id), token);
        },
        Qt::SingleShotConnection);

    m_window->close();
}

QString AccountsManager::callerId() const
{
    Q_ASSERT(calledFromDBus());
    return AccessManager::instance().appIdForService(message().service());
}

QList<Account *> AccountsManager::accountsInternal() const
{
    QList<Account *> l;

    for (auto &[key, value] : m_accounts) {
        l << value.get();
    }

    return l;
}

void AccountsManager::slotRemoveAccount(const QString &id)
{
    Q_EMIT accountRemoved(QDBusObjectPath(u"/org/kde/KOnlineAccounts/Accounts/" + id));

    m_accounts.erase(id);

    auto config = KSharedConfig::openConfig(u"konlineaccountsrc"_s);
    config->reparseConfiguration();
    KConfigGroup accountsGroup = config->group(u"Accounts"_s);
    QStringList accounts = accountsGroup.readEntry("accounts", QStringList());
    accounts.removeAll(id);
    accountsGroup.writeEntry("accounts", accounts, KConfig::Notify);
    accountsGroup.deleteGroup(id);
    config->sync();
}
