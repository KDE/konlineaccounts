/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "pimautoconfig.h"

#include "pimautoconfiginterface.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(PIMAutoconfigAccount, "metadata.json")

using namespace Qt::StringLiterals;

PIMAutoconfigAccount::PIMAutoconfigAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new PIMAutoconfigInterface(this, config().group(u"PIMAutoconfig"_s));
}

#include "pimautoconfig.moc"
