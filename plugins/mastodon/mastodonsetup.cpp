/*
 *   SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mastodonsetup.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QOAuthHttpServerReplyHandler>
#include <QUrlQuery>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>

#include <QCoro/QCoroCore>

using namespace Qt::Literals;

MastodonSetup::MastodonSetup(QObject *parent)
    : QObject(parent)
{
}

void MastodonSetup::registerMastodon(const QString &instanceUrl)
{
    doRegisterMastodon(instanceUrl);
}

QCoro::Task<void> MastodonSetup::doRegisterMastodon(const QString &_instanceUrl)
{
    const QString instanceUrl = _instanceUrl;

    auto handler = new QOAuthHttpServerReplyHandler(1234, this);

    handler->setCallbackText(u"We're done here :)"_s);

    connect(handler, &QOAuthHttpServerReplyHandler::tokensReceived, this, [](const QVariantMap &tokens) {
        qWarning() << "tokens" << tokens;
    });

    connect(handler, &QOAuthHttpServerReplyHandler::callbackReceived, this, [](const QVariantMap &values) {
        qWarning() << "callback" << values;
    });

    connect(handler, &QOAuthHttpServerReplyHandler::tokenRequestErrorOccurred, this, [](QAbstractOAuth::Error error, const QString &errorString) {
        qWarning() << "errors" /*<< error*/ << errorString;
    });

    connect(handler, &QOAuthHttpServerReplyHandler::replyDataReceived, this, [](const QByteArray &data) {
        qWarning() << "reply" << data;
    });

    connect(handler, &QOAuthHttpServerReplyHandler::callbackDataReceived, this, [](const QByteArray &data) {
        qWarning() << "c data" << data;
    });

    // register
    const QUrl regUrl = QUrl(instanceUrl + u"/api/v1/apps");
    const QJsonObject obj{
        {QStringLiteral("client_name"), u"Not KAccounts"_s},
        {QStringLiteral("redirect_uris"), u"http://localhost:1234"_s},
        {QStringLiteral("scopes"), QStringLiteral("read write follow")},
        // {QStringLiteral("website"), website},
    };
    const QJsonDocument doc(obj);

    QNetworkRequest req(regUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QNetworkAccessManager *nam = new QNetworkAccessManager;

    auto foo = co_await nam->post(req, doc.toJson());
    qWarning() << "reply" << foo;

    QJsonDocument replyDoc = QJsonDocument::fromJson(foo);

    const QString clientId = replyDoc[u"client_id"].toString();
    const QString clientSecret = replyDoc[u"client_secret"].toString();

    QDesktopServices::openUrl(
        QUrl(instanceUrl + u"/oauth/authorize?client_id=%1&scope=read+write&redirect_uri=http://localhost:1234&response_type=code"_s.arg(clientId)));

    const QVariantMap values = co_await qCoro(handler, &QOAuthHttpServerReplyHandler::callbackReceived);

    qWarning() << "got" << values;

    const QString authCode = values[u"code"_s].toString();

    const QUrl tokenUrl = QUrl(instanceUrl + u"/oauth/token");
    QUrlQuery q;

    q.addQueryItem(QStringLiteral("client_id"), clientId);
    q.addQueryItem(QStringLiteral("client_secret"), clientSecret);
    q.addQueryItem(QStringLiteral("redirect_uri"), u"http://localhost:1234"_s);
    q.addQueryItem(QStringLiteral("grant_type"), u"authorization_code"_s);
    q.addQueryItem(QStringLiteral("code"), authCode);

    QNetworkRequest req2(tokenUrl);
    req2.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));

    auto reply = co_await nam->post(req2, q.toString().toUtf8());

    qWarning() << "reply" << reply;

    QJsonDocument authReplyDoc = QJsonDocument::fromJson(reply);

    const QString accessToken = authReplyDoc[u"access_token"].toString();

    auto mastodonGroup = m_builder->config().group(u"Mastodon"_s);
    mastodonGroup.writeEntry("instanceUrl", instanceUrl);
    mastodonGroup.writeEntry("username", "nico");
    mastodonGroup.writeEntry("clientId", clientId);
    mastodonGroup.writeEntry("clientSecret", clientSecret);
    mastodonGroup.writeEntry("accessToken", accessToken);

    m_builder->finish();
    co_return;
}

AccountBuilder *MastodonSetup::builder() const
{
    return m_builder;
}

void MastodonSetup::setBuilder(AccountBuilder *builder)
{
    m_builder = builder;
    Q_EMIT builderChanged();
}
