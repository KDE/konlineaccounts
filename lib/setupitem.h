/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QQuickItem>
#include <qqmlregistration.h>

#include "accountbuilder.h"

class SetupItem : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);
    Q_SIGNAL void builderChanged();

    // Q_SIGNAL void finished();

private:
    AccountBuilder *m_builder;
};
