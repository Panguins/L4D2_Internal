#include "hooks.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"
#include "cheat.h"
#include <future>

void __fastcall hooks::lock_cursor( void* ecx, void* edx )
{
	static auto o_lock_cursor = g_l4d2.surface_hook->get_original< decltype( &hooks::lock_cursor ) >( 59 );

	if ( g_ctx.m_menu_open )
		g_l4d2.surface->UnlockCursor( );
	else
		o_lock_cursor( ecx, nullptr );
}