#pragma once

class Configuration
{
public:
	Configuration()
		: m_work_time(25)
		, m_rest_time(5)
		, m_snooze_time(5)
	{};

	guint get_work_time() {
		return m_work_time;
	}

	void set_work_time(guint value) {
		m_work_time = value;
	}

	guint get_rest_time() {
		return m_rest_time;
	}

	void set_rest_time(guint value) {
		m_rest_time = value;
	}

	guint get_snooze_time() {
		return m_snooze_time;
	}

	void set_snooze_time(guint value) {
		m_snooze_time = value;
	}

	void load();
	void save();

private:
	guint m_work_time;
	guint m_rest_time;
	guint m_snooze_time;
};
