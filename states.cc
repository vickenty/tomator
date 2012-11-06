#include <iomanip>
#include <gtkmm.h>
#include "states.h"

namespace States {

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

void Work::enter()
{
	Base::enter();

	if (m_time_msec == 0)
		m_time_msec = m_context.config().get_work_time() * 60000;

	m_timer_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Work::on_timeout), m_time_msec);
	m_timer.reset();
}

void Work::leave()
{
	m_time_msec -= elapsed();

	Base::leave();
	m_timer_conn.disconnect();
}

bool Work::on_timeout()
{
	m_context.switch_state_new<RestPending>();
	return false;
}

guint Work::elapsed()
{
	return m_timer.elapsed() * 1000;
}

void Work::handle(Events::Pause&)
{
	m_context.push_state_new<Pause>();
}

void Work::handle(Events::Skip&)
{
	m_context.switch_state_new<Rest>();
}

Glib::ustring Work::handle(Events::GetLabel&)
{
	return format_time_label(m_time_msec - elapsed());
}

void RestPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Rest>();
}

Glib::ustring RestPending::handle(Events::GetLabel&)
{
	return "REST";
}

void Rest::handle(Events::Pause&)
{
	m_context.push_state_new<Pause>();
}

void Rest::handle(Events::Skip&)
{
	m_context.switch_state_new<Work>();
}

Glib::ustring Rest::handle(Events::GetLabel&)
{
	guint time = m_context.config().get_rest_time() * 60000;
	return format_time_label(time);
}

void WorkPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Work>();
}

}
