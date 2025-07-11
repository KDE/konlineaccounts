/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nextcloud.h"

#include "caldav.h"
#include "carddav.h"
#include "nextcloudinterface.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(NextcloudAccount, "metadata.json")

using namespace Qt::StringLiterals;

NextcloudAccount::NextcloudAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new NextcloudInterface(this, config().group(u"Nextcloud"_s));
    new CalDAV(this, config().group(u"CalDAV"_s));
    new CardDAV(this, config().group(u"CardDAV"_s));
}

#include "nextcloud.moc"
