/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */


#pragma once
#include "logintestbase.h"

#include <QNetworkAccessManager>

class DAVLoginTestJob : public LoginTestBase
{
    Q_OBJECT
public:
    explicit DAVLoginTestJob(QObject *parent = nullptr);
    ~DAVLoginTestJob() override;

    void setUrl(const QUrl& url);

protected:
    void login() override;

private:
    QNetworkAccessManager *m_nam;
    QUrl m_url;
};

