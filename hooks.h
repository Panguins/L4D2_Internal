#pragma once

namespace hooks
{
	long __stdcall wndproc( void* hwnd, unsigned msg, unsigned wparam, long lparam );
	extern decltype( &wndproc ) o_wndproc;

	void __fastcall scene_end( void* ecx, void* edx );
	void __fastcall paint( void* ecx, void* edx, int mode );
	bool __fastcall createmove( void* ecx, void* edx, float input_sample, CUserCmd* cmd );
	void __fastcall on_screen_size_changed( void* ecx, void* edx, int old_width, int old_height );
	void __fastcall frame_stage_notify( void* ecx, void* edx, ClientFrameStage_t stage );
	void __fastcall override_view( void* ecx, void* edx, CViewSetup* view );
	void __fastcall lock_cursor( void* ecx, void* edx );

	//void __fastcall paint_traverse( void* ecx, void* edx, int panel, bool force_repaint, bool allow_force );
	//HRESULT __stdcall present( IDirect3DDevice9* device, RECT* src, const RECT* dst, HWND dst_window_override, const RGNDATA* dirty_region );
}