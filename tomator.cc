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

	int run(int, char**);

	static Glib::RefPtr<Tomator> create(Glib::RefPtr<Gtk::Application>);
private:
	Tomator(Glib::RefPtr<Gtk::Application>);

	int m_work_time;
	int m_rest_time;
	int m_snooze_time;

	sigc::connection m_timer;
	Glib::RefPtr<Gtk::Application> m_app;
	Glib::RefPtr<Gtk::StatusIcon> m_icon;
	PrefsWindow *m_prefswin;

	enum mode_t { WORK, REST };
	mode_t m_mode;
};

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

	m_prefswin = new PrefsWindow();
	m_app->add_window(*m_prefswin);
}

void Tomator::on_icon_activation()
{
	m_prefswin->show();
}

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("org.tomator.core");
	Glib::RefPtr<Tomator> core = Tomator::create(app);
	return core->run(argc, argv);
}
