#include "fakelag.h"
#include "context.h"
#include "player.h"
#include "cheat.h"

void c_fakelag::setup(uintptr_t ebp)
{
	m_should_choke = false;
	m_send_packet = (bool*)( ***(uintptr_t***)ebp - 33 );
}

void c_fakelag::init( )
{
	if ( m_should_choke )
	{
		*m_send_packet = false;
	}
	else if ( g_ctx.m_settings.rage_fakelag )
	{
		if ( m_choked_ticks < g_ctx.m_settings.rage_fakelag_amount )
			*m_send_packet = false;

		if ( g_ctx.m_cmd->buttons & IN_ATTACK && g_ctx.m_local->can_attack( ) )
			*m_send_packet = true;

		if ( g_cheat.speedhack.m_is_speedhacking )
			*m_send_packet = true;
	}

	if ( *m_send_packet )
	{
		m_choked_ticks = 0;
	}
	else
	{
		m_choked_ticks++;
	}
}
