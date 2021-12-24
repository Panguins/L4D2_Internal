#include "aimbot.h"
#include "context.h"
#include "player.h"
#include "input.h"
#include "l4d2.h"
#include "cheat.h"

bool c_aimbot::should_run( )
{
	if ( !g_ctx.m_settings.rage_aimbot_enabled )
		return false;

	if ( !g_ctx.m_local || !g_ctx.m_local->is_alive( ) )
		return false;

	return true;
}

void c_aimbot::init( )
{
	m_aim_point = vec3( );

	if ( !should_run( ) )
		return;

	HANDLE weapon_handle = g_ctx.m_local->weapon( );
	if ( !weapon_handle )
		return;

	CBaseWeapon* weapon = (CBaseWeapon*)g_l4d2.entity_list->GetClientEntityFromHandle( weapon_handle );
	if ( !weapon || !weapon->is_valid_aim_weapon( ) )
		return;

	// Bullet Avoider System ( tm )
	if ( g_ctx.m_settings.rage_anti_aim )
	{
		bool dont_dreidel = ( g_ctx.m_cmd->buttons & IN_USE ) || ( g_ctx.m_cmd->buttons & IN_ATTACK ) || ( g_ctx.m_cmd->buttons & IN_ATTACK2 );
		if ( !dont_dreidel )
		{
			static float yaw = 0.f;
			yaw += g_l4d2.globals->interval_per_tick * 979.f * 2.f;

			if ( yaw > 360.f )
				yaw -= 360.f;

			g_ctx.m_cmd->viewangles = vec3( 76.f, yaw, 0.f );
		}
	}

	// cant attack or no aimkey, dont run aimbot!
	if ( !g_ctx.m_local->can_attack( ) || !g_input.is_key_pressed( g_ctx.m_settings.rage_key ) )
		return;

	// find target
	int target_id = get_target( );
	if ( target_id == 979 )
		return;

	// fuck safety
	CBaseEntity* target = g_l4d2.entity_list->GetClientEntity( target_id );
	target->get_hitbox_position( m_aim_point, target->get_head_bone_id( ) );

	// set angles
	g_ctx.m_cmd->viewangles = math::vector_angles( g_ctx.m_local->view_position( ), m_aim_point );

	// revolutionary auto shoot
	if ( g_ctx.m_settings.rage_auto_shoot )
	{
		g_ctx.m_cmd->buttons |= IN_ATTACK;
		m_last_target_time[target_id] = g_l4d2.globals->curtime;
	}

	switch ( g_ctx.m_settings.rage_silent)
	{
	case 0:
		{
			g_l4d2.engine->SetViewAngles( g_ctx.m_cmd->viewangles );
		}
	case 2:
		{
			g_cheat.fakelag.choke( );
		}
	}
}

int c_aimbot::get_target( )
{
	int best_target = 979;
	int best_target_group = GROUP_INFECTED;
	float best_target_dist = g_ctx.m_settings.rage_selection == 0 ? ( g_ctx.m_settings.rage_fov * 0.5f ) : 99999.f;

	vec3 local_pos = g_ctx.m_local->view_position( );
	int local_team = g_ctx.m_local->team( );
	int max_entities = g_l4d2.entity_list->GetHighestEntityIndex( );

	Ray_t ray;
	trace_t tr;
	CTraceFilter filter;
	filter.pSkip = g_ctx.m_local;

	for ( int i = 0; i < max_entities; i++ )
	{
		CBaseEntity* entity = g_l4d2.entity_list->GetClientEntity( i );
		if ( !entity || entity == g_ctx.m_local || !entity->is_valid( ) )
			continue;

		if ( entity->team( ) == local_team )
			continue;

		if ( !entity->is_alive( ) )
			continue;

		int group = entity->get_group( );

		// skip target if we dont want to shoot at this group
		if ( !should_target_group( group ) )
			continue;

		// skip spawning players
		if ( entity->is_player( ) && entity->ghost( ) )
			continue;

		// skip target if we already have a higher threat target with threat selection
		if ( g_ctx.m_settings.rage_selection == 2 && best_target_group > group )
			continue;

		// skip regular infected if we already shot at them, real ghetto but it works
		if ( group == GROUP_INFECTED && std::abs( g_l4d2.globals->curtime - m_last_target_time[i] ) < 0.25f )
			continue;

		// do not disturb lily
		if ( entity->is_witch( ) && ( entity->sequence( ) == 4 || entity->sequence( ) == 27 ) )
			continue;

		vec3 head_pos;
		uint8_t bone_id = entity->get_head_bone_id( );
		if ( !entity->get_hitbox_position( head_pos, bone_id ) )
			continue;

		ray.Init( local_pos, head_pos );
		g_l4d2.trace->TraceRay( ray, MASK_SHOT, &filter, &tr );

		if ( tr.m_pEnt == entity || tr.fraction >= 0.97f )
		{
			vec3 angle = math::vector_angles( local_pos, head_pos );
			vec3 delta = angle - g_ctx.m_cmd->viewangles;
			delta.sanitize( );

			float fov_dist   = delta.length2d( );

			if ( fov_dist > g_ctx.m_settings.rage_fov )
				continue;

			float dist = g_ctx.m_settings.rage_selection == 0 ? fov_dist : local_pos.dist( head_pos );

			bool group_priority = g_ctx.m_settings.rage_selection == 2 && group > best_target_group;
			bool distance_priority = best_target_dist > dist;

			if ( group_priority || distance_priority )
			{
				best_target_dist = dist;
				best_target_group = group;
				best_target = i;
			}
		}
	}

	return best_target;
}

bool c_aimbot::should_target_group( int group )
{
	if ( group == GROUP_PLAYER && g_ctx.m_settings.rage_filter_player )
		return true;

	if ( group == GROUP_BOSS && g_ctx.m_settings.rage_filter_boss )
		return true;

	if ( group == GROUP_SPECIAL && g_ctx.m_settings.rage_filter_special )
		return true;

	if ( group == GROUP_INFECTED && g_ctx.m_settings.rage_filter_infected )
		return true;

	return false;
}