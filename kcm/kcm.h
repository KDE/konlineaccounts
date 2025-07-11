/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ACCOUNTSSETTINGS_H
#define ACCOUNTSSETTINGS_H

#include <QObject>
#include <QVariant>

#include <KQuickConfigModule>

#include "accountbuilder.h"

class AccountsSettings : public KQuickConfigModule
{
    Q_OBJECT
    Q_PROPERTY(QStringList requestedTypes READ requestedTypes NOTIFY requestedTypesChanged)

public:
    explicit AccountsSettings(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    QStringList requestedTypes() const;
    Q_SIGNAL void requestedTypesChanged();

    Q_INVOKABLE AccountBuilder *createBuilder(const QString &provider);

private:
    QStringList m_requestedTypes;
};

#endif // ACCOUNTSSETTINGS_H
