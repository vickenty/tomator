#pragma once
#include <assert.h>
#include "shared_ptr.h"

template<typename state_base> class Context
{
public:
	typedef shared_ptr<state_base> state_ptr;

	template<typename state_class, typename... param_types> shared_ptr<state_class> create_state(param_types... params) {
		return shared_ptr<state_class>(new state_class(*this, params...));
	}

	void init_state(state_ptr state) {
		assert(m_states.empty());
		m_states.push_back(state);
		active_state()->enter();
	}

	template<typename state_type, typename... param_types> void init_state_new(param_types... params) {
		init_state(create_state<state_type>(params...));
	}

	void switch_state(state_ptr state) {
		active_state()->leave();
		m_states.pop_back();
		m_states.push_back(state);
		active_state()->enter();
	}

	template<typename state_type, typename... param_types> void switch_state_new(param_types... params) {
		switch_state(create_state<state_type>(params...));
	}

	void push_state(state_ptr state) {
		active_state()->leave();
		m_states.push_back(state);
		active_state()->enter();
	}

	template<typename state_type, typename... param_types> void push_state_new(param_types... params) {
		push_state(create_state<state_type>(params...));
	}

	void pop_state() {
		assert(m_states.size() > 1);
		active_state()->leave();
		m_states.pop_back();
		active_state()->enter();
	}

	template<typename return_type, typename event_type> return_type send(event_type& event) {
		return active_state()->handle(event);
	}

	state_ptr active_state() {
		assert(!m_states.empty());
		return m_states.back();
	}

private:
	std::vector<state_ptr> m_states;
	state_ptr next_state;
};
