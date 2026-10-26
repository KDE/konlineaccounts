/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestbase.h"

LoginTestBase::LoginTestBase(QObject *parent)
    : KJob(parent)
{
}

LoginTestBase::~LoginTestBase() = default;

void LoginTestBase::start()
{
    QMetaObject::invokeMethod(this, &LoginTestBase::login, Qt::QueuedConnection);
}

void LoginTestBase::setUsername(const QString &username)
{
    m_username = username;
}

void LoginTestBase::setPassword(const QString &password)
{
    m_password = password;
}
