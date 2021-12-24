#include "hooks.h"
#include "input.h"
#include "context.h"
#include "l4d2.h"

decltype( &hooks::wndproc ) hooks::o_wndproc;

long __stdcall hooks::wndproc( void* hwnd, unsigned msg, unsigned wparam, long lparam )
{
	if ( msg == WM_MOUSEMOVE )
	{
		g_input.capture_mouse_move( lparam );

		if ( g_ctx.m_menu_open )
			return true;
	}


	bool block_input = g_input.register_key_press( static_cast< VirtualKeyEvents_t >( msg ), static_cast< VirtualKeys_t >( wparam ) );
	if ( block_input && g_ctx.m_menu_open && msg != KEYUP )
		return true;

	return CallWindowProcA( (WNDPROC)o_wndproc, (HWND)hwnd, msg, wparam, lparam );
}