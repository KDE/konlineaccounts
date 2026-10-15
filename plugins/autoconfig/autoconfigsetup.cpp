/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfigsetup.h"

#include <QDBusReply>

#include "setup_debug.h"

using namespace Qt::Literals;

AutoconfigSetup::AutoconfigSetup(QObject *parent)
    : QObject(parent)
{
}

void AutoconfigSetup::registerAutoconfig(const QString &email)
{
    qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Registering autoconfig" << email;
}