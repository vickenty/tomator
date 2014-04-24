#include <iostream>
#include <gtkmm.h>
#include <libnotify/notify.h>
#include "states.h"
#include "config.h"
#include "indicator.h"

class PrefsWindow : public Gtk::Window
{
public:
	PrefsWindow(Configuration &);
	virtual ~PrefsWindow();

	void on_close_clicked();

	void on_work_time_changed();
	void on_rest_time_changed();
	void on_snooze_time_changed();

private:
	Gtk::VBox m_vbox;
	Gtk::Grid m_grid;
	Gtk::Label m_l_work;
	Gtk::Label m_l_rest;
	Gtk::Label m_l_snooze;
	Gtk::SpinButton m_s_work;
	Gtk::SpinButton m_s_rest;
	Gtk::SpinButton m_s_snooze;
	Gtk::ButtonBox m_bbox;
	Gtk::Button m_b_close;
	Configuration &m_config;
};

PrefsWindow::PrefsWindow(Configuration &config)
	: m_b_close("Close")
	, m_l_work("Work time", Gtk::ALIGN_START)
	, m_l_rest("Rest time", Gtk::ALIGN_START)
	, m_l_snooze("Snooze time", Gtk::ALIGN_START)
	, m_config(config)
{
	set_border_width(10);
	set_resizable(false);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_icon_name("tomator");
	m_b_close.signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::on_close_clicked));

	Glib::RefPtr<Gtk::Adjustment> work_time = Gtk::Adjustment::create(config.get_work_time(), 1, 120, 1, 5);
	Glib::RefPtr<Gtk::Adjustment> rest_time = Gtk::Adjustment::create(config.get_rest_time(), 1, 120, 1, 5);
	Glib::RefPtr<Gtk::Adjustment> snooze_time = Gtk::Adjustment::create(config.get_snooze_time(), 1, 120, 1, 5);

	m_s_work.configure(work_time, 1, 0);
	work_time->signal_value_changed().connect(sigc::mem_fun(*this, &PrefsWindow::on_work_time_changed));

	m_s_rest.configure(rest_time, 1, 0);
	rest_time->signal_value_changed().connect(sigc::mem_fun(*this, &PrefsWindow::on_rest_time_changed));

	m_s_snooze.configure(snooze_time, 1, 0);
	snooze_time->signal_value_changed().connect(sigc::mem_fun(*this, &PrefsWindow::on_snooze_time_changed));

	m_grid.set_column_spacing(10);
	m_grid.set_row_spacing(10);

	m_grid.insert_column(0);
	m_grid.insert_row(0);
	m_grid.insert_row(0);
	m_grid.insert_row(0);

	m_grid.attach(m_l_work, 0, 0, 1, 1);
	m_grid.attach(m_s_work, 1, 0, 1, 1);
	m_grid.attach(m_l_rest, 0, 1, 1, 1);
	m_grid.attach(m_s_rest, 1, 1, 1, 1);
	m_grid.attach(m_l_snooze, 0, 2, 1, 1);
	m_grid.attach(m_s_snooze, 1, 2, 1, 1);

	m_vbox.pack_start(m_grid, false, false, 10);
	m_vbox.pack_start(m_bbox, false, false, 0);

	m_bbox.set_halign(Gtk::ALIGN_END);
	m_bbox.pack_start(m_b_close);

	add(m_vbox);
	m_vbox.show_all_children();
	m_vbox.show();

}

PrefsWindow::~PrefsWindow()
{
}

void PrefsWindow::on_close_clicked()
{
	m_config.save();
	hide();
}

void PrefsWindow::on_work_time_changed()
{
	m_config.set_work_time(m_s_work.get_value());
}

void PrefsWindow::on_rest_time_changed()
{
	m_config.set_rest_time(m_s_rest.get_value());
}

void PrefsWindow::on_snooze_time_changed()
{
	m_config.set_snooze_time(m_s_snooze.get_value());
}

class StatusWindow : public Gtk::Window
{
public:
	StatusWindow();
	virtual ~StatusWindow();

	void on_close_clicked();
	void on_next_clicked();
	void on_prefs_clicked();

	void set_label(const Glib::ustring&);

	Glib::SignalProxy0<void> signal_pause() { return m_b_pause.signal_clicked(); }
	Glib::SignalProxy0<void> signal_prefs() { return m_b_prefs.signal_clicked(); }
	Glib::SignalProxy0<void> signal_next() { return m_b_next.signal_clicked(); }

private:
	Gtk::Label m_l_timer;
	Gtk::Button m_b_pause;
	Gtk::Button m_b_prefs;
	Gtk::Button m_b_close;
	Gtk::Button m_b_next;
	Gtk::ButtonBox m_bbox_act;
	Gtk::ButtonBox m_bbox_util;
	Gtk::VBox m_vbox;
};

StatusWindow::StatusWindow()
	: m_l_timer("PAUSE")
	, m_b_pause("Pause")
	, m_b_close("Close")
	, m_b_prefs("Setup")
	, m_b_next("Next")
{
	set_border_width(10);
	set_resizable(false);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	set_icon_name("tomator");

	Pango::FontDescription fd("Monospace 32");
	m_l_timer.override_font(fd);

	m_b_close.signal_clicked().connect(sigc::mem_fun(*this, &StatusWindow::on_close_clicked));

	m_bbox_act.set_layout(Gtk::BUTTONBOX_SPREAD);
	m_bbox_act.set_spacing(25);
	m_bbox_act.pack_start(m_b_pause);
	m_bbox_act.pack_start(m_b_next);

	m_bbox_util.set_layout(Gtk::BUTTONBOX_SPREAD);
	m_bbox_util.set_spacing(25);
	m_bbox_util.pack_start(m_b_prefs);
	m_bbox_util.pack_start(m_b_close);

	m_vbox.pack_start(m_l_timer, false, false, 25);
	m_vbox.pack_start(m_bbox_act, false, false, 10);
	m_vbox.pack_start(m_bbox_util, false, false, 0);

	add(m_vbox);
	m_vbox.show_all_children();
	m_vbox.show();
}

StatusWindow::~StatusWindow()
{
}

void StatusWindow::on_close_clicked()
{
	hide();
}

void StatusWindow::set_label(const Glib::ustring& label)
{
	m_l_timer.set_label(label);
}

class Tomator : public Glib::ObjectBase
{
public:

	void set_work_time(int);
	int get_work_time();

	void set_rest_time(int);
	int get_rest_time();

	void set_snooze_time(int);
	int get_snooze_time();

	void start();
	void snooze();

	void on_startup();
	void on_icon_activation();
	void on_icon_menu_popup(guint, guint32);
	void on_menu_exit();
	void on_pause_resume();
	bool on_update_timer();
	void update_label();
	void init_icon_theme();

	void switch_to_next();
	void show_status();
	void show_preferences();

	int run(int, char**);

	static Glib::RefPtr<Tomator> create(Glib::RefPtr<Gtk::Application>);

	static Glib::ustring s_menu_xml;

private:
	Tomator(Glib::RefPtr<Gtk::Application>);

	Glib::RefPtr<Gtk::Application> m_app;

	Indicator m_indicator;

	Glib::RefPtr<Gtk::UIManager> m_ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> m_action_group;

	sigc::connection m_update_timer;

	PrefsWindow *m_prefswin;
	StatusWindow *m_statuswin;
	Gtk::Menu *m_menu;

	States::Context m_context;
	Configuration m_config;
};

Glib::ustring Tomator::s_menu_xml =
		"<ui>"
		"	<popup name='icon_menu'>"
		"		<menuitem action='show'/>"
		"		<menuitem action='next'/>"
		"		<menuitem action='pause'/>"
		"		<menuitem action='edit'/>"
		"		<menuitem action='exit'/>"
		"	</popup>"
		"</ui>";

Tomator::Tomator(Glib::RefPtr<Gtk::Application> app)
	: m_app(app)
	, m_context(m_config)
{
}

Glib::RefPtr<Tomator> Tomator::create(Glib::RefPtr<Gtk::Application> app)
{
	return Glib::RefPtr<Tomator>(new Tomator(app));
}

int Tomator::run(int argc, char** argv)
{
	m_app->hold();
	m_app->signal_startup().connect(sigc::mem_fun(*this, &Tomator::on_startup));
	return m_app->run(argc, argv);
}

void Tomator::init_icon_theme() {
	auto theme = Gtk::IconTheme::get_default();
	std::string prefix = Glib::get_current_dir() + "/icons";
	theme->append_search_path(prefix);
}

void Tomator::on_startup()
{
	Glib::set_application_name("Tomator");

	m_config.load();

	init_icon_theme();

	m_prefswin = new PrefsWindow(m_config);
	m_app->add_window(*m_prefswin);

	m_statuswin = new StatusWindow();
	m_statuswin->signal_pause().connect(sigc::mem_fun(*this, &Tomator::on_pause_resume));
	m_statuswin->signal_prefs().connect(sigc::mem_fun(*this, &Tomator::show_preferences));
	m_statuswin->signal_next().connect(sigc::mem_fun(*this, &Tomator::switch_to_next));
	m_app->add_window(*m_statuswin);

	m_action_group = Gtk::ActionGroup::create();
	m_action_group->add(
		Gtk::Action::create("exit", "Exit"),
		sigc::mem_fun(*this, &Tomator::on_menu_exit));
	m_action_group->add(
		Gtk::Action::create("edit","Edit preferences"),
		sigc::mem_fun(*this, &Tomator::show_preferences));
	m_action_group->add(
		Gtk::Action::create("show", "Show timer"),
		sigc::mem_fun(*this, &Tomator::show_status));
	m_action_group->add(
		Gtk::Action::create("pause", "Pause"),
		sigc::mem_fun(*this, &Tomator::on_pause_resume));
	m_action_group->add(
		Gtk::Action::create("next", "Next"),
		sigc::mem_fun(*this, &Tomator::switch_to_next));

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);
	m_ui_manager->add_ui_from_string(Tomator::s_menu_xml);
	m_menu = dynamic_cast<Gtk::Menu*>(m_ui_manager->get_widget("/icon_menu"));

	m_context.signal_state_changed().connect(sigc::mem_fun(*this, &Tomator::update_label));
	m_update_timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Tomator::on_update_timer), 100);

	m_context.init_state_new<States::Work>();
	Events::Pause pause;
	m_context.send(pause);
}

void Tomator::show_status() {
	m_statuswin->show();
}

void Tomator::show_preferences()
{
	m_prefswin->show();
}

void Tomator::on_menu_exit()
{
	m_app->quit();
}

void Tomator::on_pause_resume()
{
	Events::Pause pause;
	m_context.send(pause);
}

void Tomator::update_label()
{
	Glib::ustring str;

	Events::GetLabel get_label;
	str = m_context.send(get_label);
	m_indicator.set_label(str);

	Events::GetIcon get_icon;
	str = m_context.send(get_icon);
	m_indicator.set_icon(str);

	Events::GetClock get_clock;
	str = m_context.send(get_clock);
	m_statuswin->set_label(str);
}

bool Tomator::on_update_timer()
{
	update_label();
	return true;
}

void Tomator::switch_to_next()
{
	Events::Skip skip;
	m_context.send(skip);
}

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("org.tomator.core");
	Glib::RefPtr<Tomator> core = Tomator::create(app);
	notify_init("Tomator");
	return core->run(argc, argv);
}
