/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "davlogintestjob.h"
#include "setup_debug.h"

#include <QNetworkReply>
#include <QAuthenticator>

DAVLoginTestJob::DAVLoginTestJob(QObject *parent)
    : LoginTestBase(parent)
    , m_nam(new QNetworkAccessManager())
{
    connect(m_nam, &QNetworkAccessManager::authenticationRequired, [](const QNetworkReply *reply, QAuthenticator *auth) {
        const QUrl url = reply->request().url();
        if (!url.userName().isEmpty()) {
            auth->setUser(url.userName());
            auth->setPassword(url.password());
        }
    });

}

DAVLoginTestJob::~DAVLoginTestJob() = default;

void DAVLoginTestJob::setUrl(const QUrl &url)
{
    m_url = url;
}

void DAVLoginTestJob::login()
{
    m_url.setUserName(m_username);
    m_url.setPassword(m_password);
    const QNetworkRequest request(m_url);
    const auto reply = m_nam->sendCustomRequest(request, "PROPFIND");
    reply->setParent(this);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        if (reply->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "DAV login failed:" << reply->errorString();
            setError(reply->error());
            setErrorText(reply->errorString());
        } else {
            qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "DAV login succeeded";
        }
        emitResult();
    });
}

