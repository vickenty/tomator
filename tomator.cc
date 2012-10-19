#include <iostream>
#include <gtkmm.h>

class PrefsWindow : public Gtk::Window
{
public:
	PrefsWindow();
	virtual ~PrefsWindow();

	void on_close_clicked();

private:
	Gtk::Button m_b_close;
};

PrefsWindow::PrefsWindow()
	: m_b_close("Close")
{
	set_border_width(10);
	m_b_close.signal_clicked().connect(sigc::mem_fun(*this, &PrefsWindow::on_close_clicked));
	add(m_b_close);
	m_b_close.show();
}

PrefsWindow::~PrefsWindow()
{
}

void PrefsWindow::on_close_clicked()
{
	hide();
}

int main(int argc, char **argv)
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.tomator.base");
	PrefsWindow win;
	return app->run(win);
}
