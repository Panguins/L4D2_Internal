#include "speedhack.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"

void c_speedhack::run( )
{
	m_is_speedhacking = false;
	m_cm_dummy_call = false;

	if ( !g_l4d2.engine->IsInGame( ) )
		return;

	if ( !g_ctx.m_settings.misc_speedhack_enabled || !g_input.is_key_pressed( g_ctx.m_settings.misc_speedhack_key( ) ) )
		return;

	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return;

	static uintptr_t some_secret_function_nobody_has_ever_seen_before_in_the_history_of_mankind = uintptr_t( GetModuleHandleA( "engine.dll" ) ) + 0x7C530;

	int tickbase = g_ctx.m_local->tickbase( );
	int factor = int( g_ctx.m_settings.misc_speedhack_factor( ) );

	m_is_speedhacking = true;

	m_cm_dummy_call = true;

	for ( int i = 0; i < factor; i++ )
		( ( void( __cdecl* )( float, bool ) )( some_secret_function_nobody_has_ever_seen_before_in_the_history_of_mankind ) )( g_l4d2.globals->interval_per_tick, false );

	m_cm_dummy_call = false;

	g_ctx.m_local->tickbase( ) = tickbase;
}
