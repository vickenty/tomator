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
	, m_l_work("Worktime", Gtk::ALIGN_START)
	, m_l_rest("Resttime", Gtk::ALIGN_START)
	, m_l_snooze("Snoozetime", Gtk::ALIGN_START)
	, m_s_work()
	, m_s_rest()
	, m_s_snooze()
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

	show_all();
}

PrefsWindow::~PrefsWindow()
{
}

void PrefsWindow::on_close_clicked()
{
	std::cout << m_s_work.get_value() << std::endl;
	std::cout << m_s_rest.get_value() << std::endl;
	std::cout << m_s_snooze.get_value() << std::endl;
	hide();
}

void PrefsWindow::on_work_time_changed()
{
	std::cout << "set work time: " << m_s_work.get_value() << std::endl;
}

void PrefsWindow::on_rest_time_changed()
{
	std::cout << "set rest time: " << m_s_rest.get_value() << std::endl;
}

void PrefsWindow::on_snooze_time_changed()
{
	std::cout << "set snooze time: " << m_s_snooze.get_value() << std::endl;
}

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.tomator.base");
	PrefsWindow win;
	return app->run(win);
}
