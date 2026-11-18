/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "logintestjobbase.h"

#include <KAccountAutoconfigServer>

#include <KIMAP/LoginJob>

class IMAPLoginTest : public LoginTestJobBase
{
    Q_OBJECT

public:
    explicit IMAPLoginTest(const KAccountAutoconfigServer &server, QObject *parent = nullptr);
    ~IMAPLoginTest() override;

protected:
    void login() override;
};