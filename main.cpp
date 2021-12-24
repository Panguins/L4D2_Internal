#include <windows.h>
#include <thread>
#include "context.h"
#include "l4d2.h"
#include "netvar.h"
#include "cheat.h"

c_context g_ctx;
c_l4d2 g_l4d2;
c_cheat g_cheat;

void load( )
{
	DELETE_START( 0 );

	// initialize lava lamps for pRandomNumberGenerator
	time_t t;
	time( &t );
	srand( static_cast<uint32_t>( t ) );

	// find and load configs
	CreateFile( "./default.cfg", GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 );
	util::get_all_files( g_ctx.m_cfg_list, "./" );

	// l4d2 specific, the game does not load client.dll until all the intro videos are either finished or manually skipped
	while ( !GetModuleHandleA( xors( "client.dll" ) ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	// give it a bit more time to be ready
	std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

	// grab interfaces
	g_l4d2.grab_interfaces( );

	// dump netvars
	g_netvar.dump_netvars( g_l4d2.client->GetAllClasses( ) );

	// hook funcs
	g_l4d2.hook_functions( );

	DELETE_END( 0 );
}

void unload( )
{
	// free the allocated console
	fclose( stdout );
	FreeConsole( );

	// unhook all functions and restore vmt
	g_l4d2.unload( );

	// wait for shit to unload
	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

	// exit
	FreeLibraryAndExitThread( g_ctx.instance, 0 );
}

void main_thread( HINSTANCE instance )
{
	g_ctx.instance = instance;

	// allocate a console
	AllocConsole( );
	SetConsoleTitleA( xors( "979 Strassenbande" ) );
	freopen( xors( "CON" ), xors( "w" ), stdout );

	// load the cheat
	load( );

	// wait until we want to unload the cheat
	while ( !g_ctx.panic )
	{
		if ( GetAsyncKeyState( VK_DELETE ) )
			g_ctx.panic = true;

		// zzzzzzzzzzz
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	}

	// unload
	unload( );
}

BOOL APIENTRY DllMain( HINSTANCE instance, DWORD reason, LPVOID lpReserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( instance );
		CreateThread( nullptr, 0, (LPTHREAD_START_ROUTINE)( main_thread ), instance, 0, nullptr );
	}

	return 1;
}