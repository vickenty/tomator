#include <iostream>
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

void Pause::handle(Events::Pause&)
{
	m_context.pop_state();
}

Glib::ustring Pause::handle(Events::GetLabel&)
{
	return "PAUSED";
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
	return "WORKING";
}

void RestPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Rest>();
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
	return "RESTING";
}

void WorkPending::handle(Events::Skip&)
{
	m_context.switch_state_new<Work>();
}

}
