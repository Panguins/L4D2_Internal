#pragma once
#include "netvar.h"

enum group_t
{
	GROUP_INVALID,
	GROUP_INFECTED,
	GROUP_SPECIAL,
	GROUP_BOSS,
	GROUP_PLAYER
};

class CBaseWeapon
{
public:
	NIGVAR( owner, HANDLE, 312 );
	NIGVAR( id, int, 2392 );
	NIGVAR( next_primary_attack, float, 2400 );
	NIGVAR( clip1, int, 2436 );
	NIGVAR( spread, float, 3340 );


	weapon_info_t* weapon_info( )
	{
		//Biggest possible signature is not unique, not going to bother searching any further.
		//IDA Style Sig : 0F B7 81 ? ? ? ? 50 E8 ? ? ? ? 83 C4 04
		//YE FUCK YOU TOO IDA
		static auto get_weapon_info = ( weapon_info_t* ( __thiscall* )( CBaseWeapon* ) ) ( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x148A0 );
		return get_weapon_info( this );
	}

	const char* weapon_name( )
	{
		//55 8B EC 8B 45 08 83 F8 37
		static auto get_weapon_name = ( char* ( *)( int ) ) ( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x22C770 );
		return get_weapon_name( id( ) );
	}

	bool is_valid_aim_weapon()
	{
		weapon_info_t* info = weapon_info( );
		if ( !info )
			return false;

		return info->iMaxClip1 > 1;
	}
};

class CBaseEntity : public IClientEntity
{
public:
	NIGVAR( origin, vec3, 148 );
	NIGVAR( view_offset, vec3, 244 );
	NIGVAR( velocity, vec3, 256 );

	NIGVAR( team, uint8_t, 228 );
	NIGVAR( health, uint32_t, 236 );
	NIGVAR( mins, vec3, 548 );
	NIGVAR( maxs, vec3, 560 );

	NIGVAR( flags, uint32_t, 240 );
	NIGVAR( lifestate, bool, 327 );

	NIGVAR( tickbase, uint32_t, 5356 );
	NIGVAR( punch, vec3, 4644 );

	NIGVAR( weapon, HANDLE, 4260 );
	NIGVAR( next_attack, float, 3872 );

	NIGVAR( sequence, uint32_t, 2212 );
	NIGVAR( max_health, uint32_t, 8188 );

	NIGVAR( zombie_state, int, 0x1CB4 );
	NIGVAR( zombie_class, int, 0x1CB0 );
	NIGVAR( survivor, int, 0x1CAC );
	NIGVAR( ghost, uint8_t, 0x1CBA );

	vec3 view_position( )
	{
		return origin( ) + view_offset( );
	}

	float health_fraction()
	{
		return std::clamp<float>( float( health( ) ) / float( max_health( ) ), 0.f, 1.f );
	}

	bool is_alive( )
	{
		return !lifestate( );
	}

	bool is_valid()
	{
		if ( IsDormant( ) )
			return false;

		int cur_team = team( );
		if ( cur_team != 2 && cur_team != 3 )
			return false;

		if ( !is_alive( ) )
			return false;

		group_t group = get_group( );
		if ( group == GROUP_INVALID )
			return false;

		if ( group == GROUP_BOSS && sequence( ) > 70 )
			return false;

		if ( group == GROUP_INFECTED && sequence( ) > 305 )
			return false;

		return group == GROUP_PLAYER || !( GetCollideable( )->GetSolidFlags( ) & 4 );

	}

	uint8_t get_head_bone_id()
	{
		int class_index = GetClientClass( )->iClassID;

		if ( class_index == Infected )
			return 15;

		if ( class_index == Tank )
			return 12;

		if ( class_index == Spitter || class_index == Jockey)
			return 4;

		if ( class_index == Charger )
			return 9;

		return 10;
	}

	bool get_hitbox_position( vec3& out, uint8_t bone_id );

	bool is_bot()
	{
		return GetClientClass( )->iClassID != CTerrorPlayer;
	}

	bool is_player( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == CTerrorPlayer;
	}

	group_t get_group( )
	{
		int class_index = GetClientClass( )->iClassID;

		if ( class_index == Infected )
			return GROUP_INFECTED;

		if ( class_index == CTerrorPlayer )
		{
			if ( team( ) == 2 )
				return GROUP_PLAYER;

			if ( zombie_class( ) == 8 ) //tank
				return GROUP_BOSS;

			return GROUP_SPECIAL;
		}

		if ( class_index == SurvivorBot )
			return GROUP_PLAYER;

		if ( class_index == Tank || class_index == Witch )
			return GROUP_BOSS;

		if ( class_index == Boomer || class_index == Smoker || class_index == Hunter || class_index == Jockey || class_index == Spitter || class_index == Charger )
			return GROUP_SPECIAL;

		return GROUP_INVALID;
	}

	bool is_infected()
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Infected;
	}

	bool is_tank( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Tank;
	}

	bool is_witch( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Witch;
	}

	bool is_boomer( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Boomer;
	}

	bool is_smoker( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Smoker;
	}

	bool is_hunter( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Hunter;
	}

	bool is_jockey( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Jockey;
	}

	bool is_spitter( )
	{
		int class_index = GetClientClass( )->iClassID;
		return class_index == Spitter;
	}

	const char* player_name( )
	{
		static uintptr_t get_player_name_addr = uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x62800;
		return ( ( const char*( __thiscall* )( CBaseEntity* ) )( get_player_name_addr ) )( this );
	}

	const char* network_name( )
	{
		return GetClientClass( )->szName;
	}

	std::wstring player_name_wide( )
	{
		const char* name = player_name( );

		std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > convert;
		return convert.from_bytes( std::string( name ) );
	}

	std::wstring network_name_wide( )
	{
		const char* name = network_name( );

		std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > convert;
		return convert.from_bytes( std::string( name ) );
	}

	CBaseWeapon* get_weapon( );

	bool can_attack( );
};