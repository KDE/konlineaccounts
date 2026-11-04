/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "autoconfigsetup.h"

#include "autoconfighelper.h"

#include <QDBusReply>

#include <KAccountAutoconfigJob>

#include "setup_debug.h"

#include <KConfigGroup>
#include <qt6keychain/keychain.h>

using namespace Qt::Literals;

AutoconfigSetup::AutoconfigSetup(QObject *parent)
    : QObject(parent)
    , m_builder(nullptr)
    , m_imapServers(new ServerListModel(this))
    , m_pop3Servers(new ServerListModel(this))
    , m_smtpServers(new ServerListModel(this))
    , m_calDavServers(new ServerListModel(this))
    , m_cardDavServers(new ServerListModel(this))
{
}

void AutoconfigSetup::registerAutoconfig(const QString &email, const QString &password)
{
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Registering autoconfig" << email;
    m_email = email;
    m_password = password;
    const auto job = new KAccountAutoconfigJob(this);
    job->setEmail(email);
    job->start();

    connect(job, &KJob::result, this, [this, job] {
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

void AutoconfigSetup::setProtocolEnabled(const QString &protocolStr, bool enabled)
{
    const auto protocol = AutoconfigHelper::protocolFromString(protocolStr);
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << (enabled ? "Enabling" : "Disabling") << protocol;

    if (enabled) {
        m_enabledProtocols.insert(protocol);
    } else {
        m_enabledProtocols.remove(protocol);
    }
}

void AutoconfigSetup::setUrlSelectedForProtocol(const QString &protocol_str, const QUrl &url)
{
    const auto protocol = AutoconfigHelper::protocolFromString(protocol_str);
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Selected" << protocol << "server with url" << url;

    m_selectedUrlPerProtocol.insert(protocol, url);
}

void AutoconfigSetup::handleReply(const QList<KAccountAutoconfigServer> &servers)
{
    qCInfo(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Found servers:" << servers;
    m_servers = servers;
    populateServersByProtocol();

    populateSelectedUrlMap();

    m_state = ServerSelection;
    Q_EMIT stateChanged();
}

void AutoconfigSetup::populateServersByProtocol()
{
    // TODO: better notifications than one each time
    m_imapServers->setServers(getServersWithProtocol(KAccountAutoconfigServer::IMAP));
    Q_EMIT imapServersChanged();
    m_pop3Servers->setServers(getServersWithProtocol(KAccountAutoconfigServer::POP3));
    Q_EMIT pop3ServersChanged();
    m_smtpServers->setServers(getServersWithProtocol(KAccountAutoconfigServer::SMTP));
    Q_EMIT smtpServersChanged();
    m_calDavServers->setServers(getServersWithProtocol(KAccountAutoconfigServer::CalDAV));
    Q_EMIT calDavServersChanged();
    m_cardDavServers->setServers(getServersWithProtocol(KAccountAutoconfigServer::CardDAV));
    Q_EMIT cardDavServersChanged();
}

void AutoconfigSetup::populateSelectedUrlMap()
{
    m_enabledProtocols.clear();
    m_selectedUrlPerProtocol.clear();

    if (!m_imapServers->isEmpty()) {
        m_enabledProtocols.insert(KAccountAutoconfigServer::Protocol::IMAP);
        m_selectedUrlPerProtocol.insert(KAccountAutoconfigServer::Protocol::IMAP, m_imapServers->first().url());
    }
    if (!m_pop3Servers->isEmpty()) {
        m_enabledProtocols.insert(KAccountAutoconfigServer::Protocol::POP3);
        m_selectedUrlPerProtocol.insert(KAccountAutoconfigServer::Protocol::POP3, m_pop3Servers->first().url());
    }
    if (!m_smtpServers->isEmpty()) {
        m_enabledProtocols.insert(KAccountAutoconfigServer::Protocol::SMTP);
        m_selectedUrlPerProtocol.insert(KAccountAutoconfigServer::Protocol::SMTP, m_smtpServers->first().url());
    }
    if (!m_calDavServers->isEmpty()) {
        m_enabledProtocols.insert(KAccountAutoconfigServer::Protocol::CalDAV);
        m_selectedUrlPerProtocol.insert(KAccountAutoconfigServer::Protocol::CalDAV, m_calDavServers->first().url());
    }
    if (!m_cardDavServers->isEmpty()) {
        m_enabledProtocols.insert(KAccountAutoconfigServer::Protocol::CardDAV);
        m_selectedUrlPerProtocol.insert(KAccountAutoconfigServer::Protocol::CardDAV, m_cardDavServers->first().url());
    }
}

template<typename F>
void AutoconfigSetup::dumpServersWithProtocolToConfig(const KAccountAutoconfigServer::Protocol &protocol, F &&callback)
{
    if (!m_enabledProtocols.contains(protocol)) {
        return;
    }
    auto serverConfig = m_builder->config().group(AutoconfigHelper::protocolToString(protocol));
    const auto server = findServerFromUrlAndProtocol(m_selectedUrlPerProtocol[protocol], protocol);
    callback(server, serverConfig);
}

void AutoconfigSetup::dumpServersToConfig()
{
    qCDebug(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP) << "Dumping selected servers to config" << m_selectedUrlPerProtocol << "to" << m_builder->config().name();

    const auto dumpMailServer = [this](const KAccountAutoconfigServer &server, KConfigGroup serverConfig) {
        serverConfig.writeEntry(u"hostname"_s, server.url().host());
        serverConfig.writeEntry(u"port"_s, server.url().port());
        serverConfig.writeEntry(u"socket_type"_s, AutoconfigHelper::socketTypeToString(server.socketType()));
        serverConfig.writeEntry(u"authentication_type"_s, AutoconfigHelper::authenticationTypeToString(server.authenticationType()));
        serverConfig.writeEntry(u"username"_s, m_email);
    };

    const auto dumpDavServer = [this](const KAccountAutoconfigServer &server, KConfigGroup serverConfig) {
        serverConfig.writeEntry(u"url"_s, server.url());
        serverConfig.writeEntry(u"username"_s, m_email);
    };

    dumpServersWithProtocolToConfig(KAccountAutoconfigServer::IMAP, dumpMailServer);
    dumpServersWithProtocolToConfig(KAccountAutoconfigServer::POP3, dumpMailServer);
    dumpServersWithProtocolToConfig(KAccountAutoconfigServer::SMTP, dumpMailServer);
    dumpServersWithProtocolToConfig(KAccountAutoconfigServer::CalDAV, dumpDavServer);
    dumpServersWithProtocolToConfig(KAccountAutoconfigServer::CardDAV, dumpDavServer);

    auto job = new QKeychain::WritePasswordJob(u"konlineaccounts"_s);
    job->setKey(u"account/" + m_builder->accountId() + u"/autoconfig/password");
    job->setTextData(m_password);

    connect(job, &QKeychain::WritePasswordJob::finished, this, [this, job] {
        if (job->error()) {
            qCWarning(LOG_KONLINEACCOUNTS_AUTOCONFIG_SETUP)
                << "Failed to write password for Autoconfig account" << m_builder->accountId() << job->errorString();
            m_builder->fail(job->errorString());
            return;
        }
        m_builder->finish();
    });
    job->start();
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

QList<KAccountAutoconfigServer> AutoconfigSetup::getServersWithProtocol(KAccountAutoconfigServer::Protocol protocol)
{
    auto result = m_servers | std::views::filter([protocol](const KAccountAutoconfigServer &server) {
                      return server.protocol() == protocol;
                  });

    QList<KAccountAutoconfigServer> servers;
    std::ranges::copy(result, std::back_inserter(servers));
    return servers;
}

KAccountAutoconfigServer AutoconfigSetup::findServerFromUrlAndProtocol(const QUrl &url, KAccountAutoconfigServer::Protocol protocol)
{
    auto result = m_servers | std::views::filter([protocol, url](const KAccountAutoconfigServer &server) {
                      return server.protocol() == protocol && server.url() == url;
                  });
    QList<KAccountAutoconfigServer> servers;
    std::ranges::copy(result, std::back_inserter(servers));
    Q_ASSERT(servers.count() == 1);
    return servers.first();
}

ServerListModel *AutoconfigSetup::getImapServers()
{
    return m_imapServers;
}

ServerListModel *AutoconfigSetup::getPop3Servers()
{
    return m_pop3Servers;
}

ServerListModel *AutoconfigSetup::getSmtpServers()
{
    return m_smtpServers;
}

ServerListModel *AutoconfigSetup::getCalDavServers()
{
    return m_calDavServers;
}

ServerListModel *AutoconfigSetup::getCardDavServers()
{
    return m_cardDavServers;
}
