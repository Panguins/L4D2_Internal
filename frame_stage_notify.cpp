#include "hooks.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"
#include "cheat.h"

void __fastcall hooks::frame_stage_notify( void* ecx, void* edx, ClientFrameStage_t stage )
{
	static auto o_frame_stage_notify = g_l4d2.client_hook->get_original< decltype( &hooks::frame_stage_notify ) >( 34 );

	o_frame_stage_notify( ecx, nullptr, stage );

	if ( stage == FRAME_NET_UPDATE_END )
		g_cheat.speedhack.run( );
}
