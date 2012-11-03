#pragma once
#include <sigc++/sigc++.h>
#include "context.h"

namespace Events {
	struct Snooze {};
	struct Pause {};
	struct Skip {};
	struct GetLabel {};
};

namespace States {

class Base;
typedef ::Context<Base> Context;

class Base
{
public:
	Base(Context& context) : m_context(context) {};

	virtual void handle(Events::Snooze&) {}
	virtual void handle(Events::Pause&) {}
	virtual void handle(Events::Skip&) {}
	virtual Glib::ustring handle(Events::GetLabel&) { return ""; }

	inline sigc::signal<void, Base*> signal_state_enter() {
		return m_state_enter_slot;
	}

	inline sigc::signal<void, Base*> signal_state_leave() {
		return m_state_leave_slot;
	}

	virtual void enter();
	virtual void leave();

protected:
	Context& m_context;

	sigc::signal<void, Base*> m_state_enter_slot;
	sigc::signal<void, Base*> m_state_leave_slot;
};

class Pause : public Base
{
public:
	Pause(Context& context) : Base(context) {}

	virtual void handle(Events::Pause&);
	virtual Glib::ustring handle(Events::GetLabel&);
};

class Work : public Base
{
public:
	Work(Context& context) : Base(context) {}
	virtual void handle(Events::Pause&);
	virtual Glib::ustring handle(Events::GetLabel&);
};

}