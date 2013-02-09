#include <glibmm.h>
#include <giomm/file.h>
#include <giomm/outputstream.h>
#include "config.h"

static void get_integer_safe(guint &dest, Glib::KeyFile &kf, const Glib::ustring& key)
{
	guint val;
	try {
		val = kf.get_integer("tomator", key);
	} catch (Glib::Error) {
		return;
	}

	if (val < 1 || val > 120)
		return;

	dest = val;
}

void Configuration::load()
{
	auto cfg = Gio::File::create_for_path(Glib::get_user_config_dir())->get_child("tomator");

	Glib::KeyFile kf;
	try {
		kf.load_from_file(cfg->get_path());
	} catch (Glib::Error) {
		return;
	}

	get_integer_safe(m_work_time, kf, "work_time");
	get_integer_safe(m_rest_time, kf, "rest_time");
	get_integer_safe(m_snooze_time, kf, "snooze_time");
}

void Configuration::save()
{
	Glib::KeyFile kf;

	kf.set_integer("tomator", "work_time", m_work_time);
	kf.set_integer("tomator", "rest_time", m_rest_time);
	kf.set_integer("tomator", "snooze_time", m_snooze_time);

	auto dir = Gio::File::create_for_path(Glib::get_user_config_dir());
	try {
		dir->make_directory_with_parents();
	} catch (Gio::Error &e) {
		if (e.code() != Gio::Error::EXISTS)
			throw e;
	}

	auto cfg = dir->get_child("tomator");

	Glib::RefPtr<Gio::FileOutputStream> stream = cfg->replace();
	stream->write(kf.to_data());
	stream->close();
}
