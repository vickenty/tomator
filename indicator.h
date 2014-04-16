#pragma once

#include <glibmm.h>
#include <libappindicator/app-indicator.h>

class Indicator {
public:
	Indicator() {
		Glib::ustring icon_path = get_icon_path();
		m_obj = app_indicator_new_with_path("tomator", "tomator-panel",
				APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
				icon_path.length() ? icon_path.c_str() : NULL);
	}

	~Indicator() {
		g_object_unref(G_OBJECT(m_obj));
	}

	Glib::ustring get_icon_path() {
		Glib::ustring root = Glib::get_current_dir() + "/icons";
		Glib::ustring test = root + "/ubuntu-mono-light/scalable/apps/tomator.svg";
		if (Glib::file_test(test, Glib::FILE_TEST_EXISTS)) {
			return root;
		}
		return "";
	}

	void set_active(bool active) {
		app_indicator_set_status(m_obj,
			active
			? APP_INDICATOR_STATUS_ACTIVE
			: APP_INDICATOR_STATUS_PASSIVE);
	}

	void set_attention(bool attn) {
		app_indicator_set_status(m_obj,
			attn
			? APP_INDICATOR_STATUS_ATTENTION
			: APP_INDICATOR_STATUS_ACTIVE);
	}

	void set_menu(Gtk::Menu *menu) {
		app_indicator_set_menu(m_obj, menu->gobj());
	}

	void set_label(Glib::ustring &label) {
		app_indicator_set_label(m_obj, label.c_str(), NULL);
	}

	void set_icon(Glib::ustring &icon) {
		app_indicator_set_icon(m_obj, icon.c_str());
	}

private:
	AppIndicator *m_obj;
};
