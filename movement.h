#pragma once

class c_movement
{
	vec3 old_viewangles;
	vec3 old_movement;
public:
	void bunnyhop( );
	void autostrafe( );

	void fix_start( );
	void fix_end( );
};