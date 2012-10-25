#pragma once
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

	virtual void enter() {};
	virtual void leave() {};

	virtual void handle(Events::Snooze&) {}
	virtual void handle(Events::Pause&) {}
	virtual void handle(Events::Skip&) {}
	virtual Glib::ustring handle(Events::GetLabel&) { return ""; }

protected:
	Context& m_context;
};

class Pause : public Base
{
public:
	Pause(Context& context) : Base(context) {}
	virtual void handle(Events::Pause&);
	virtual void enter();
	virtual void leave();
};

class Work : public Base
{
public:
	Work(Context& context) : Base(context) {}
	virtual void handle(Events::Pause&);
	virtual void enter();
	virtual void leave();
};

}
