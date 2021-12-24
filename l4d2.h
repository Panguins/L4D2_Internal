#pragma once
#include "interfaces.h"
#include "hooks.h"

__forceinline void safe_unhook( c_vmt** hook )
{
	if ( *hook )
	{
		delete *hook;
		*hook = nullptr;
	}
}

class c_l4d2
{
public:
	// window
	HWND game_window{ };

	// interfaces
	void*                device{ };
	void*                clientmode{ };
	void*                enginevgui{ };
	IBaseClientDLL*      client{ };
	IVEngineClient*      engine{ };
	IClientEntityList*   entity_list;
	IEngineTrace*        trace;
	IVRenderView*        render_view;
	IVModelRenderer*     model_renderer;
	IMaterialSystem*     material_system;
	CModelInfo*          model_info;
	IPanel*              panel;
	ISurface*            surface;
	IPrediction*         prediction;
	IGameMovement*       game_movement;
	ICVar*               cvar;

	IMoveHelper*         move_helper;
	CGlobalVarsBase*     globals;
	CInput*              input;

	// vmt hooks
	c_vmt*   client_hook{ };
	c_vmt*   clientmode_hook{ };
	c_vmt*   surface_hook{ };
	c_vmt*   panel_hook{ };
	c_vmt*   render_view_hook{ };
	c_vmt*   device_hook{ };
	c_vmt*   enginevgui_hook{ };


	void unload( )
	{
		SetWindowLongPtr( game_window, GWLP_WNDPROC, (long)hooks::o_wndproc );

		safe_unhook( &client_hook );
		safe_unhook( &clientmode_hook );
		safe_unhook( &surface_hook );
		safe_unhook( &panel_hook );
		safe_unhook( &render_view_hook );
		safe_unhook( &device_hook );
		safe_unhook( &enginevgui_hook );
	}

	// this is questionable at best, but it means the static function can be wiped from memory right after use
	__forceinline void grab_interfaces( ) { static_grab_interfaces( this ); }
	__forceinline void hook_functions( ) { static_hook_functions( this ); }
private:
	static __declspec( noinline ) void static_grab_interfaces( c_l4d2* instance )
	{
		DELETE_START( 0 );

		instance->client          = interfaces::grab_interface<IBaseClientDLL>( xors( "client.dll" ), xors( "VClient016" ) );
		instance->entity_list     = interfaces::grab_interface<IClientEntityList>( xors( "client.dll" ), xors( "VClientEntityList003" ) );
		instance->prediction      = interfaces::grab_interface<IPrediction>( xors( "client.dll" ), xors( "VClientPrediction001" ) );
		instance->game_movement   = interfaces::grab_interface<IGameMovement>( xors( "client.dll" ), xors( "GameMovement001" ) );

		instance->engine          = interfaces::grab_interface<IVEngineClient>( xors( "engine.dll" ), xors( "VEngineClient013" ) );
		instance->trace           = interfaces::grab_interface<IEngineTrace>( xors( "engine.dll" ), xors( "EngineTraceClient003" ) );
		instance->render_view     = interfaces::grab_interface<IVRenderView>( xors( "engine.dll" ), xors( "VEngineRenderView013" ) );
		instance->model_renderer  = interfaces::grab_interface<IVModelRenderer>( xors( "engine.dll" ), xors( "VEngineModel016" ) );
		instance->model_info      = interfaces::grab_interface<CModelInfo>( xors( "engine.dll" ), xors( "VModelInfoClient004" ) );
		instance->enginevgui      = interfaces::grab_interface<CModelInfo>( xors( "engine.dll" ), xors( "VEngineVGui001" ) );

		instance->material_system = interfaces::grab_interface<IMaterialSystem>( xors( "materialsystem.dll" ), xors( "VMaterialSystem080" ) );
		instance->panel           = interfaces::grab_interface<IPanel>( xors( "vgui2.dll" ), xors( "VGUI_Panel009" ) );
		instance->surface         = interfaces::grab_interface<ISurface>( xors( "vguimatsurface.dll" ), xors( "VGUI_Surface031" ) );

		instance->cvar            = interfaces::grab_interface<ICVar>( xors( "vstdlib.dll" ), xors( "VEngineCvar007" ) );

		instance->device      = **pattern::first_code_match< void*** >( GetModuleHandleA( xors( "shaderapidx9.dll" ) ), "A1 ? ? ? ? 8B 08 6A 03", 1 );
		instance->move_helper = **(IMoveHelper***)     ( ( ( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0xFAD93 ) + 0x1 ) );
		instance->globals     = **(CGlobalVarsBase***) ( ( ( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x46C0 ) + 0x2 ) );
		instance->input       = **(CInput***)              ( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x1D7390 ); 		//8B 0D ? ? ? ? 8B 10 56

		// 8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 8B 04 85 ? ? ? ? C3
		// "Biggest possible signature is not unique, not going to bother searching any further" ( IDA 6.8, 2k18 )
		auto getclientmode = ( void*( __cdecl* )( ) )( uintptr_t( GetModuleHandleA( xors( "client.dll" ) ) ) + 0x223470 );
		instance->clientmode = getclientmode( );

		DELETE_END( 0 );
	}

	static __declspec( noinline ) void static_hook_functions( c_l4d2* instance )
	{
		DELETE_START( 0 );

		// replace window procedure function
		instance->game_window = FindWindowA( xors( "Valve001" ), nullptr );
		hooks::o_wndproc = (decltype( hooks::o_wndproc ))SetWindowLongPtr( instance->game_window, GWLP_WNDPROC, (long)hooks::wndproc );

		// replace vmt on interfaces
		instance->client_hook = new c_vmt( instance->client );
		instance->clientmode_hook = new c_vmt( instance->clientmode );
		instance->surface_hook = new c_vmt( instance->surface );
		instance->panel_hook = new c_vmt( instance->panel );
		instance->render_view_hook   = new c_vmt( instance->render_view );
		instance->enginevgui_hook = new c_vmt( instance->enginevgui );
		//instance->device_hook        = new c_vmt( instance->device );

		// add custom functions to custom vmt
		instance->client_hook->hook( 34, hooks::frame_stage_notify );
		instance->clientmode_hook->hook( 27, hooks::createmove );
		instance->clientmode_hook->hook( 19, hooks::override_view );
		instance->surface_hook->hook( 59, hooks::lock_cursor );
		instance->surface_hook->hook( 108, hooks::on_screen_size_changed );
		instance->enginevgui_hook->hook( 14, hooks::paint );
		instance->render_view_hook->hook( 9, hooks::scene_end );

		//instance->panel_hook->hook( 41, hooks::paint_traverse );
		//instance->device_hook->hook( 17, hooks::present );

		DELETE_END( 0 );
	}

}; extern c_l4d2 g_l4d2;

static constexpr float TICK_INTERVAL( ) { return g_l4d2.globals->interval_per_tick; }