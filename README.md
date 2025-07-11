## KOnlineAccounts

KOnlineAccounts is a system service to allow applications to obtain information about system-wide logged in accounts. It's designed to replace the current KAccounts system.

### Why replace KAccounts?

KAccounts suffers from a number of problems that are hard to address in an iterative manner.

- Not many applications actually use it. Notably absent is KDE PIM
- Its architecture is not sandbox-friendly. Applications more or less directly read account data from a database in the home directory. If they can read the database they have access to all accounts of all types. There is no way to do fine-grained access control.
- The architecture is rather complex. Responsibilities are split across a dozen repos, some of which are outside the direct control of KDE and mostly unmaintained. 

### Design Goals

- Be simple and flexible/extensible. Avoid making too many assumptions about different account types.
- Be friendly to Flatpak and other sandboxed formats.
- Allow per-app per-account access control, where feasible
- Separate functionality from provider. A given provider (e.g. Nextcloud) can expose more than one functionality (e.g. webdav, carddav, caldav), and a given functionality can be provided my more than one provider (e.g. caldav can be provided by Nextcloud or a company-specific provider)
- Be extensible with custom providers
- Allow applications to leverage it without heavy dependencies

### Architecture

The central piece is the konlineaccountsd dameon. It provides a DBus interface that applications use to obtain all information about the accounts.

Applications can list the accounts they have access to, and request access to a new account.

Every granted account is exposed as an object on the Bus. Account functionality is exposed using specific interfaces on that object. For example a Nextcloud account might expose a Nextcloud-specific interface, a CalDAV interface, and a CardDAV interface.

Access control is managed based on the application ID/desktop file name. While for unsandboxed applications this can be easily spoofed, for Flatpak applications we obtain the true app ID from the Flatpak runtime, similar to what xdg-desktop-portal does.

A KCM allows to add and manage accounts. When adding a new account the KCM can suggest applications that can make use of this account and offers to directly authenticate them.

### Providers

Currently the following providers are supported:

- Nextcloud
- Mastodon
- Google

### Consumers

Check the demo application under `demo/`.

[Tokodon](https://invent.kde.org/network/tokodon/-/merge_requests/777)

[Purpose](https://invent.kde.org/frameworks/purpose/-/merge_requests/146)

KDE PIM:
 - [Akonadi](https://invent.kde.org/pim/akonadi/-/tree/work/nico/onlineaccounts)
 - [kdepim-runtime](https://invent.kde.org/pim/kdepim-runtime/-/tree/work/nico/not-kaccounts)
 - [Merkuro](https://invent.kde.org/pim/merkuro/-/tree/work/nico/onlineaccounts)

### Status

This project is in a prototype stage, not ready for production.

Check the Issues page and inline comments for things to be done.
