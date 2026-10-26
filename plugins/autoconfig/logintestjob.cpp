/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestjob.h"

#include "setup_debug.h"

#include <qloggingcategory.h>

LoginTestJob::LoginTestJob(QObject *parent)
    : KCompositeJob(parent)
{
}

LoginTestJob::~LoginTestJob() = default;

void LoginTestJob::start()
{
    for (const auto job : subjobs()) {
        job->start();
    }
}

void LoginTestJob::setEmail(const QString &email)
{
    m_email = email;
}

void LoginTestJob::setPassword(const QString &password)
{
    m_password = password;
}

void LoginTestJob::slotResult(KJob *job)
{
    KCompositeJob::slotResult(job);
    if (!hasSubjobs()) {
        emitResult();
    }
}

IMAPLoginTest *LoginTestJob::mapAutoconfigServerToIMAP(const KAutoconfigServer &server)
{
    auto const imap = new IMAPLoginTest(this);
    imap->setHostname(server.url().host());
    imap->setPort(server.url().port());
    imap->setUsername(m_email);
    imap->setPassword(m_password);
    imap->setSocketType(server.socketType());
    imap->setAuthenticationType(server.authenticationType());

    connect(imap, &KJob::finished, this, [this, imap, server] {
        result.insert(QPair{server.url(), server.protocol()}, imap->error() == NoError);
    });

    return imap;
}

void LoginTestJob::addServer(const KAutoconfigServer &server)
{
    // TODO: make this not fail if a subjob fails
    switch (server.protocol()) {
    case KAutoconfigServer::IMAP:
        addSubjob(mapAutoconfigServerToIMAP(server));
        break;
    // TODO: add other protocols
    default:
        qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unsupported protocol" << server.protocol();
        break;
    }
}
