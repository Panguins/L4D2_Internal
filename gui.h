#pragma once

#include <vector>
#include <string>
#include <iterator>
#include "renderer.h"

class c_gui
{
	enum tabs_t
	{
		TABS_AIMBOT,
		TABS_VISUALS,
		TABS_MISC,
		TABS_CONFIG
	};

	std::vector<std::string> m_tabs =
	{
		"aimbot",
		"visuals",
		"misc",
		"config"
	};

	void tab_aimbot( );
	void tab_visuals( );
	void tab_misc( );
	void tab_config( );
public:
	void draw( );
}; extern c_gui gui;
