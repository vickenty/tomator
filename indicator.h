#pragma once

#include <glibmm.h>
#include <libappindicator/app-indicator.h>

class Indicator {
public:
	Indicator() {
		Glib::ustring icon_path = get_icon("tomator.svg");
		m_obj = app_indicator_new("tomator", icon_path.c_str(),
				APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
	}

	~Indicator() {
		g_object_unref(G_OBJECT(m_obj));
	}

	Glib::ustring get_icon(Glib::ustring icon) {
		Glib::ustring path = Glib::get_current_dir() + "/" + icon;
		if (Glib::file_test(path, Glib::FILE_TEST_EXISTS)) {
			return path;
		}

		return icon;
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

private:
	AppIndicator *m_obj;
};
