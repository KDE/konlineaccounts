/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "accountsmanagerprivate.h"

#include <QDebug>

using namespace Qt::Literals;

void AccountsManagerPrivate::sendAccountCreationFinished(const QString &id, const QString &xdgActivationToken)
{
    Q_EMIT accountCreationFinished(id, xdgActivationToken);
}

void AccountsManagerPrivate::removeAccount(const QString &id)
{
    Q_EMIT accountRemovalRequested(id);
}
