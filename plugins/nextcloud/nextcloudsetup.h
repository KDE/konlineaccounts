/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QQuickWebEngineProfile>
#include <QVariant>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>

#include <QCoro/QCoroCore>

#include "accountbuilder.h"

class NextcloudUrlIntercepter : public QWebEngineUrlRequestInterceptor
{
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;
};

class NextcloudSetup : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(State state MEMBER m_state NOTIFY stateChanged)
    Q_PROPERTY(QQuickWebEngineProfile *webengineProfile MEMBER m_webengineProfile CONSTANT)
    Q_PROPERTY(QUrl loginUrl MEMBER m_loginUrl NOTIFY loginUrlChanged)
    Q_PROPERTY(AccountBuilder *builder READ builder WRITE setBuilder NOTIFY builderChanged)

public:
    enum State {
        ServerUrl = 0,
        WebLogin,
    };
    Q_ENUM(State)

    explicit NextcloudSetup(QObject *parent = nullptr);

    Q_INVOKABLE void checkServer(const QString &server);

    AccountBuilder *builder() const;
    void setBuilder(AccountBuilder *builder);
    Q_SIGNAL void builderChanged();

    bool isWorking();
    QString errorMessage() const;

Q_SIGNALS:
    void loginUrlChanged();
    void isWorkingChanged();
    void errorMessageChanged();
    void stateChanged();

private Q_SLOTS:
    void finalUrlHandler(const QUrl &url);

private:
    QCoro::Task<void> checkServer(const QUrl &url);
    void setWorking(bool start);
    QCoro::Task<void> serverCheckResult();
    void wrongUrlDetected();

    QString m_errorMessage;
    QQuickWebEngineProfile *m_webengineProfile;
    NextcloudUrlIntercepter m_urlIntercepter;
    QUrl m_loginUrl;
    QNetworkAccessManager m_nam;
    QUrl m_server;
    bool m_isWorking = false;
    State m_state = ServerUrl;

    AccountBuilder *m_builder;
};
