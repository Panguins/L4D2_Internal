#include "hooks.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"
#include "cheat.h"
#include <future>

bool __fastcall hooks::createmove( void* ecx, void* edx, float input_sample, CUserCmd* cmd )
{
	uintptr_t ebp = uintptr_t( _AddressOfReturnAddress( ) ) - 4;

	static auto o_createmove = g_l4d2.clientmode_hook->get_original< decltype( &hooks::createmove ) >( 27 );
	bool original = o_createmove( ecx, edx, input_sample, cmd );

	// calls come from own CL_Move spam
	if ( g_cheat.speedhack.m_cm_dummy_call )
		return original;

	// dummy call from CInput::ExtraMouseSample
	if ( !cmd->command_number || !cmd->tick_count || g_ctx.panic )
		return original;

	// we will return false, so lets mimic what game does
	if ( original )
		g_l4d2.engine->SetViewAngles( cmd->viewangles );

	// random seed is not updated yet
	cmd->random_seed = MD5_PseudoRandom( cmd->command_number ) & 0x7fffffff;

	// update context
	g_ctx.m_cmd = cmd;
	g_ctx.m_local_index = g_l4d2.engine->GetLocalPlayer( );
	g_ctx.m_local = g_l4d2.entity_list->GetClientEntity( g_ctx.m_local_index );

	if ( g_ctx.m_local && g_ctx.m_local->is_alive( ) /* gp_GameInterFaces->EngineDLLInterFace->IsInGame( ) */ )
	{
		g_cheat.movement.autostrafe( );
		g_cheat.movement.bunnyhop( );

		g_cheat.prediction.start( );
		{
			g_cheat.fakelag.setup( ebp );

			if ( g_ctx.m_settings.misc_rapidfire && g_ctx.m_local->get_group() == GROUP_PLAYER )
			{
				if ( g_ctx.m_cmd->buttons & IN_ATTACK && g_ctx.m_local->get_weapon( ) && !g_ctx.m_local->can_attack( ) )
					g_ctx.m_cmd->buttons &= ~IN_ATTACK;
			}

			g_cheat.movement.fix_start( );

			g_cheat.aimbot.init( );
			g_cheat.accuracy.init( );

			g_cheat.movement.fix_end( );

			g_cheat.fakelag.init( );
		}
		g_cheat.prediction.end( );


		g_ctx.m_angles = g_ctx.m_cmd->viewangles;
	}

	return false;
}


