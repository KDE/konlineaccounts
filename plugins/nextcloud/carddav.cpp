/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "carddav.h"

using namespace Qt::Literals;

CardDAV::CardDAV(QObject *parent, KConfigGroup config)
    : QDBusAbstractAdaptor(parent)
    , m_config(config)
{
}

QString CardDAV::url() const
{
    return m_config.readEntry("url", QString());
}

QString CardDAV::username() const
{
    return m_config.readEntry("username", QString());
}

QString CardDAV::password() const
{
    return m_config.readEntry("password", QString());
}
