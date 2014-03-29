#include <iomanip>
#include <gtkmm.h>
#include "states.h"
#include "notification.h"

namespace States {

void show_notification(Glib::ustring msg)
{
	auto theme = Gtk::IconTheme::get_default();
	auto info = theme->lookup_icon("tomator", 0, Gtk::ICON_LOOKUP_FORCE_SVG);
	auto path = info.get_filename();
	Notification("Tomator", msg.c_str(), path.c_str()).show();
}

void Base::enter()
{
	signal_state_enter().emit(this);
}

void Base::leave()
{
	signal_state_leave().emit(this);
}

Glib::ustring Base::format_time_label(gulong msec)
{
	gulong seconds = msec / 1000;
	return Glib::ustring::compose("%1:%2",
		Glib::ustring::format(std::setfill(L'0'), std::setw(2), seconds / 60),
		Glib::ustring::format(std::setfill(L'0'), std::setw(2), seconds % 60));
}

void Pause::handle(Events::Pause&)
{
	m_context.pop_state();
}

Glib::ustring Pause::handle(Events::GetLabel&)
{
	return "PAUSED";
}

void TimerState::enter()
{
	Base::enter();

	if (m_time_msec == 0)
		m_time_msec = default_timeout();

	m_timer_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &TimerState::on_timeout), m_time_msec);
	m_timer.reset();
}

void TimerState::leave()
{
	Base::leave();
	m_time_msec -= elapsed();
	m_timer_conn.disconnect();
}

void TimerState::handle(Events::Pause&)
{
	m_context.push_state_new<Pause>();
}

Glib::ustring TimerState::handle(Events::GetLabel&)
{
	return format_time_label(m_time_msec - elapsed());
}


guint TimerState::elapsed()
{
	return m_timer.elapsed() * 1000;
}

guint Work::default_timeout()
{
	return m_context.config().get_work_time() * 60000;
}

bool Work::on_timeout()
{
	m_context.switch_state_new<RestPending>();
	return false;
}

void Work::handle(Events::Skip&)
{
	m_context.switch_state_new<Rest>();
}


void RestPending::enter()
{
	Base::enter();
	show_notification("Get some rest");
}

void RestPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Rest>();
}

Glib::ustring RestPending::handle(Events::GetLabel&)
{
	return "REST";
}

bool Rest::on_timeout()
{
	m_context.switch_state_new<WorkPending>();
	return false;
}

guint Rest::default_timeout()
{
	return m_context.config().get_rest_time() * 60000;
}

void Rest::handle(Events::Skip&)
{
	m_context.switch_state_new<Work>();
}

void WorkPending::enter()
{
	Base::enter();
	show_notification("Time to get to work");
}

void WorkPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Work>();
}

Glib::ustring WorkPending::handle(Events::GetLabel&)
{
	return "WORK";
}

}
