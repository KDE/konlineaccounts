/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfigsetup.h"

#include <QDBusConnection>
#include <QDBusReply>

#include "setup_debug.h"

using namespace Qt::Literals;

AutoconfigSetup::AutoconfigSetup(QObject *parent)
    : QObject(parent)
{
}

void AutoconfigSetup::registerAutoconfig(const QString &email)
{
    qWarning() << "Registering autoconfig" << email;
}