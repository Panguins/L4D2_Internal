#include "hooks.h"
#include "l4d2.h"
#include "context.h"

void __fastcall hooks::on_screen_size_changed( void* ecx, void* edx, int old_width, int old_height )
{
	static auto o_on_screen_size_changed = g_l4d2.surface_hook->get_original< decltype( &hooks::on_screen_size_changed ) >( 108 );

	g_ctx.m_render.fonts_initialized = false;

	o_on_screen_size_changed( ecx, nullptr, old_width, old_height );
}
