#pragma once

#include <libnotify/notify.h>

class Notification {
public:
	Notification(const char *summary, const char *body = NULL, const char *icon = NULL) {
		m_obj = notify_notification_new(summary, body, icon);
	}

	~Notification() {
		g_object_unref(G_OBJECT(m_obj));
	}

	bool show() {
		return notify_notification_show(m_obj, NULL);
	}

private:
	NotifyNotification *m_obj;
};
