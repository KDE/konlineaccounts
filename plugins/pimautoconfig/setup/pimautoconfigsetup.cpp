/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "pimautoconfigsetup.h"

#include <QDBusReply>

#include "setup_debug.h"

using namespace Qt::Literals;

PIMAutoconfigSetup::PIMAutoconfigSetup(QObject *parent)
    : QObject(parent)
{
}

void PIMAutoconfigSetup::registerAutoconfig(const QString &email)
{
    qCInfo(LOG_KONLINEACCOUNTS_PIMAUTOCONFIG_SETUP) << "Registering autoconfig" << email;
}
