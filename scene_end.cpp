#include "hooks.h"
#include "l4d2.h"

void __fastcall hooks::scene_end( void* ecx, void* edx )
{
	static auto o_scene_end = g_l4d2.render_view_hook->get_original< decltype( &hooks::scene_end ) >( 9 );

	o_scene_end( ecx, nullptr );
}
