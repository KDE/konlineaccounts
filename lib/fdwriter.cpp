/*
 *   SPDX-FileCopyrightText: 2026 Nicolas Fella <nicolas.fella@gmx.de>
 *   SPDX-FileCopyrightText: 2025 Marco Martin <notmart@gmail.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fdwriter.h"

#include <QFile>

#include "debug.h"
#include "safe_strerror.h"

#include <sys/socket.h>
#include <unistd.h>

namespace FdWriter
{

std::optional<QDBusUnixFileDescriptor> write(const QByteArray &data)
{
    std::array<int, 2> fds{};
    if (auto ret = socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0, fds.data()); ret == -1) {
        qCWarning(LOG_KONLINEACCOUNTS_LIB) << "Failed to create socketpair for prompt result:" << safe_strerror(errno);
        return std::nullopt;
    }
    auto closer = qScopeGuard([&] {
        for (auto fd : fds) {
            close(fd);
        }
    });

    auto &[readFd, writeFd] = fds;
    QFile writeFile;
    if (!writeFile.open(writeFd, QIODevice::WriteOnly)) {
        qCWarning(LOG_KONLINEACCOUNTS_LIB) << "Failed to open write pipe fd as QFile for prompt result:" << writeFile.errorString();
        return std::nullopt;
    }
    // Sockets seem to have net.core.rmem_default buffer limits, but they are super spacious. There is practically no
    // chance of exhausting them with a password.
    writeFile.write(data);
    if (!writeFile.flush()) {
        qCWarning(LOG_KONLINEACCOUNTS_LIB) << "Failed to write on pipe fd result:" << writeFile.errorString();
        return std::nullopt;
    }

    return QDBusUnixFileDescriptor(readFd);
}

}
