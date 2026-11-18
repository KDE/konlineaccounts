/*
 *   SPDX-FileCopyrightText: 2026 Alan Thouvenin <ath@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "logintestjobbase.h"

#include <KAccountAutoconfigServer>

class SMTPLoginTestJob : public LoginTestJobBase
{
    Q_OBJECT
public:
    explicit SMTPLoginTestJob(const KAccountAutoconfigServer &server, QObject *parent = nullptr);
    ~SMTPLoginTestJob() override;

protected:
    void login() override;

private:
    void handleSmtpConnectionError(const QString &error);
};
