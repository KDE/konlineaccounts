/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QMetaObject>
#include <qqmlintegration.h>

struct KAccountAutoconfigServerForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(KAccountAutoconfigServer)
    QML_FOREIGN_NAMESPACE(KAccountAutoconfigServer)
};
