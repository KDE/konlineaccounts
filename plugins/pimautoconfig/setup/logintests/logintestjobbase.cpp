/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "logintestjobbase.h"

LoginTestJobBase::LoginTestJobBase(QObject *parent)
    : KJob(parent)
{
}

LoginTestJobBase::~LoginTestJobBase() = default;

void LoginTestJobBase::start()
{
    QMetaObject::invokeMethod(this, &LoginTestJobBase::login, Qt::QueuedConnection);
}

void LoginTestJobBase::setUsername(const QString &username)
{
    m_username = username;
}

void LoginTestJobBase::setPassword(const QString &password)
{
    m_password = password;
}
