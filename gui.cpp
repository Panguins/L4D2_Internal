#include "renderer.h"
#include "context.h"

#include "gui.h"

#include <stdio.h>
#include <algorithm>

#include "menu.h"

// symbols often used that are hard to get
// °

c_gui gui;
UI::c_window menu;
UI::c_cursor cursor;

void c_gui::tab_aimbot( )
{
	static UI::c_checkbox enable;

	static UI::c_groupbox gb_aimbot;

	static UI::c_key_bind aimkey;
	static UI::c_checkbox autoshoot;
	static UI::c_slider   fov;
	static UI::c_dropdown silent;
	static UI::c_checkbox no_recoil;
	static UI::c_checkbox no_spread;

	menu.column_left( );
	{
		enable.handle( menu.data( ), xor ( "enable" ), &g_ctx.m_settings.rage_aimbot_enabled );

		gb_aimbot.start( menu.data( ), xor ( "aimbot" ) );
		{
			aimkey.handle( menu.data( ), xor ( "aim key" ), &g_ctx.m_settings.rage_key );
			autoshoot.handle( menu.data( ), xor ( "auto shoot" ), &g_ctx.m_settings.rage_auto_shoot );
			fov.handle( menu.data( ), xor ( "fov" ), &g_ctx.m_settings.rage_fov, 1, 360, L"°" );
			silent.handle( menu.data( ), xor ( "silent" ), { xor ( "off" ), xor ( "client" ), xor ( "server" ) }, &g_ctx.m_settings.rage_silent );
			no_recoil.handle( menu.data( ), xor ( "no recoil" ), &g_ctx.m_settings.rage_anti_recoil );
			no_spread.handle( menu.data( ), xor ( "no spread" ), &g_ctx.m_settings.rage_anti_spread );
		}
		gb_aimbot.end( menu.data( ), xor ( "aimbot" ) );
	}

	static UI::c_label    pad;
	static UI::c_groupbox gb_selection;
	static UI::c_dropdown priority;
	static UI::c_checkbox player;
	static UI::c_checkbox boss;
	static UI::c_checkbox special;
	static UI::c_checkbox infected;

	static UI::c_groupbox gb_antihit;
	static UI::c_checkbox antiaim;
	static UI::c_checkbox fakelag;
	static UI::c_slider   fakelag_amount;

	menu.column_right( );
	{
		pad.handle( menu.data( ), nullptr );

		gb_antihit.start( menu.data( ), xor ( "anti hit" ) );
		{
			antiaim.handle( menu.data( ), xor ( "anti aim" ), &g_ctx.m_settings.rage_anti_aim );
			fakelag.handle( menu.data( ), xor ( "fake lag" ), &g_ctx.m_settings.rage_fakelag );

			if ( g_ctx.m_settings.rage_fakelag )
				fakelag_amount.handle( menu.data( ), xor ( "amount" ), &g_ctx.m_settings.rage_fakelag_amount, 0, 15 );
		}
		gb_antihit.end( menu.data( ), xor ( "anti hit" ) );

		gb_selection.start( menu.data( ), xor ( "selection" ) );
		{
			priority.handle( menu.data( ), xor ( "priority" ), { xor ( "fov" ), xor ( "distance" ), xor ( "threat" ) }, &g_ctx.m_settings.rage_selection );
			player.handle( menu.data( ), xor ( "player" ), &g_ctx.m_settings.rage_filter_player );
			boss.handle( menu.data( ), xor ( "boss" ), &g_ctx.m_settings.rage_filter_boss );
			special.handle( menu.data( ), xor ( "special" ), &g_ctx.m_settings.rage_filter_special );
			infected.handle( menu.data( ), xor ( "infected" ), &g_ctx.m_settings.rage_filter_infected );
		}
		gb_selection.end( menu.data( ), xor ( "selection" ) );
	}

	silent.dropdown( menu.data( ) );
	priority.dropdown( menu.data( ) );
}

void c_gui::tab_visuals( )
{
	static UI::c_checkbox enable;

	static UI::c_groupbox gb_entity;
	static UI::c_checkbox box;
	static UI::c_checkbox name;
	static UI::c_checkbox skeleton;
	static UI::c_checkbox health;

	static UI::c_groupbox gb_visual;
	static UI::c_checkbox crosshair;
	static UI::c_slider   fov;
	static UI::c_checkbox thirdperson;
	static UI::c_slider   thirdperson_dist;
	static UI::c_slider   thirdperson_height;
	static UI::c_slider   thirdperson_offset;
	static UI::c_key_bind thirdperson_key;

	menu.column_left( );
	{
		enable.handle( menu.data( ), xor ( "enable" ), &g_ctx.m_settings.visuals_active );

		gb_entity.start( menu.data( ), xor ( "entity" ) );
		{
			box.handle( menu.data( ), xor ( "box" ), &g_ctx.m_settings.visuals_box );
			name.handle( menu.data( ), xor ( "name" ), &g_ctx.m_settings.visuals_name );
			skeleton.handle( menu.data( ), xor ( "skeleton" ), &g_ctx.m_settings.visuals_skeleton );
			health.handle( menu.data( ), xor ( "health" ), &g_ctx.m_settings.visuals_health );
		}
		gb_entity.end( menu.data( ), xor ( "entity" ) );

		gb_visual.start( menu.data( ), xor ( "visuals" ) );
		{
			crosshair.handle( menu.data( ), xor ( "crosshair" ), &g_ctx.m_settings.visuals_crosshair );
			fov.handle( menu.data( ), xor ( "fov" ), &g_ctx.m_settings.visuals_view_fov, 0, 150, L"°" );
			thirdperson.handle( menu.data( ), xor ( "thirdperson" ), &g_ctx.m_settings.visuals_thirdperson );
			if ( g_ctx.m_settings.visuals_thirdperson )
			{
				thirdperson_dist.handle( menu.data( ), xor ( "thirdperson dist" ), &g_ctx.m_settings.visuals_thirdperson_dist, 0, 100 );
				thirdperson_key.handle( menu.data( ), xor ( "thirdperson key" ), &g_ctx.m_settings.visuals_thirdperson_key );
			}
		}
		gb_visual.end( menu.data( ), xor ( "visuals" ) );
	}

	static UI::c_label    pad;

	static UI::c_groupbox gb_selection;
	static UI::c_checkbox enemy_only;
	static UI::c_checkbox_smoll player;
	static UI::c_checkbox_smoll boss;
	static UI::c_checkbox_smoll special;
	static UI::c_checkbox_smoll infected;

	static UI::c_color_picker player_color;
	static UI::c_color_picker boss_color;
	static UI::c_color_picker special_color;
	static UI::c_color_picker infected_color;

	menu.column_right( );
	{
		pad.handle( menu.data( ), nullptr );

		gb_selection.start( menu.data( ), xor ( "selection" ) );
		{
			enemy_only.handle( menu.data( ), xor ( "enemies only" ), &g_ctx.m_settings.visuals_filter_enemies_only );
			
			player.handle( menu.data( ), &g_ctx.m_settings.visuals_filter_players );
			player_color.handle( menu.data( ), xor ( "player" ), g_ctx.m_settings.visuals_color_players( ).r_ptr( ), g_ctx.m_settings.visuals_color_players( ).g_ptr( ), g_ctx.m_settings.visuals_color_players( ).b_ptr( ) );

			boss.handle( menu.data( ), &g_ctx.m_settings.visuals_filter_boss );
			boss_color.handle( menu.data( ), xor ( "boss" ), g_ctx.m_settings.visuals_color_boss( ).r_ptr( ), g_ctx.m_settings.visuals_color_boss( ).g_ptr( ), g_ctx.m_settings.visuals_color_boss( ).b_ptr( ) );

			special.handle( menu.data( ), &g_ctx.m_settings.visuals_filter_special );
			special_color.handle( menu.data( ), xor ( "special" ), g_ctx.m_settings.visuals_color_special( ).r_ptr( ), g_ctx.m_settings.visuals_color_special( ).g_ptr( ), g_ctx.m_settings.visuals_color_special( ).b_ptr( ) );

			infected.handle( menu.data( ), &g_ctx.m_settings.visuals_filter_infected );
			infected_color.handle( menu.data( ), xor ( "infected" ), g_ctx.m_settings.visuals_color_infected( ).r_ptr( ), g_ctx.m_settings.visuals_color_infected( ).g_ptr( ), g_ctx.m_settings.visuals_color_infected( ).b_ptr( ) );

		}
		gb_selection.end( menu.data( ), xor ( "selection" ) );
	}

	infected_color.picker( menu.data( ) );
	special_color.picker( menu.data( ) );
	boss_color.picker( menu.data( ) );
	player_color.picker( menu.data( ) );
}

void c_gui::tab_misc( )
{
	static UI::c_groupbox gb_movement;
	static UI::c_checkbox bhop;
	static UI::c_checkbox autostrafe;

	static UI::c_groupbox gb_speedhack;
	static UI::c_checkbox speedhack;
	static UI::c_key_bind speedhack_key;
	static UI::c_slider   speedhack_factor;

	menu.column_left( );
	{
		gb_movement.start( menu.data( ), xor ( "movement" ) );
		{
			bhop.handle( menu.data( ), xor ( "bunny hop" ), &g_ctx.m_settings.misc_bhop );
			autostrafe.handle( menu.data( ), xor ( "auto strafer" ), &g_ctx.m_settings.misc_autostrafe );
			autostrafe.handle( menu.data( ), xor ( "rapid fire" ), &g_ctx.m_settings.misc_rapidfire );
		}
		gb_movement.end( menu.data( ), xor ( "movement" ) );

		gb_speedhack.start( menu.data( ), xor ( "speedhack" ) );
		{
			speedhack.handle( menu.data( ), xor ( "enable" ), &g_ctx.m_settings.misc_speedhack_enabled );
			speedhack_key.handle( menu.data( ), xor ( "key" ), &g_ctx.m_settings.misc_speedhack_key );
			speedhack_factor.handle( menu.data( ), xor ( "factor" ), &g_ctx.m_settings.misc_speedhack_factor, 0, 100 );
		}
		gb_speedhack.end( menu.data( ), xor ( "speedhack" ) );
	}

	menu.column_right( );
	{
		
	}
}

void c_gui::tab_config( )
{

	static UI::c_groupbox menu_setts;
	static UI::c_groupbox hack;

	static UI::c_color_picker menu_color;
	static UI::c_slider       menu_fade_speed;
	static UI::c_checkbox     rainbow_mode;

	static UI::c_button   save;
	static UI::c_button   load;
	static UI::c_button   uninject;

	menu.column_left( );
	{
		menu_setts.start( menu.data( ), xor ( "menu" ) );
		{
			menu_color.handle( menu.data( ), xor ( "accent" ), g_ctx.m_settings.menu_color( ).r_ptr( ), g_ctx.m_settings.menu_color( ).g_ptr( ), g_ctx.m_settings.menu_color( ).b_ptr( ) );
			menu_fade_speed.handle( menu.data( ), xor ( "fade speed" ), &g_ctx.m_settings.menu_fade_speed, 0, 10 );
			rainbow_mode.handle( menu.data( ), xor ( "rainbow mode" ), &g_ctx.m_settings.menu_rainbow );

			menu_setts.end( menu.data( ), xor ( "menu" ) );
		}
	}

	menu.column_right( );
	{
		hack.start( menu.data( ), xor ( "hack" ) );
		{
			save.handle( menu.data( ), xor ( "save" ), [] { g_ctx.m_settings.save( xor ( "./onetap.cfg" ) ); } );
			load.handle( menu.data( ), xor ( "load" ), [] { g_ctx.m_settings.load( xor ( "./onetap.cfg" ) ); } );
			uninject.handle( menu.data( ), xor ( "unload" ), [] { g_ctx.panic = true; } );

			hack.end( menu.data( ), xor ( "hack" ) );
		}
	}

	menu_color.picker( menu.data( ) );
}

void c_gui::draw( )
{
	anti_rin( g_ctx.m_settings.load( xor ( "./onetap.cfg" ) ); )

	g_ctx.m_menu_open = menu.is_open( );

	if ( !menu.handle( m_tabs, UI::UIcolor_t( g_ctx.m_settings.menu_color( ).r( ), g_ctx.m_settings.menu_color( ).g( ), g_ctx.m_settings.menu_color( ).b( ) ) ) )
		return;

	switch ( menu.m_current_tab )
	{
	case TABS_AIMBOT:
		tab_aimbot( );
		break;
	case TABS_VISUALS:
		tab_visuals( );
		break;
	case TABS_MISC:
		tab_misc( );
		break;
	case TABS_CONFIG:
		tab_config( );
		break;
	}

	cursor.handle( );
}