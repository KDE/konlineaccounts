/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestjob.h"

#include "setup_debug.h"
#include <KAccountAutoconfigServer>

#include <qloggingcategory.h>

LoginTestJob::LoginTestJob(QObject *parent)
    : KCompositeJob(parent)
{
}

LoginTestJob::~LoginTestJob() = default;

void LoginTestJob::start()
{
    if (!hasSubjobs()) {
        emitResult();
        return;
    }

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

IMAPLoginTest *LoginTestJob::mapAutoconfigServerToIMAP(const KAccountAutoconfigServer &server)
{
    auto const imap = new IMAPLoginTest(this);
    imap->setHostname(server.url().host());
    imap->setPort(server.url().port());
    imap->setUsername(m_email);
    imap->setPassword(m_password);
    imap->setSocketType(server.socketType());
    imap->setAuthenticationType(server.authenticationType());

    connect(imap, &KJob::finished, this, [this, imap, server] {
        result.emplace_back(QPair{server, QPair{imap->error() == NoError, imap->errorString()}});
    });

    return imap;
}

SMTPLoginTestJob *LoginTestJob::mapAutoconfigServerToSMTP(const KAccountAutoconfigServer &server)
{
    const auto smtpJob = new SMTPLoginTestJob(this);
    smtpJob->setHostname(server.url().host());
    smtpJob->setPort(server.url().port());
    smtpJob->setUsername(m_email);
    smtpJob->setPassword(m_password);
    smtpJob->setSocketType(server.socketType());
    smtpJob->setAuthenticationType(server.authenticationType());

    connect(smtpJob, &KJob::finished, this, [this, smtpJob, server] {
        result.emplace_back(QPair{server, QPair{smtpJob->error() == NoError, smtpJob->errorString()}});
    });

    return smtpJob;
}

void LoginTestJob::addServer(const KAccountAutoconfigServer &server)
{
    switch (server.protocol()) {
    case KAccountAutoconfigServer::IMAP:
        addSubjob(mapAutoconfigServerToIMAP(server));
        break;
    case KAccountAutoconfigServer::SMTP:
        addSubjob(mapAutoconfigServerToSMTP(server));
        break;
    // TODO: add other protocols
    default:
        qCInfo(LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP) << "Unsupported protocol" << server.protocol();
        break;
    }
}
