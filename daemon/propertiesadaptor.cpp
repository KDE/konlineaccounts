/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "propertiesadaptor.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QMetaClassInfo>

using namespace Qt::StringLiterals;

PropertiesAdaptor::PropertiesAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
}

QDBusVariant PropertiesAdaptor::Get(const QString &interfaceName, const QString &propertyName)
{
    QDBusContext *ctx = dynamic_cast<QDBusContext *>(parent());

    int index = parent()->metaObject()->indexOfClassInfo("D-Bus Interface");

    if (index == -1) {
        ctx->sendErrorReply(QDBusError::InvalidArgs, u"No such interface"_s);
    }

    QMetaClassInfo info = parent()->metaObject()->classInfo(index);

    if (QString::fromUtf8(info.value()) != interfaceName) {
        ctx->sendErrorReply(QDBusError::InvalidArgs, u"No such interface"_s);
    }
    int pindex = parent()->metaObject()->indexOfProperty(qPrintable(propertyName));

    if (pindex == -1) {
        ctx->sendErrorReply(QDBusError::InvalidArgs, u"No such property"_s);
    }
    return QDBusVariant(parent()->property(qPrintable(propertyName)));
}
