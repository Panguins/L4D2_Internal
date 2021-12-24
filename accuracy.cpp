#include "accuracy.h"
#include "context.h"
#include "player.h"
#include "l4d2.h"


#pragma optimize( "", off ) 
void c_accuracy::nospread( )
{
	if ( !g_ctx.m_settings.rage_aimbot_enabled || !g_ctx.m_settings.rage_anti_spread )
		return;

	if ( !( g_ctx.m_cmd->buttons & IN_ATTACK ) )
		return;

	HANDLE weapon_handle = g_ctx.m_local->weapon( );
	if ( !weapon_handle )
		return;

	CBaseWeapon* weapon = (CBaseWeapon*)g_l4d2.entity_list->GetClientEntityFromHandle( weapon_handle );
	if ( !weapon || !weapon->is_valid_aim_weapon( ) )
		return;

	static uintptr_t update_max_spread = uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x2ECAF0;
	static uintptr_t shared_random_float_addr = uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x1A8170;
	static auto shared_random_float = ( float( __cdecl* )( const char*, float, float, int ) )shared_random_float_addr;

	// store original data
	int32_t original_seed = **(int32_t**)( shared_random_float_addr + 7 );
	float original_spread = weapon->spread( );

	// overwrite random seed
	**(int32_t**)( shared_random_float_addr + 7 ) = g_ctx.m_cmd->random_seed;

	// update spread
	//( ( void( __cdecl* )( CBaseWeapon* ) )( update_max_spread ) )( weapon );

	// calculate the spread
	float spread_x = shared_random_float( xors( "CTerrorGun::FireBullet HorizSpread" ), -weapon->spread( ), weapon->spread( ), 0 );
	float spread_y = shared_random_float( xors( "CTerrorGun::FireBullet VertSpread" ), -weapon->spread( ), weapon->spread( ), 0 );

	// restore data
	**(int32_t**)( shared_random_float_addr + 7 ) = original_seed;
	weapon->spread( ) = original_spread;

	// mitigate spread
	g_ctx.m_cmd->viewangles.x -= spread_x;
	g_ctx.m_cmd->viewangles.y -= spread_y;
}
#pragma optimize( "", on ) 

void c_accuracy::norecoil( )
{
	if ( !g_ctx.m_settings.rage_anti_recoil )
		return;

	g_ctx.m_cmd->viewangles.x -= g_ctx.m_local->punch( ).x;
	g_ctx.m_cmd->viewangles.y -= g_ctx.m_local->punch( ).y;
}