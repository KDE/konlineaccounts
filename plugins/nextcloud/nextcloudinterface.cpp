/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nextcloudinterface.h"

using namespace Qt::Literals;

NextcloudInterface::NextcloudInterface(QObject *parent, KConfigGroup config)
    : QDBusAbstractAdaptor(parent)
    , m_config(config)
{
}

QString NextcloudInterface::url() const
{
    return m_config.readEntry("url", QString());
}

QString NextcloudInterface::username() const
{
    return m_config.readEntry("username", QString());
}

QString NextcloudInterface::password() const
{
    return m_config.readEntry("password", QString());
}

QString NextcloudInterface::storagePath() const
{
    return m_config.readEntry("storagePath", QString());
}
