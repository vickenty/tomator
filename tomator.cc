#include <iostream>
#include <gtkmm.h>

class PrefsWindow : public Gtk::Window
{
public:
	PrefsWindow();
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
};

PrefsWindow::PrefsWindow()
	: m_b_close("Close")
	, m_l_work("Work time", Gtk::ALIGN_START)
	, m_l_rest("Rest time", Gtk::ALIGN_START)
	, m_l_snooze("Snooze time", Gtk::ALIGN_START)
{
	set_border_width(10);
	set_resizable(false);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	m_b_close.signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::on_close_clicked));

	Glib::RefPtr<Gtk::Adjustment> work_time = Gtk::Adjustment::create(25, 1, 120, 1, 5);
	Glib::RefPtr<Gtk::Adjustment> rest_time = Gtk::Adjustment::create(5, 1, 120, 1, 5);
	Glib::RefPtr<Gtk::Adjustment> snooze_time = Gtk::Adjustment::create(5, 1, 120, 1, 5);

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
	hide();
}

void PrefsWindow::on_work_time_changed()
{
}

void PrefsWindow::on_rest_time_changed()
{
}

void PrefsWindow::on_snooze_time_changed()
{
}

class StatusWindow : public Gtk::Window
{
public:
	StatusWindow();
	virtual ~StatusWindow();

	void on_close_clicked();
	void on_next_clicked();
	void on_prefs_clicked();

	sigc::signal<void> signal_next() { return m_signal_next; }
	sigc::signal<void> signal_prefs() { return m_signal_prefs; }

private:
	Gtk::Label m_l_timer;
	Gtk::Button m_b_prefs;
	Gtk::Button m_b_close;
	Gtk::Button m_b_next;
	Gtk::ButtonBox m_bbox;
	Gtk::VBox m_vbox;

	sigc::signal<void> m_signal_next;
	sigc::signal<void> m_signal_prefs;

};

StatusWindow::StatusWindow()
	: m_l_timer("PAUSE")
	, m_b_close("Close")
	, m_b_prefs("Preferences")
	, m_b_next("Next")
{
	set_border_width(10);
	set_resizable(false);
	set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);

	Pango::FontDescription fd("Monospace 32");
	m_l_timer.override_font(fd);

	m_b_close.signal_clicked().connect(sigc::mem_fun(*this, &StatusWindow::on_close_clicked));
	m_b_next.signal_clicked().connect(sigc::mem_fun(*this, &StatusWindow::on_next_clicked));
	m_b_prefs.signal_clicked().connect(sigc::mem_fun(*this, &StatusWindow::on_prefs_clicked));

	m_bbox.set_halign(Gtk::ALIGN_END);
	m_bbox.pack_start(m_b_next);
	m_bbox.pack_start(m_b_prefs);
	m_bbox.pack_start(m_b_close);

	m_vbox.pack_start(m_l_timer, false, false, 10);
	m_vbox.pack_start(m_bbox, false, false, 0);

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

void StatusWindow::on_next_clicked()
{
	m_signal_next.emit();
}

void StatusWindow::on_prefs_clicked()
{
	m_signal_prefs.emit();
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

	void schedule();
	bool on_timeout();
	void on_startup();
	void on_icon_activation();
	void on_icon_menu_popup(guint, guint32);
	void on_menu_exit();

	void show_preferences();

	int run(int, char**);

	static Glib::RefPtr<Tomator> create(Glib::RefPtr<Gtk::Application>);

	static Glib::ustring s_menu_xml;

private:
	Tomator(Glib::RefPtr<Gtk::Application>);

	int m_work_time;
	int m_rest_time;
	int m_snooze_time;

	sigc::connection m_timer;
	Glib::RefPtr<Gtk::Application> m_app;
	Glib::RefPtr<Gtk::StatusIcon> m_icon;

	Glib::RefPtr<Gtk::UIManager> m_ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> m_action_group;

	PrefsWindow *m_prefswin;
	StatusWindow *m_statuswin;
	Gtk::Menu *m_menu;

	enum mode_t { WORK, REST };
	mode_t m_mode;
};

Glib::ustring Tomator::s_menu_xml =
		"<ui>"
		"	<popup name='icon_menu'>"
		"		<menuitem action='edit'/>"
		"		<menuitem action='exit'/>"
		"	</popup>"
		"</ui>";

Tomator::Tomator(Glib::RefPtr<Gtk::Application> app)
	: m_app(app)
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

void Tomator::set_work_time(int time)
{
	m_work_time = time;
}

int Tomator::get_work_time()
{
	return m_work_time;
}

void Tomator::set_rest_time(int time)
{
	m_rest_time = time;
}

int Tomator::get_rest_time()
{
	return m_rest_time;
}

void Tomator::set_snooze_time(int time)
{
	m_snooze_time = time;
}

int Tomator::get_snooze_time()
{
	return m_snooze_time;
}

void Tomator::schedule()
{
	m_timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Tomator::on_timeout), 1000);
}

bool Tomator::on_timeout()
{
	std::cout << "YAY" << std::endl;
	return false;
}

void Tomator::on_startup()
{
	m_icon = Gtk::StatusIcon::create(Gtk::Stock::FILE);
	m_icon->set_has_tooltip();
	m_icon->set_tooltip_text("Tomator");
	m_icon->signal_activate().connect(sigc::mem_fun(*this, &Tomator::on_icon_activation));
	m_icon->signal_popup_menu().connect(sigc::mem_fun(*this, &Tomator::on_icon_menu_popup));

	m_prefswin = new PrefsWindow();
	m_app->add_window(*m_prefswin);

	m_statuswin = new StatusWindow();
	m_statuswin->signal_prefs().connect(sigc::mem_fun(*this, &Tomator::show_preferences));
	m_app->add_window(*m_statuswin);

	m_action_group = Gtk::ActionGroup::create();
	m_action_group->add(
		Gtk::Action::create("exit", "Exit"),
		sigc::mem_fun(*this, &Tomator::on_menu_exit));
	m_action_group->add(
		Gtk::Action::create("edit","Edit preferences"),
		sigc::mem_fun(*this, &Tomator::show_preferences));

	m_ui_manager = Gtk::UIManager::create();
	m_ui_manager->insert_action_group(m_action_group);
	m_ui_manager->add_ui_from_string(Tomator::s_menu_xml);
	m_menu = dynamic_cast<Gtk::Menu*>(m_ui_manager->get_widget("/icon_menu"));
}

void Tomator::on_icon_activation()
{
	m_statuswin->show();
}

void Tomator::on_icon_menu_popup(guint button, guint32 activate_time)
{
	m_menu->popup(button, activate_time);
}

void Tomator::show_preferences()
{
	m_prefswin->show();
}

void Tomator::on_menu_exit()
{
	m_app->quit();
}

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("org.tomator.core");
	Glib::RefPtr<Tomator> core = Tomator::create(app);
	return core->run(argc, argv);
}
