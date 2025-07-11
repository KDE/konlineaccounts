/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDBusAbstractAdaptor>
#include <QDBusContext>
#include <QDBusVariant>

class PropertiesAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.DBus.Properties")

public:
    PropertiesAdaptor(QObject *parent);

    Q_SCRIPTABLE QDBusVariant Get(const QString &interfaceName, const QString &propertyName);
};
