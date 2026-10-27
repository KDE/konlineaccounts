/*
 *   SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "accountbuilder.h"
#include "logintestlistmodel.h"

#include "serverlistmodel.h"

#include <QAbstractListModel>
#include <QQmlEngine>

class AutoconfigSetup : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)
    Q_PROPERTY(const QString email READ email NOTIFY emailChanged)
    Q_PROPERTY(const ServerListModel *imapServers READ imapServers NOTIFY imapServersChanged)
    Q_PROPERTY(const ServerListModel *pop3Servers READ pop3Servers NOTIFY pop3ServersChanged)
    Q_PROPERTY(const ServerListModel *smtpServers READ smtpServers NOTIFY smtpServersChanged)
    Q_PROPERTY(const ServerListModel *calDavServers READ calDavServers NOTIFY calDavServersChanged)
    Q_PROPERTY(const ServerListModel *cardDavServers READ cardDavServers NOTIFY cardDavServersChanged)

    Q_PROPERTY(const LoginTestListModel *loginTestResult READ loginTestResult NOTIFY loginTestResultChanged)

public:
    /**
     * This state is used directly in Main.qml to indicate the index of the page being displayed.
     * Make sure to match the enum index with the child index in the StackLayout.
     */
    enum State {
        EmailForm = 0,
        ServerSelection,
        LoginResult,
    };
    Q_ENUM(State);

    explicit AutoconfigSetup(QObject *parent = nullptr);

    Q_INVOKABLE void registerAutoconfig(const QString &email, const QString &password);
    Q_INVOKABLE void setProtocolEnabled(const KAccountAutoconfigServer::Protocol &protocol, bool enabled);
    Q_INVOKABLE void setUrlSelectedForProtocol(const KAccountAutoconfigServer::Protocol &protocol, const QUrl &url);
    Q_INVOKABLE void performLoginTests();
    Q_INVOKABLE void dumpServersToConfig();

    [[nodiscard]] AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);

    [[nodiscard]] const QString &email() const;

    [[nodiscard]] State state() const;

    [[nodiscard]] const ServerListModel *imapServers() const;
    [[nodiscard]] const ServerListModel *pop3Servers() const;
    [[nodiscard]] const ServerListModel *smtpServers() const;
    [[nodiscard]] const ServerListModel *calDavServers() const;
    [[nodiscard]] const ServerListModel *cardDavServers() const;

    [[nodiscard]] const LoginTestListModel *loginTestResult() const;

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

    void loginTestResultChanged();

private:
    void handleReply(const QList<KAccountAutoconfigServer> &servers);
    void populateServersByProtocol();
    void populateSelectedUrlMap();
    template<typename F>
    void dumpServersWithProtocolToConfig(const KAccountAutoconfigServer::Protocol &protocol, F &&callback);
    /**
     * Filters the servers returned by KAccountAutoconfigJob
     */
    QList<KAccountAutoconfigServer> getServersWithProtocol(KAccountAutoconfigServer::Protocol protocol);
    /**
     * Finds a server from the list of servers returned by KAccountAutoconfigJob by its protocol/url
     * Used because we can't pass an entire KAccountAutoconfigServer to our QML view so we can't know which entry was selected
     */
    KAccountAutoconfigServer findServerFromUrlAndProtocol(const QUrl &url, KAccountAutoconfigServer::Protocol protocol);
    QPair<KAccountAutoconfigServer, bool> getLoginResultForProtocol(KAccountAutoconfigServer::Protocol protocol);

    State m_state = EmailForm;
    AccountBuilder *m_builder;

    QString m_email;
    QString m_password;
    QList<KAccountAutoconfigServer> m_servers;
    ServerListModel *m_imapServers;
    ServerListModel *m_pop3Servers;
    ServerListModel *m_smtpServers;
    ServerListModel *m_calDavServers;
    ServerListModel *m_cardDavServers;
    QHash<KAccountAutoconfigServer::Protocol, QUrl> m_selectedUrlPerProtocol;
    QSet<KAccountAutoconfigServer::Protocol> m_enabledProtocols;

    LoginTestListModel *m_loginTestResult;
};
