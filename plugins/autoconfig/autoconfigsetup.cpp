/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfigsetup.h"

#include <QDBusReply>

#include <KAccountAutoconfig/KAccountAutoconfigJob>

#include "setup_debug.h"

#include <KConfigGroup>
#include <qmetaobject.h>
#include <qt6keychain/keychain.h>

using namespace Qt::Literals;

namespace
{
std::optional<KAutoconfigServer::Protocol> parseProtocol(const QString &protocol_str)
{
    switch (const QStringList protocols = {u"IMAP"_s, u"POP3"_s, u"SMTP"_s, u"CalDAV"_s, u"CardDAV"_s}; protocols.indexOf(protocol_str)) {
    case 0:
        return KAutoconfigServer::Protocol::IMAP;
    case 1:
        return KAutoconfigServer::Protocol::POP3;
    case 2:
        return KAutoconfigServer::Protocol::SMTP;
    case 3:
        return KAutoconfigServer::Protocol::CalDAV;
    case 4:
        return KAutoconfigServer::Protocol::CardDAV;
    default:
        qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Unknown protocol" << protocol_str;
        return {};
    }
}
}

AutoconfigSetup::AutoconfigSetup(QObject *parent)
    : QObject(parent)
    , m_builder(nullptr)
{
    registerAutoconfig(u"klo@enioka.com"_s);
}

void AutoconfigSetup::registerAutoconfig(const QString &email)
{
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Registering autoconfig" << email;
    m_email = email;
    const auto job = new KAccountAutoconfigJob(this);
    job->setEmail(email);
    job->start();

    connect(job, &KJob::result, this, [this, email, job]() {
        if (job->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Job failed" << job->errorString();
            // Q_EMIT autoconfigJobFailed();
            m_builder->fail(u"Could not find configuration for the given email address"_s);
        } else {
            handleReply(job->servers());
        }

        job->deleteLater();
    });
}

void AutoconfigSetup::setProtocolEnabled(const QString &protocol_str, bool enabled)
{
    const auto protocol = parseProtocol(protocol_str);
    Q_ASSERT(protocol.has_value());
    qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Disabling" << protocol;

    if (enabled) {
        m_enabledProtocols.insert(protocol.value());
    } else {
        m_enabledProtocols.remove(protocol.value());
    }
}

void AutoconfigSetup::setUrlSelectedForProtocol(const QString &protocol_str, const QString &url)
{
    const auto protocol = parseProtocol(protocol_str);
    Q_ASSERT(protocol.has_value());
    qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Selected" << protocol << "server with url" << url;

    m_selectedUrlPerProtocol.insert(protocol.value(), url);
}

void AutoconfigSetup::handleReply(const QList<KAutoconfigServer> &servers)
{
    qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Found servers:" << servers;
    m_servers = servers;

    populateSelectedUrlMap();

    m_state = ServerSelection;
    Q_EMIT stateChanged();
}

void AutoconfigSetup::populateSelectedUrlMap()
{
    m_enabledProtocols.clear();
    m_selectedUrlPerProtocol.clear();

    if (!getImapServers().empty()) {
        m_enabledProtocols.insert(KAutoconfigServer::Protocol::IMAP);
        m_selectedUrlPerProtocol.insert(KAutoconfigServer::Protocol::IMAP, getImapServers()[0]);
    }
    if (!getPop3Servers().empty()) {
        m_enabledProtocols.insert(KAutoconfigServer::Protocol::POP3);
        m_selectedUrlPerProtocol.insert(KAutoconfigServer::Protocol::POP3, getPop3Servers()[0]);
    }
    if (!getSmtpServers().empty()) {
        m_enabledProtocols.insert(KAutoconfigServer::Protocol::SMTP);
        m_selectedUrlPerProtocol.insert(KAutoconfigServer::Protocol::SMTP, getSmtpServers()[0]);
    }
    if (!getCalDavServers().empty()) {
        m_enabledProtocols.insert(KAutoconfigServer::Protocol::CalDAV);
        m_selectedUrlPerProtocol.insert(KAutoconfigServer::Protocol::CalDAV, getCalDavServers()[0]);
    }
    if (!getCardDavServers().empty()) {
        m_enabledProtocols.insert(KAutoconfigServer::Protocol::CardDAV);
        m_selectedUrlPerProtocol.insert(KAutoconfigServer::Protocol::CardDAV, getCardDavServers()[0]);
    }
}

void AutoconfigSetup::dumpServersToConfig()
{
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Dumping selected servers to config" << m_selectedUrlPerProtocol << "to" << m_builder->config().name();

    if (m_enabledProtocols.contains(KAutoconfigServer::Protocol::IMAP)) {
        auto serverConfig = m_builder->config().group(u"IMAP"_s);
        const QUrl serverUrl{m_selectedUrlPerProtocol[KAutoconfigServer::Protocol::IMAP]};
        serverConfig.writeEntry(u"hostname"_s, serverUrl.host());
        serverConfig.writeEntry(u"port"_s, serverUrl.port());
        serverConfig.writeEntry(u"username"_s, m_email);
        serverConfig.writeEntry(u"socket_type"_s, u"TODO"_s);
        serverConfig.writeEntry(u"authentication_type"_s, u"TODO"_s);
    }
    if (m_enabledProtocols.contains(KAutoconfigServer::Protocol::POP3)) {
        auto serverConfig = m_builder->config().group(u"POP3"_s);
        const QUrl serverUrl{m_selectedUrlPerProtocol[KAutoconfigServer::Protocol::POP3]};
        serverConfig.writeEntry(u"hostname"_s, serverUrl.host());
        serverConfig.writeEntry(u"port"_s, serverUrl.port());
        serverConfig.writeEntry(u"username"_s, m_email);
        serverConfig.writeEntry(u"socket_type"_s, u"TODO"_s);
        serverConfig.writeEntry(u"authentication_type"_s, u"TODO"_s);
    }
    if (m_enabledProtocols.contains(KAutoconfigServer::Protocol::SMTP)) {
        auto serverConfig = m_builder->config().group(u"SMTP"_s);
        const QUrl serverUrl{m_selectedUrlPerProtocol[KAutoconfigServer::Protocol::SMTP]};
        serverConfig.writeEntry(u"hostname"_s, serverUrl.host());
        serverConfig.writeEntry(u"port"_s, serverUrl.port());
        serverConfig.writeEntry(u"username"_s, m_email);
        serverConfig.writeEntry(u"socket_type"_s, u"TODO"_s);
        serverConfig.writeEntry(u"authentication_type"_s, u"TODO"_s);
    }
    if (m_enabledProtocols.contains(KAutoconfigServer::Protocol::CalDAV)) {
        auto calDav = m_builder->config().group(u"CalDAV"_s);
        auto serverConfig = m_builder->config().group(u"SMTP"_s);
        serverConfig.writeEntry(u"url"_s, m_selectedUrlPerProtocol[KAutoconfigServer::Protocol::SMTP]);
        serverConfig.writeEntry(u"username"_s, m_email);
    }
    if (m_enabledProtocols.contains(KAutoconfigServer::Protocol::CardDAV)) {
        auto cardDav = m_builder->config().group(u"CardDAV"_s);
    }
    // auto job = new QKeychain::WritePasswordJob(u"konlineaccounts"_s);
    m_builder->finish();
}

AccountBuilder *AutoconfigSetup::builder() const
{
    return m_builder;
}

void AutoconfigSetup::setBuilder(AccountBuilder *builder)
{
    m_builder = builder;
    Q_EMIT builderChanged();
}

QString AutoconfigSetup::getEmail()
{
    return m_email;
}

QStringList AutoconfigSetup::getUrlForServersWithProtocol(KAutoconfigServer::Protocol protocol)
{
    auto result = m_servers | std::views::filter([protocol](const KAutoconfigServer &server) {
                      return server.protocol() == protocol;
                  })
        | std::views::transform([](const KAutoconfigServer &server) {
                      return server.url().toDisplayString();
                  });

    QStringList serverUrls;
    std::ranges::copy(result, std::back_inserter(serverUrls));
    return serverUrls;
}

QStringList AutoconfigSetup::getImapServers()
{
    return getUrlForServersWithProtocol(KAutoconfigServer::IMAP);
}

QStringList AutoconfigSetup::getPop3Servers()
{
    return getUrlForServersWithProtocol(KAutoconfigServer::POP3);
}

QStringList AutoconfigSetup::getSmtpServers()
{
    return getUrlForServersWithProtocol(KAutoconfigServer::SMTP);
}

QStringList AutoconfigSetup::getCalDavServers()
{
    return getUrlForServersWithProtocol(KAutoconfigServer::CalDAV);
}

QStringList AutoconfigSetup::getCardDavServers()
{
    return getUrlForServersWithProtocol(KAutoconfigServer::CardDAV);
}
