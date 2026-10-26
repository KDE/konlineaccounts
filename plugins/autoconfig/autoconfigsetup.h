/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "accountbuilder.h"
#include "autoconfighelper.h"
#include "kautoconfigserver.h"

#include <QAbstractListModel>
#include <QQmlEngine>

class ServerListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool empty READ isEmpty NOTIFY countChanged)

public:
    ServerListModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    enum ServerDisplayRoles {
        UrlRole = Qt::UserRole + 1,
        HostRole,
        PortRole,
        ProtocolRole,
        AuthenticationTypeRole,
        SocketTypeRole,
    };

    int rowCount(const QModelIndex &parent) const override
    {
        return m_items.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() >= m_items.size())
            return {};

        const auto &item = m_items.at(index.row());
        switch (role) {
        case UrlRole:
            return item.url();
        case HostRole:
            return item.url().host();
        case PortRole:
            return item.url().port();
        case ProtocolRole:
            return AutoconfigHelper::protocolToString(item.protocol());
        case AuthenticationTypeRole:
            return AutoconfigHelper::authenticationTypeToString(item.authenticationType());
        case SocketTypeRole:
            return AutoconfigHelper::socketTypeToString(item.socketType());
        default:
            return {};
        }
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            {UrlRole, "url"},
            {HostRole, "host"},
            {PortRole, "port"},
            {ProtocolRole, "protocol"},
            {AuthenticationTypeRole, "authenticationType"},
            {SocketTypeRole, "socketType"},
        };
    }

    void setServers(const QList<KAutoconfigServer> &servers)
    {
        beginResetModel();
        m_items = servers;
        endResetModel();
        Q_EMIT countChanged();
    }

    bool isEmpty() const
    {
        return m_items.isEmpty();
    }

    KAutoconfigServer &first()
    {
        return m_items.first();
    }

Q_SIGNALS:
    void countChanged();

private:
    QList<KAutoconfigServer> m_items;
};

class AutoconfigSetup : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(State state MEMBER m_state NOTIFY stateChanged)
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    Q_PROPERTY(ServerListModel *imapServers READ getImapServers NOTIFY imapServersChanged)
    Q_PROPERTY(ServerListModel *pop3Servers READ getPop3Servers NOTIFY pop3ServersChanged)
    Q_PROPERTY(ServerListModel *smtpServers READ getSmtpServers NOTIFY smtpServersChanged)
    Q_PROPERTY(ServerListModel *calDavServers READ getCalDavServers NOTIFY calDavServersChanged)
    Q_PROPERTY(ServerListModel *cardDavServers READ getCardDavServers NOTIFY cardDavServersChanged)

public:
    enum State {
        EmailForm = 0,
        ServerSelection,
    };
    Q_ENUM(State);

    explicit AutoconfigSetup(QObject *parent = nullptr);

    Q_INVOKABLE void registerAutoconfig(const QString &email, const QString &password);
    Q_INVOKABLE void setProtocolEnabled(const QString &protocol_str, bool enabled);
    Q_INVOKABLE void setUrlSelectedForProtocol(const QString &protocol_str, const QUrl &url);
    Q_INVOKABLE void dumpServersToConfig();

    AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);

    QString getEmail();

    ServerListModel *getImapServers();
    ServerListModel *getPop3Servers();
    ServerListModel *getSmtpServers();
    ServerListModel *getCalDavServers();
    ServerListModel *getCardDavServers();

Q_SIGNALS:
    void stateChanged();
    void autoconfigJobFailed();
    void builderChanged();
    void emailChanged();

    void imapServersChanged();
    void pop3ServersChanged();
    void smtpServersChanged();
    void calDavServersChanged();
    void cardDavServersChanged();

private:
    void handleReply(const QList<KAutoconfigServer> &servers);
    void populateServersByProtocol();
    void populateSelectedUrlMap();
    template<typename F>
    void dumpServersWithProtocolToConfig(const KAutoconfigServer::Protocol &protocol, F &&callback);
    /**
     * Filters the servers returned by KAccountAutoconfigJob
     */
    QList<KAutoconfigServer> getServersWithProtocol(KAutoconfigServer::Protocol protocol);
    /**
     * Finds a server from the list of servers returned by KAccountAutoconfigJob by its protocol/url
     * Used because we can't pass an entire KAutoconfigServer to our QML view so we can't know which entry was selected
     */
    KAutoconfigServer findServerFromUrlAndProtocol(const QUrl &url, KAutoconfigServer::Protocol protocol);

    State m_state = EmailForm;
    AccountBuilder *m_builder;

    QString m_email;
    QString m_password;
    QList<KAutoconfigServer> m_servers;
    ServerListModel *m_imapServers;
    ServerListModel *m_pop3Servers;
    ServerListModel *m_smtpServers;
    ServerListModel *m_calDavServers;
    ServerListModel *m_cardDavServers;
    QHash<KAutoconfigServer::Protocol, QUrl> m_selectedUrlPerProtocol;
    QSet<KAutoconfigServer::Protocol> m_enabledProtocols;
};
