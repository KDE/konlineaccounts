/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "account.h"

class AutoconfigAccount : public Account
{
    Q_OBJECT
public:
    AutoconfigAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args);
};
