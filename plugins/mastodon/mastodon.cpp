/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mastodon.h"

#include "mastodoninterface.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(MastodonAccount, "metadata.json")

using namespace Qt::StringLiterals;

MastodonAccount::MastodonAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new MastodonInterface(this, config().group(u"Mastodon"_s));
}

#include "mastodon.moc"
