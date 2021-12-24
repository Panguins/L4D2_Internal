#pragma once
#include "player.h"

class c_esp
{
	bool should_draw_group( group_t group );

	void draw_player( CBaseEntity* ent );
	void draw_skeleton( CBaseEntity* ent );
	void draw_health( CBaseEntity* ent, vec2 bot, vec2 top, uint8_t alpha, color_t outline_color );
public:
	void init( );
};
