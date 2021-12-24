#include "hooks.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"
#include "cheat.h"
#include <future>

void __fastcall hooks::override_view( void* ecx, void* edx, CViewSetup* view )
{
	static auto o_override_view = g_l4d2.clientmode_hook->get_original< decltype( &hooks::override_view ) >( 19 );

	// thanks lily
	if ( g_ctx.m_settings.visuals_thirdperson )
	{
		auto set_thirdperson_state = []( const bool& state, const bool& alive )
		{
			if ( alive )
			{
				static vec3 player_view;
				float dist = 50.f + float( g_ctx.m_settings.visuals_thirdperson_dist );

				g_l4d2.engine->GetViewAngles( player_view );
				g_l4d2.input->m_fCameraInThirdPerson = state;
				g_l4d2.input->m_vecCameraOffset = state ? vec3( player_view.x, player_view.y, dist ) : vec3( player_view.x, player_view.y, player_view.z );
			}
			else
			{
				g_l4d2.input->m_fCameraInThirdPerson = false;
			}
		};

		static bool on = false;
		static bool is_clicking = false;

		// toggle
		{
			if ( g_input.is_key_pressed( g_ctx.m_settings.visuals_thirdperson_key ) )
			{
				if ( !is_clicking )
					on = !on;
				is_clicking = true;
			}
			else
				is_clicking = false;
		}

		// update thirdperson state
		set_thirdperson_state( on, g_ctx.m_local->is_alive( ) );

		if ( g_ctx.m_local->is_alive( ) && on )
		{
			constexpr   float   cam_hull_offset{ 16.0f };
			const       vec3    cam_hull_min( -cam_hull_offset, -cam_hull_offset, -cam_hull_offset );
			const       vec3    cam_hull_max( cam_hull_offset, cam_hull_offset, cam_hull_offset );
			vec3                cam_forward, origin = g_ctx.m_local->view_position( );

			Ray_t ray;
			trace_t tr;
			CTraceFilter filter;
			filter.pSkip = g_ctx.m_local;

			cam_forward = math::angle_vectors( vec3( g_l4d2.input->m_vecCameraOffset.x, g_l4d2.input->m_vecCameraOffset.y, 0.0f ) );

			ray.Init( origin, origin - ( cam_forward * g_l4d2.input->m_vecCameraOffset.z ), cam_hull_min, cam_hull_max );

			g_l4d2.trace->TraceRay( ray, MASK_SOLID & ~CONTENTS_MONSTER, &filter, &tr );

			if ( tr.fraction < 1.0f )
				g_l4d2.input->m_vecCameraOffset.z *= tr.fraction;
		}
	}


	if ( g_ctx.m_settings.visuals_view_fov )
		view->fov = g_ctx.m_settings.visuals_view_fov;

	o_override_view( ecx, edx, view );
}