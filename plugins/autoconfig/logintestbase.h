/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KJob>

class LoginTestBase : public KJob
{
    Q_OBJECT

public:
    explicit LoginTestBase(QObject *parent = nullptr);
    ~LoginTestBase() override;
    void start() override;

    void setUsername(const QString &username);
    void setPassword(const QString &password);

protected:
    virtual void login() = 0;

    QString m_username;
    QString m_password;
};