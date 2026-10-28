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
SMTPLoginTestJob *LoginTestJob::mapAutoconfigServerToSMTP(const KAutoconfigServer &server)
{
    const auto smtpJob = new SMTPLoginTestJob(this);
    smtpJob->setHostname(server.url().host());
    smtpJob->setPort(server.url().port());
    smtpJob->setUsername(m_email);
    smtpJob->setPassword(m_password);
    smtpJob->setSocketType(server.socketType());
    smtpJob->setAuthenticationType(server.authenticationType());

    connect(smtpJob, &KJob::finished, this, [this, smtpJob, server] {
        result.insert(QPair{server.url(), server.protocol()}, smtpJob->error() == NoError);
    });

    return smtpJob;
}

POPLoginTestJob *LoginTestJob::mapAutoconfigServerToPOP(const KAutoconfigServer &server)
{
    const auto popJob = new POPLoginTestJob(this);
    popJob->setHostname(server.url().host());
    popJob->setPort(server.url().port());
    popJob->setUsername(m_email);
    popJob->setPassword(m_password);
    popJob->setSocketType(server.socketType());
    popJob->setAuthenticationType(server.authenticationType());

    connect(popJob, &KJob::finished, this, [this, popJob, server] {
        result.insert(QPair{server.url(), server.protocol()}, popJob->error() == NoError);
    });

    return popJob;
}

void LoginTestJob::addServer(const KAutoconfigServer &server)
{
    switch (server.protocol()) {
    case KAutoconfigServer::SMTP:
        addSubjob(mapAutoconfigServerToSMTP(server));
        break;
    case KAutoconfigServer::POP3:
        addSubjob(mapAutoconfigServerToPOP(server));
        break;
    default:
        qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unsupported protocol" << server.protocol();
        break;
    }
}
