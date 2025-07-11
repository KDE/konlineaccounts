/*
 *  SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.loadFromModule("org.kde.konlineaccounts.demo", "Main");

    return app.exec();
}
