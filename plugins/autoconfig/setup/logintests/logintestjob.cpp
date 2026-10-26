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

void LoginTestJob::addServer(const KAccountAutoconfigServer &server)
{
    // TODO: make this not fail if a subjob fails
    switch (server.protocol()) {
    // TODO: add other protocols
    default:
        qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unsupported protocol" << server.protocol();
        break;
    }
}
