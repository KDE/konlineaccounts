# SPDX-FileCopyrightText: 2025 Nicolas Fella <nicolas.fella@gmx.de>
# SPDX-FileCopyrightText: 2026 Kenny Lorin <kenny.lorin@enioka.com>
# SPDX-License-Identifier: BSD-2-Clause

if (USE_AUTOCONFIG_PLUGIN)
    add_subdirectory(autoconfig)
endif()
add_subdirectory(nextcloud)
add_subdirectory(mastodon)
add_subdirectory(google)
