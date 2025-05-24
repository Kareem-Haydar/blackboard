// NotificationListener.cpp
#include <headers/NotificationListener.h>
#include <QDebug>
#include <QDBusArgument>

void NotificationListener::NotificationReceived(const Notification& notif) {

}

NotificationListener::NotificationListener(QObject* parent) : QObject(parent) {
    QDBusConnection session = QDBusConnection::sessionBus();

    session.connect(
        "org.freedesktop.Notifications", // service
        "/org/freedesktop/Notifications", // path
        "org.freedesktop.Notifications", // interface
        "Notify", // method signal
        this,
        SLOT(onNotifyCalled(QDBusMessage))
    );
}

void NotificationListener::OnNotifyCalled(const QDBusMessage& msg) {
    const auto args = msg.arguments();
    if (args.size() < 8) return;

    Notification notif;
    notif.appName = args[0].toString();
    notif.id = args[1].toUInt();
    notif.icon = args[2].toString();
    notif.summary = args[3].toString();
    notif.body = args[4].toString();
    notif.actions = args[5].toStringList();
    notif.hints = args[6].toMap();
    notif.timeout = args[7].toInt();

    emit NotificationReceived(notif);
}
