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
    if (const auto loginTestJob = dynamic_cast<LoginTestJobBase *>(job)) {
        result.emplace_back(QPair{loginTestJob->server(), QPair{loginTestJob->error() == NoError, loginTestJob->errorString()}});
    }

    removeSubjob(job);
    if (!hasSubjobs()) {
        emitResult();
    }
}

IMAPLoginTest *LoginTestJob::mapAutoconfigServerToIMAP(const KAccountAutoconfigServer &server)
{
    auto const imap = new IMAPLoginTest(server, this);
    imap->setUsername(m_email);
    imap->setPassword(m_password);

    return imap;
}

SMTPLoginTestJob *LoginTestJob::mapAutoconfigServerToSMTP(const KAccountAutoconfigServer &server)
{
    const auto smtp = new SMTPLoginTestJob(server, this);
    smtp->setUsername(m_email);
    smtp->setPassword(m_password);

    return smtp;
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
        qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unsupported protocol" << server.protocol();
        break;
    }
}
