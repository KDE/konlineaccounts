/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfig.h"

#include "adaptors/smtp.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(AutoconfigAccount, "metadata.json")

using namespace Qt::StringLiterals;

AutoconfigAccount::AutoconfigAccount(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Account(parent, md, args)
{
    new SMTPAdaptor(this, config().group(u"SMTP"_s));
}

#include "autoconfig.moc"
