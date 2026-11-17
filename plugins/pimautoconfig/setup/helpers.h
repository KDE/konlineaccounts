/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KAccountAutoconfigServer>

#include <QString>

class Helpers
{
public:
    static QString authenticationTypeToVisibleString(const KAccountAutoconfigServer::AuthenticationType &authentication);
};
