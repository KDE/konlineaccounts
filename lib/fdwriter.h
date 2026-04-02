/*
 *   SPDX-FileCopyrightText: 2026 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "accounts_export.h"

#include <QDBusUnixFileDescriptor>
#include <QString>

namespace FdWriter
{

ACCOUNTS_EXPORT std::optional<QDBusUnixFileDescriptor> write(const QByteArray &data);

};
