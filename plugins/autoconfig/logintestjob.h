/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "imaplogintest.h"
#include "kautoconfigserver.h"

#include <KCompositeJob>

class LoginTestJob : public KCompositeJob
{
    Q_OBJECT

public:
    explicit LoginTestJob(QObject *parent);
    ~LoginTestJob() override;

    void start() override;

    void setEmail(const QString &email);
    void setPassword(const QString &password);

    void addServer(const KAutoconfigServer &server);

    QHash<QPair<QUrl, KAutoconfigServer::Protocol>, bool> result;

protected:
    Q_SLOT void slotResult(KJob *job) override;
    IMAPLoginTest *mapAutoconfigServerToIMAP(const KAutoconfigServer &server);

private:
    QString m_email;
    QString m_password;
};