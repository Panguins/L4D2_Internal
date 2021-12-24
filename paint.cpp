#include "hooks.h"
#include "l4d2.h"
#include "context.h"
#include "cheat.h"

void __fastcall hooks::paint( void* ecx, void* edx, int mode )
{
	static auto o_paint = g_l4d2.enginevgui_hook->get_original< decltype( &hooks::paint ) >( 14 );
	o_paint( ecx, nullptr, mode );

	g_l4d2.engine->GetScreenSize( g_ctx.m_screen_w, g_ctx.m_screen_h );

	g_ctx.m_render.start( );
	{
		g_cheat.esp.init( ); // init bruv
		g_ctx.m_gui.draw( );
	}
	g_ctx.m_render.finish( );
}
