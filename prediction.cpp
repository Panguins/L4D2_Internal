#include "prediction.h"
#include "l4d2.h"
#include "context.h"
#include "player.h"

void c_prediction::start( )
{
	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return;

	CMoveData movedata{ }; // s/o to estroterik
	memset( &movedata, 0, sizeof( movedata ) );
	movedata.m_nButtons = g_ctx.m_cmd->buttons;

	int original_tickbase = g_ctx.m_local->tickbase( );
	int original_flags = g_ctx.m_local->flags( );
	original_frametime = g_l4d2.globals->frametime;
	original_curtime = g_l4d2.globals->curtime;

	g_l4d2.globals->frametime = TICK_INTERVAL( );
	g_l4d2.globals->curtime = g_ctx.m_local->tickbase( ) * TICK_INTERVAL( );

	g_l4d2.game_movement->StartTrackPredictionErrors( g_ctx.m_local );
	g_l4d2.move_helper->SetHost( g_ctx.m_local );
	g_l4d2.prediction->SetupMove( g_ctx.m_local, g_ctx.m_cmd, g_l4d2.move_helper, &movedata );
	g_l4d2.game_movement->ProcessMovement( g_ctx.m_local, &movedata );
	g_l4d2.prediction->FinishMove( g_ctx.m_local, g_ctx.m_cmd, &movedata );

	g_ctx.m_local->tickbase( ) = original_tickbase;
	g_ctx.m_local->flags( ) = original_flags;
}

void c_prediction::end( )
{
	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return;

	g_l4d2.game_movement->FinishTrackPredictionErrors( g_ctx.m_local );

	g_l4d2.globals->frametime = original_frametime;
	g_l4d2.globals->curtime = original_curtime;

	g_l4d2.move_helper->SetHost( nullptr );
}

