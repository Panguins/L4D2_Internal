#include "movement.h"
#include "context.h"
#include "player.h"

void c_movement::bunnyhop()
{
	if ( !g_ctx.m_settings.misc_bhop )
		return;

	if ( g_ctx.m_cmd->buttons & IN_JUMP && !( g_ctx.m_local->flags( ) & FL_ONGROUND ) )
		g_ctx.m_cmd->buttons &= ~IN_JUMP;
}


void c_movement::autostrafe( )
{
	if ( !g_ctx.m_settings.misc_autostrafe )
		return;

	if ( !( g_ctx.m_cmd->buttons & IN_JUMP ) )
		return;

	// we are not aimware, no ground strafer!
	if ( g_ctx.m_local->flags( ) & FL_ONGROUND )
		return;

	if ( g_ctx.m_cmd->mousedx )
	{
		g_ctx.m_cmd->sidemove = g_ctx.m_cmd->mousedx < 0 ? -450.f : 450.f;
	}
	else
	{
		g_ctx.m_cmd->sidemove = ( g_ctx.m_cmd->command_number % 2 ) == 0 ? -450.f : 450.f;
	}
}

void c_movement::fix_start( )
{
	old_viewangles = g_ctx.m_cmd->viewangles;
	old_movement = vec3( g_ctx.m_cmd->forwardmove, g_ctx.m_cmd->sidemove, g_ctx.m_cmd->upmove );
}

void c_movement::fix_end( )
{
	vec3 new_viewangles = g_ctx.m_cmd->viewangles;

	if ( new_viewangles == old_viewangles )
		return;

	new_viewangles.sanitize( );

	float old_yaw = old_viewangles.y < 0.f ? old_viewangles.y + 360.f : old_viewangles.y;
	float new_yaw = new_viewangles.y < 0.f ? new_viewangles.y + 360.f : new_viewangles.y;

	float yaw_delta = new_yaw < old_yaw ? 360.f - abs( new_yaw - old_yaw ) : abs( old_yaw - new_yaw );

	g_ctx.m_cmd->forwardmove = cos( yaw_delta * PIRAD ) * old_movement.x + cos( ( yaw_delta + 90.f ) * PIRAD ) * old_movement.y;
	g_ctx.m_cmd->sidemove = sin( yaw_delta * PIRAD ) * old_movement.x + sin( ( yaw_delta + 90.f ) * PIRAD ) * old_movement.y;

	g_ctx.m_cmd->forwardmove = std::clamp<float>( g_ctx.m_cmd->forwardmove, -450.f, 450.f );
	g_ctx.m_cmd->sidemove = std::clamp<float>( g_ctx.m_cmd->sidemove, -450.f, 450.f );
}