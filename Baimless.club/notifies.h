#pragma once
#include "Render.h"
#include "Utils/GlobalVars.h"
#include <vector>
#include <string>
enum notify_state_s : int
{
	info_state = 0,
	success_state,
	warning_state,
	danger_state,
	debug_state
};

struct notify_data_t
{
	std::string message;
	notify_state_s state;
	unsigned __int64 end_time;
};

namespace notifies
{

	void handle();
	void push(const std::string& message, notify_state_s state = notify_state_s::info_state);
	void push(const std::string& english, const std::string& russian, notify_state_s state = notify_state_s::info_state);
}