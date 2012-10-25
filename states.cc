#include <iostream>
#include <gtkmm.h>
#include "states.h"

namespace States {

void Pause::enter()
{
	std::cout << "pause enter" << std::endl;
}

void Pause::leave()
{
	std::cout << "pause leave" << std::endl;
}

void Pause::handle(Events::Pause&)
{
	m_context.pop_state();
}

void Work::enter()
{
	std::cout << "work enter" << std::endl;
}

void Work::leave()
{
	std::cout << "work leave" << std::endl;
}


void Work::handle(Events::Pause&)
{
	m_context.push_state_new<Pause>();
}

}
