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

Glib::ustring Work::handle(Events::GetLabel&)
{
	return "WORKING";
}

}
