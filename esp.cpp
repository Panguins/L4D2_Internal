#include "esp.h"
#include "context.h"
#include "l4d2.h"
#include "player.h"

#include <functional>
#include "cheat.h"

struct esp_ent_t
{
	CBaseEntity* ent;
	float dist;
};

void c_esp::init()
{
	if ( !g_ctx.m_settings.visuals_active )
		return;

	if ( !g_ctx.m_local || !g_l4d2.engine->IsConnected( ) || !g_l4d2.engine->IsInGame( ) )
		return;

	if ( g_ctx.m_settings.visuals_crosshair && g_ctx.m_local->is_alive( ) )
	{
		// yes i reversed aimware for how to draw a cross
		vec2 center;

		if ( g_cheat.aimbot.m_aim_point.IsZero( ) || !g_ctx.w2s( g_cheat.aimbot.m_aim_point, center ) )
		{
			vec3 local_pos = g_ctx.m_local->view_position( );
			vec3 angles = g_l4d2.engine->GetViewAngles( );

			vec3 forward = math::angle_vectors( angles );

			vec3 end_pos = local_pos + forward * 1024.f;

			Ray_t ray;
			trace_t tr;
			CTraceFilter filter;
			filter.pSkip = g_ctx.m_local;

			ray.Init( local_pos, end_pos );
			g_l4d2.trace->TraceRay( ray, MASK_SHOT, &filter, &tr );

			if ( !g_ctx.w2s( tr.endpos, center ) )
				center = vec2( static_cast< float >( g_ctx.m_screen_w / 2 ), static_cast < float >( g_ctx.m_screen_h / 2 ) );		
		}

		/*
		// the communist nigga
		g_ctx.m_render.line( color_t( 240, 0, 0 ), center.x, center.y + 12.0f, center.x, center.y - 12.0f );
		g_ctx.m_render.line( color_t( 240, 0, 0 ), center.x + 12.0f, center.y, center.x - 12.0f, center.y );

		// the not communist nigga
		g_ctx.m_render.line( color_t( 255, 255, 255 ), center.x, center.y + 7.0f, center.x, center.y - 7.0f );
		g_ctx.m_render.line( color_t( 255, 255, 255 ), center.x + 7.0f, center.y, center.x - 7.0f, center.y );
		*/

		g_ctx.m_render.filled_rect( color_t( 0, 0, 0, 200 ), center.x - 3, center.y - 3, center.x + 3, center.y + 3 );
		g_ctx.m_render.filled_rect( color_t( 255, 255, 255, 255 ), center.x - 2, center.y - 2, center.x + 2, center.y + 2 );
	}

	std::vector<esp_ent_t> entities;
	int entity_count = g_l4d2.entity_list->GetHighestEntityIndex( );
	vec3 local_origin = g_ctx.m_local->origin( );

	// fill entity list
	for ( int i = 1; i < entity_count; i++ )
	{
		CBaseEntity* ent = g_l4d2.entity_list->GetClientEntity( i );

		if ( !ent || ent == g_ctx.m_local )
			continue;

		if ( ent->IsDormant( ) )
			continue;

		if ( ent->is_alive() && ent->is_valid( ) )
		{
			if ( g_ctx.m_settings.visuals_filter_enemies_only && ent->team( ) == g_ctx.m_local->team( ) )
				continue;

			float dist = local_origin.dist( ent->origin( ) );

			entities.push_back( { ent, dist } );
		}
	}

	// return if we didnt find any
	if ( entities.empty( ) )
		return;

	// sort em with a disgusting lambda BLEH
	std::function<bool( const esp_ent_t&, const esp_ent_t& )> dist_sort_fn;
	dist_sort_fn = []( const esp_ent_t& a, const esp_ent_t& b ) -> bool
	{
		return a.dist > b.dist;
	};

	std::sort( entities.begin( ), entities.end( ), dist_sort_fn );

	for ( size_t i = 0; i < entities.size(); i++)
	{
		draw_player( entities.at( i ).ent );
	}
}

void c_esp::draw_player( CBaseEntity* ent )
{
	group_t group = ent->get_group( );
	if ( !should_draw_group( group ) )
		return;

	// prepare box
	vec2 bottom, top;
	vec3 bottom_world = ent->origin( );
	vec3 top_world = ent->origin( );

	bottom_world.z += ent->mins().z;
	top_world.z += ent->maxs( ).z;

	if ( !g_ctx.w2s( bottom_world, bottom ) || !g_ctx.w2s( top_world, top ) )
		return;

	// yeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
	float width_fraction = std::abs( ent->maxs( ).x - ent->mins( ).x ) / std::abs( ent->maxs( ).z - ent->mins( ).z );

	float h = std::abs( top.y - bottom.y );
	float w = h * width_fraction;

	float x = top.x - w * 0.5f;
	float y = top.y;

	// prepare color and name
	uint8_t alpha = 255;
	color_t color;
	color_t outline_color = color_t( 0, 0, 0, 175 );
	std::wstring name;
	bool should_draw_health{};

	switch ( ent->get_group( ) )
	{
		case GROUP_PLAYER:
		{
			name = ent->is_player( ) ? ent->player_name_wide( ) : ent->network_name_wide( );
			color = g_ctx.m_settings.visuals_color_players( );
			color.a( alpha );
			should_draw_health = true;
			break;
		}

		case GROUP_BOSS:
		{
			name = ent->is_player( ) ? ent->player_name_wide( ) : ent->network_name_wide( );
			color = g_ctx.m_settings.visuals_color_boss( );
			color.a( alpha );
			should_draw_health = true;
			break;
		}

		case GROUP_SPECIAL:
		{
			name = ent->is_player( ) ? ent->player_name_wide( ) : ent->network_name_wide( );
			color = g_ctx.m_settings.visuals_color_special( );
			color.a( ent->is_player( ) && ent->ghost( ) ? alpha / 2 : alpha );
			should_draw_health = true;
			break;
		}

		default:
		{
			name = wxor( "Infected" );
			color = g_ctx.m_settings.visuals_color_infected( );
			color.a( alpha );
			should_draw_health = false;
		}
	}

	if ( g_ctx.m_settings.visuals_skeleton )
		draw_skeleton( ent );

	if ( g_ctx.m_settings.visuals_health && should_draw_health )
	{
		draw_health( ent, vec2( x, y + h ), vec2( x, y ), alpha, outline_color );
	}

	if ( g_ctx.m_settings.visuals_box )
	{
		g_ctx.m_render.rect( outline_color, x - 1.f, y - 1.f, x + w + 1.f, y + h + 1.f );
		g_ctx.m_render.rect( outline_color, x + 1.f, y + 1.f, x + w - 1.f, y + h - 1.f );
		g_ctx.m_render.rect( color, x, y, x + w, y + h );
	}

	if ( g_ctx.m_settings.visuals_name && !name.empty( ) )
	{
		g_ctx.m_render._string( font_t::font_esp, color_t( 0, 0, 0, 0 ), top.x + 1.f, top.y - 11.f, true, name.c_str( ) );
		g_ctx.m_render._string( font_t::font_esp, color_t( 255, 255, 255, 255 ), top.x, top.y - 12.f, true, name.c_str( ) );
	}
}

bool c_esp::should_draw_group( group_t group )
{
	if ( group == GROUP_PLAYER && g_ctx.m_settings.visuals_filter_players )
		return true;

	if ( group == GROUP_BOSS && g_ctx.m_settings.visuals_filter_boss )
		return true;

	if ( group == GROUP_SPECIAL && g_ctx.m_settings.visuals_filter_special )
		return true;

	if ( group == GROUP_INFECTED && g_ctx.m_settings.visuals_filter_infected )
		return true;

	return false;
}

void c_esp::draw_skeleton( CBaseEntity* ent )
{
	studiohdr_t* hdr = g_l4d2.model_info->GetStudioModel( ent->GetModel( ) );
	if ( !hdr )
		return;

	matrix3x4 matrix[128];
	if ( !ent->SetupBones( matrix, 128, 0x100, g_l4d2.globals->curtime ) )
		return;

	g_l4d2.surface->DrawSetColor( 255, 255, 255, 255 );

	for ( int i = 0; i < hdr->numbones; i++ )
	{
		auto bone = hdr->GetBone( i );
		if ( !bone || !( bone->flags & 256 ) || bone->parent == -1 )
			continue;

		vec2 bone_pos, bone_parent;

		if ( !g_ctx.w2s( vec3( matrix[i][0][3], matrix[i][1][3], matrix[i][2][3] ), bone_pos ) )
			continue;

		if ( !g_ctx.w2s( vec3( matrix[bone->parent][0][3], matrix[bone->parent][1][3], matrix[bone->parent][2][3] ), bone_parent ) )
			continue;

		g_l4d2.surface->DrawLine( bone_pos.x, bone_pos.y, bone_parent.x, bone_parent.y );

		g_l4d2.surface->DrawLine( bone_pos.x, bone_pos.y + 1.f, bone_parent.x, bone_parent.y + 1.f);
		g_l4d2.surface->DrawLine( bone_pos.x + 1.f, bone_pos.y, bone_parent.x + 1.f, bone_parent.y );
		g_l4d2.surface->DrawLine( bone_pos.x + 1.f, bone_pos.y + 1.f, bone_parent.x + 1.f, bone_parent.y + 1.f );
	}
}

void c_esp::draw_health( CBaseEntity* ent, vec2 bot, vec2 top, uint8_t alpha, color_t outline_color )
{
	float fraction = ent->health_fraction( );
	color_t color( 255.f - ( 255.f * fraction ), 255.f * fraction, 0, alpha );

	float height = bot.y - top.y;
	float pos = height - ( height * fraction );

	g_ctx.m_render.line( outline_color, top.x - 6.f, top.y - 1.f, top.x - 6.f, bot.y + 1.f );
	g_ctx.m_render.line( outline_color, top.x - 5.f, top.y - 1.f, top.x - 5.f, bot.y + 1.f );
	g_ctx.m_render.line( outline_color, top.x - 4.f, top.y - 1.f, top.x - 4.f, bot.y + 1.f );
	g_ctx.m_render.line( outline_color, top.x - 3.f, top.y - 1.f, top.x - 3.f, bot.y + 1.f );

	g_ctx.m_render.line( color, top.x - 5.f, top.y + pos, top.x - 5.f, bot.y );
	g_ctx.m_render.line( color, top.x - 4.f, top.y + pos, top.x - 4.f, bot.y );

	if ( ent->health( ) != 100 )
	{
		g_ctx.m_render._string( font_t::font_esp_small, color_t( 0, 0, 0 ), top.x - 4.f, top.y + pos - 4.0f, true, xor ( "%i" ), ent->health( ) );
		g_ctx.m_render._string( font_t::font_esp_small, color_t( 255, 255, 255 ), top.x - 5.f, top.y + pos - 5.0f, true, xor ( "%i" ), ent->health( ) );
	}
}