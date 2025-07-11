/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "caldav.h"

using namespace Qt::Literals;

CalDAV::CalDAV(QObject *parent, KConfigGroup config)
    : QDBusAbstractAdaptor(parent)
    , m_config(config)
{
}

QString CalDAV::url() const
{
    return m_config.readEntry("url", QString());
}

QString CalDAV::username() const
{
    return m_config.readEntry("username", QString());
}

QString CalDAV::password() const
{
    return m_config.readEntry("password", QString());
}
