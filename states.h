#pragma once
#include <sigc++/sigc++.h>
#include "context.h"

namespace Events {
	struct Base {
		typedef void return_type;
	};

	struct Snooze : public Base {};
	struct Pause : public Base {};
	struct Skip : public Base {};

	struct GetLabel {
		typedef Glib::ustring return_type;
	};

	struct GetIcon : public GetLabel {};
	struct GetClock : public GetLabel {};
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
	virtual Glib::ustring handle(Events::GetLabel&) { Events::GetClock ev; return handle(ev); }
	virtual Glib::ustring handle(Events::GetIcon&) { return "tomator-panel"; }
	virtual Glib::ustring handle(Events::GetClock&) { return ""; }

	inline sigc::signal<void, Base*> signal_state_enter() {
		return m_state_enter_slot;
	}

	inline sigc::signal<void, Base*> signal_state_leave() {
		return m_state_leave_slot;
	}

	virtual void enter();
	virtual void leave();

	Glib::ustring format_time_label(gulong);

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
	virtual Glib::ustring handle(Events::GetClock&);
	virtual Glib::ustring handle(Events::GetIcon&);
};

class TimerState : public Base
{
public:
	TimerState(Context& context, guint msec = 0) : Base(context), m_time_msec(msec) {}

	virtual void handle(Events::Pause&);
	virtual Glib::ustring handle(Events::GetClock&);

	virtual void enter();
	virtual void leave();

	virtual bool on_timeout() { return false; }
	virtual guint default_timeout() { return 0; }

	guint elapsed();

private:
	guint m_time_msec;
	sigc::connection m_timer_conn;
	Glib::Timer m_timer;
};

class Work : public TimerState
{
public:
	Work(Context& context, guint msec = 0) : TimerState(context, msec) {}
	virtual void handle(Events::Skip&);

	virtual bool on_timeout();
	virtual guint default_timeout();
};

class RestPending : public Base
{
public:
	RestPending(Context& context) : Base(context) {}
	virtual void handle(Events::Skip&);
	virtual Glib::ustring handle(Events::GetClock&);

	virtual void enter();
};

class Rest : public TimerState
{
public:
	Rest(Context& context, int msec = 0) : TimerState(context, msec) {}
	virtual void handle(Events::Skip&);

	virtual bool on_timeout();
	virtual guint default_timeout();
};

class WorkPending : public Base
{
public:
	WorkPending(Context& context) : Base(context) {}
	virtual void handle(Events::Skip&);
	virtual Glib::ustring handle(Events::GetClock&);

	virtual void enter();
};

}
