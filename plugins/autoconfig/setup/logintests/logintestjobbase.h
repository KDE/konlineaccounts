/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KAccountAutoconfigServer>

#include <KJob>

class LoginTestJobBase : public KJob
{
    Q_OBJECT

public:
    explicit LoginTestJobBase(const KAccountAutoconfigServer &server, QObject *parent = nullptr);
    ~LoginTestJobBase() override;
    void start() override;

    void setUsername(const QString &username);
    void setPassword(const QString &password);

    [[nodiscard]] KAccountAutoconfigServer server() const;

protected:
    virtual void login() = 0;

    KAccountAutoconfigServer m_server;

    QString m_username;
    QString m_password;
};