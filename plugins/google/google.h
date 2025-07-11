/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "account.h"

class GoogleAccount : public Account
{
    Q_OBJECT
public:
    GoogleAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args);
};
