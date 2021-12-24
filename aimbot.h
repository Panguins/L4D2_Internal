#pragma once

class c_aimbot
{
	float m_last_target_time[4096]{ };

	bool should_target_group( int group );
	bool should_run( );

	int get_target( );
public:
	void init( );

	vec3 m_aim_point{ };
};