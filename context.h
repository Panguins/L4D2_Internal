#pragma once
#include "settings.h"
#include "gui.h"
#include "l4d2.h"

#define anti_rin( items )\
 static bool init = true; \
if ( init ) \
{ \
	items; \
	init = false;\
} \

class c_context
{
public:
	bool      panic{ };
	HMODULE   instance{ };

	c_gui                        m_gui;
	c_render                     m_render;
	c_settings                   m_settings;
	std::vector< std::string >   m_cfg_list;
	int                          m_current_cfg;

	bool m_menu_open;
	int m_screen_w;
	int m_screen_h;

	int            m_local_index{ };
	CBaseEntity*   m_local{ };
	CUserCmd*      m_cmd{ };
	vec3           m_angles{ };

	__forceinline bool w2s( const vec3& world, vec2& screen )
	{
		vec3 transform;

		auto& matrix = g_l4d2.engine->WorldToScreenMatrix( );

		transform.z = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];
		if ( transform.z < 0.001f )
			return false;

		transform.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		transform.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];

		transform.x /= transform.z;
		transform.y /= transform.z;

		screen.x = ( m_screen_w * 0.5f ) + ( transform.x * m_screen_w ) * 0.5f;
		screen.y = ( m_screen_h * 0.5f ) - ( transform.y * m_screen_h ) * 0.5f;

		return true;
	}
}; extern c_context g_ctx;