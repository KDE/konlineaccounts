/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfig.h"

#include "adaptors/imap.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(AutoconfigAccount, "metadata.json")

using namespace Qt::StringLiterals;

AutoconfigAccount::AutoconfigAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new IMAPAdaptor(this, config().group(u"IMAP"_s));
}

#include "autoconfig.moc"
