/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "googlesetup.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDebug>

#include <KConfigGroup>
#include <KSharedConfig>

using namespace Qt::Literals;

QList<QUrl> googleScopes()
{
    // TODO: determine it based on what user wants?
    return {
        KGAPI2::Account::accountInfoScopeUrl(),
        KGAPI2::Account::calendarScopeUrl(),
        KGAPI2::Account::calendarEventsScopeUrl(),
        KGAPI2::Account::peopleScopeUrl(),
        KGAPI2::Account::tasksScopeUrl(),
    };
};

QString clientId()
{
    return QStringLiteral("554041944266.apps.googleusercontent.com");
}

QString clientSecret()
{
    return QStringLiteral("mdT1DjzohxN3npUUzkENT0gO");
}

GoogleSetup::GoogleSetup(QObject *parent)
    : QObject(parent)
{
    m_account = KGAPI2::AccountPtr(new KGAPI2::Account());
    const QList<QUrl> resourceScopes = googleScopes();
    for (const QUrl &scope : resourceScopes) {
        if (!m_account->scopes().contains(scope)) {
            m_account->addScope(scope);
        }
    }
    auto authJob = new KGAPI2::AuthJob(m_account, clientId(), clientSecret());
    authJob->setUsername(QString());
    connect(authJob, &KGAPI2::AuthJob::finished, this, &GoogleSetup::slotAuthJobFinished);
}

void GoogleSetup::slotAuthJobFinished(KGAPI2::Job *job)
{
    auto authJob = qobject_cast<KGAPI2::AuthJob *>(job);
    m_account = authJob->account();

    if (authJob->error() != KGAPI2::NoError) {
        qWarning() << "error!" << authJob->error() << authJob->errorString();
        m_builder->fail(authJob->errorString());
        return;
    }

    auto googleGroup = m_builder->config().group(u"Google"_s);
    googleGroup.writeEntry("clientId", clientId());
    googleGroup.writeEntry("clientSecret", clientSecret());
    googleGroup.writeEntry("accessToken", m_account->accessToken());
    googleGroup.writeEntry("refreshToken", m_account->refreshToken());
    googleGroup.writeEntry("scopes", m_account->scopes());

    m_builder->finish();

    Q_EMIT finished();
}

AccountBuilder *GoogleSetup::builder() const
{
    return m_builder;
}

void GoogleSetup::setBuilder(AccountBuilder *builder)
{
    m_builder = builder;
    Q_EMIT builderChanged();
}
