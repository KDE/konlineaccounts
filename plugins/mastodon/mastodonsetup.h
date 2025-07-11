/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>
#include <QVariant>
#include <qqmlregistration.h>

#include <QCoro/QCoroCore>

#include "accountbuilder.h"

class MastodonSetup : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    explicit MastodonSetup(QObject *parent = nullptr);

    Q_INVOKABLE void registerMastodon(const QString &instanceUrl);

    AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);
    Q_SIGNAL void builderChanged();

private:
    QCoro::Task<void> doRegisterMastodon(const QString &instanceUrl);

    AccountBuilder *m_builder;
};
