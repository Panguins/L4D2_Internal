#include "player.h"
#include "l4d2.h"

bool CBaseEntity::get_hitbox_position( vec3& out, uint8_t bone_id )
{
	studiohdr_t* hdr = g_l4d2.model_info->GetStudioModel( GetModel( ) );
	if ( !hdr )
		return false;

	mstudiohitboxset_t* set = hdr->GetHitboxSet( 0 );
	mstudiobbox_t* hitbox = set->GetHitbox( bone_id );

	if ( !hitbox )
		return false;

	matrix3x4 matrix[128];
	if ( !SetupBones( matrix, 128, 0x100, g_l4d2.globals->curtime ) )
		return false;

	vec3 min, max;
	math::vector_transform( hitbox->bbmin, matrix[hitbox->bone], min );
	math::vector_transform( hitbox->bbmax, matrix[hitbox->bone], max );

	out = ( min + max ) * 0.5f;
	return true;
}

CBaseWeapon* CBaseEntity::get_weapon( )
{
	HANDLE weapon_handle = weapon( );
	if ( !weapon_handle )
		return nullptr;

	return (CBaseWeapon*)g_l4d2.entity_list->GetClientEntityFromHandle( weapon_handle );
}

bool CBaseEntity::can_attack( )
{
	CBaseWeapon* weapon = get_weapon( );
	if ( !weapon )
		return false;

	return tickbase( ) * g_l4d2.globals->interval_per_tick >= weapon->next_primary_attack( );
}