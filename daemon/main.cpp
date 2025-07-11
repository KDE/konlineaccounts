/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QApplication>
#include <QDBusConnection>

#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>

#include "accountsmanager.h"

#include "accountsfiltermodel.h"
#include "accountsmodel.h"

#include <QQmlEngine>

using namespace Qt::Literals;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KAboutData about(u"konlineaccountsd"_s);
    KAboutData::setApplicationData(about);

    KCrash::initialize();

    app.setQuitLockEnabled(false);
    app.setQuitOnLastWindowClosed(false);

    // TODO service name?
    KDBusService service(KDBusService::Unique);

    KLocalizedString::setApplicationDomain("konlineaccountsd");

    qmlRegisterType<AccountsModel>("org.kde.konlineaccounts.kcm", 1, 0, "AccountsModel");
    qmlRegisterType<AccountsFilterModel>("org.kde.konlineaccounts.kcm", 1, 0, "AccountsFilterModel");

    auto manager = new AccountsManager;

    QDBusConnection::sessionBus().registerObject(u"/org/kde/KOnlineAccounts"_s,
                                                 manager,
                                                 QDBusConnection::ExportScriptableContents | QDBusConnection::ExportAdaptors);

    QDBusConnection::sessionBus().registerService(u"org.kde.KOnlineAccounts"_s);

    return app.exec();
}
