#pragma once

#include <memory>
#include <vector>
#include "input.h"

extern void simple_save( const char* path, const char* name, const void* src, size_t size, const char* filename );
extern void simple_load( const char* path, const char* name, void* dest, size_t size, const char* filename );

class node_t
{
public:
	virtual ~node_t( ) = default;

	virtual void add( node_t* node_ptr ) = 0;

	virtual void load( const char* path, const char* file ) = 0;
	virtual void save( const char* path, const char* file ) const = 0;
	virtual void clear( ) = 0;
};

class holder_t : public node_t
{
public:
	__forceinline holder_t( const char* name ) : m_name( name ) { }
	__forceinline holder_t( holder_t* holder_ptr, const char* name ) : m_name( name ) { holder_ptr->add( this ); }

	void add( node_t* node_ptr ) override
	{
		m_nodes.push_back( node_ptr );
	}

	void load( const char* path, const char* file ) override
	{
		char full_path[64];
		strcpy_s( full_path, path );
		strcat_s( full_path, "_" );
		strcat_s( full_path, m_name );
		for ( auto x : m_nodes )
			x->load( full_path, file );
	}

	void save( const char* path, const char* file ) const override
	{
		char full_path[64];
		strcpy_s( full_path, path );
		strcat_s( full_path, "_" );
		strcat_s( full_path, m_name );
		for ( auto x : m_nodes )
			x->save( full_path, file );
	}

	void clear( )
	{
		for ( auto x : m_nodes )
			x->load( "979", "979" );
	}

	const char* m_name;
	std::vector<node_t*> m_nodes;
};

class ISetting : public node_t
{
	void add( node_t* node_ptr ) override { }
};

template <typename T>
class setting_t : ISetting
{
public:
	__forceinline setting_t( holder_t* holder_ptr, const char* name ) : m_name( name ) { holder_ptr->add( this ); }
	__forceinline setting_t( holder_t* holder_ptr, const char* name, const T& rhs ) : m_value( rhs ), m_name( name ) { holder_ptr->add( this ); }

	void clear( ) override { simple_load( "979", "979", &m_value, sizeof( m_value ), "979" ); }
	void load( const char* path, const char* file ) override { simple_load( path, m_name, &m_value, sizeof( m_value ), file ); }
	void save( const char* path, const char* file ) const override { simple_save( path, m_name, &m_value, sizeof( m_value ), file ); }

	__forceinline operator T&( ) { return m_value; }
	__forceinline operator const T&( ) const { return m_value; }
	__forceinline T* operator &( ) { return &m_value; }
	__forceinline const T* operator &( ) const { return &m_value; }

	__forceinline T& operator ()( ) { return m_value; }
	__forceinline const T& operator ()( ) const { return m_value; }
private:
	T m_value;
	const char* m_name;
};

class c_settings
{
public:

	holder_t m_holder{ "" };

	setting_t<bool> rage_aimbot_enabled{ &m_holder, "rage_aimbot_enabled", 0 };
	setting_t<int> rage_silent{ &m_holder, "rage_silent", 0 };
	setting_t<int> rage_key{ &m_holder, "rage_key", 0 };
	setting_t<bool> rage_auto_shoot{ &m_holder, "rage_auto_shoot", 0 };
	setting_t<int> rage_fov{ &m_holder, "rage_fov", 0 };
	setting_t<bool> rage_anti_spread{ &m_holder, "rage_anti_spread", 0 };
	setting_t<int> rage_selection{ &m_holder, "rage_selection", 0 };
	setting_t<bool> rage_anti_recoil{ &m_holder, "rage_anti_recoil", 0 };
	setting_t<bool> rage_anti_aim{ &m_holder, "rage_anti_aim", 0 };
	setting_t<bool> rage_fakelag{ &m_holder, "rage_fakelag", 0 };
	setting_t<int> rage_fakelag_amount{ &m_holder, "rage_fakelag_amount", 0 };

	setting_t<bool> rage_filter_player{ &m_holder, "rage_filter_player", 0 };
	setting_t<bool> rage_filter_boss{ &m_holder, "rage_filter_llama", 0 };
	setting_t<bool> rage_filter_special{ &m_holder, "rage_filter_special", 0 };
	setting_t<bool> rage_filter_infected{ &m_holder, "rage_filter_infected", 0 };

	setting_t<bool> visuals_active{ &m_holder, "visuals_active", 0 };

	setting_t<bool> visuals_box{ &m_holder, "visuals_box", 0 };
	setting_t<bool> visuals_name{ &m_holder, "visuals_name", 0 };
	setting_t<bool> visuals_skeleton{ &m_holder, "visuals_skeleton", 0 };
	setting_t<bool> visuals_health{ &m_holder, "visuals_health", 0 };

	setting_t<bool> visuals_filter_enemies_only{ &m_holder, "visuals_filter_enemies_only", 0 };
	setting_t<bool> visuals_filter_players{ &m_holder, "visuals_filter_players", 0 };
	setting_t<bool> visuals_filter_boss{ &m_holder, "visuals_filter_llama", 0 };
	setting_t<bool> visuals_filter_special{ &m_holder, "visuals_filter_special", 0 };
	setting_t<bool> visuals_filter_infected{ &m_holder, "visuals_filter_infected", 0 };

	setting_t<color_t> visuals_color_players{ &m_holder, "visuals_color_players", { 150, 200, 50, 255 } };
	setting_t<color_t> visuals_color_boss{ &m_holder, "visuals_color_boss",{ 150, 0, 0, 255 } };
	setting_t<color_t> visuals_color_special{ &m_holder, "visuals_color_special",{ 250, 200, 0, 255 } };
	setting_t<color_t> visuals_color_infected{ &m_holder, "visuals_color_infected",{ 255, 50, 50, 255 } };

	setting_t<bool> visuals_crosshair{ &m_holder, "visuals_crosshair", 0 };
	setting_t<int> visuals_view_fov{ &m_holder, "visuals_view_fov", 0 };
	setting_t<bool> visuals_thirdperson{ &m_holder, "visuals_thirdperson", 0 };
	setting_t<int> visuals_thirdperson_dist{ &m_holder, "visuals_thirdperson_dist", 0 };
	setting_t<int> visuals_thirdperson_key{ &m_holder, "visuals_thirdperson_key", 0 };

	setting_t<bool> misc_bhop{ &m_holder, "misc_bhop", 0 };
	setting_t<bool> misc_autostrafe{ &m_holder, "misc_autostrafe", 0 };
	setting_t<bool> misc_rapidfire{ &m_holder, "misc_rapidfire", 0 };

	setting_t<bool> misc_speedhack_enabled{ &m_holder, "misc_speedhack_enabled", 0 };
	setting_t<int> misc_speedhack_key{ &m_holder, "misc_speedhack_key", 0 };
	setting_t<int> misc_speedhack_factor{ &m_holder, "misc_speedhack_factor", 0 };

	setting_t< color_t> menu_color{ &m_holder, "menu_color",{ 181, 0, 41, 255 } };
	setting_t< bool > menu_rainbow{ &m_holder, "menu_rainbow", 0 };
	setting_t< int > menu_fade_speed{ &m_holder, "menu_fade_speed", 5 };

	void save( const char* name ) { m_holder.save( "", name ); }
	void load( const char* name ) { m_holder.load( "", name ); }
	void clear( ) { m_holder.clear( ); }
};