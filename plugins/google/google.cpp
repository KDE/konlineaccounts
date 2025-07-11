/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "google.h"

#include "googleinterface.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(GoogleAccount, "metadata.json")

using namespace Qt::StringLiterals;

GoogleAccount::GoogleAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new GoogleInterface(this, config().group(u"Google"_s));
}

#include "google.moc"
