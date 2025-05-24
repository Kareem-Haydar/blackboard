// NotificationListener.h
#pragma once

#include <QObject>
#include <QString>
#include <QDBusMessage>
#include <QDBusConnection>

struct Notification {
    QString appName;
    uint id;
    QString summary;
    QString body;
    QString icon;
    QStringList actions;
    QVariantMap hints;
    int timeout;
};

class NotificationListener : public QObject {
    Q_OBJECT

public:
    NotificationListener(QObject* parent = nullptr);

signals:
    void NotificationReceived(const Notification& notif);

private:
    void OnNotifyCalled(const QDBusMessage& message);
};

