/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "accountbuilder.h"
#include "kautoconfigserver.h"

#include <QQmlEngine>

class AutoconfigSetup : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(State state MEMBER m_state NOTIFY stateChanged)
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    Q_PROPERTY(QStringList imapServers READ getImapServers NOTIFY imapServersChanged)
    Q_PROPERTY(QStringList pop3Servers READ getPop3Servers NOTIFY pop3ServersChanged)
    Q_PROPERTY(QStringList smtpServers READ getSmtpServers NOTIFY smtpServersChanged)
    Q_PROPERTY(QStringList calDavServers READ getCalDavServers NOTIFY calDavServersChanged)
    Q_PROPERTY(QStringList cardDavServers READ getCardDavServers NOTIFY cardDavServersChanged)

public:
    enum State {
        EmailForm = 0,
        ServerSelection,
    };
    Q_ENUM(State);

    explicit AutoconfigSetup(QObject *parent = nullptr);

    Q_INVOKABLE void registerAutoconfig(const QString &email);
    Q_INVOKABLE void setProtocolEnabled(const QString &protocol_str, bool enabled);
    Q_INVOKABLE void setUrlSelectedForProtocol(const QString &protocol_str, const QString &url);
    Q_INVOKABLE void dumpServersToConfig();

    AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);

    QString getEmail();

    QStringList getImapServers();
    QStringList getPop3Servers();
    QStringList getSmtpServers();
    QStringList getCalDavServers();
    QStringList getCardDavServers();

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
    void populateSelectedUrlMap();
    QStringList getUrlForServersWithProtocol(KAutoconfigServer::Protocol protocol);

    State m_state = EmailForm;
    AccountBuilder *m_builder;

    QString m_email;
    QList<KAutoconfigServer> m_servers;
    QHash<KAutoconfigServer::Protocol, QString> m_selectedUrlPerProtocol;
    QSet<KAutoconfigServer::Protocol> m_enabledProtocols;
};
