#pragma once

#define EXPAND( x,y )x##y

#define BEGIN___( MARKER_BEGIN )MARKER_BEGIN:

#define DEL_END___( MARKER_BEGIN, MARKER_END )__asm { __asm PUSH MARKER_END __asm PUSH MARKER_BEGIN __asm CALL erase } \
MARKER_END: 

#define BEGIN__( x, y ) BEGIN___( EXPAND2( MARKER_BEGIN_, x, y ) )
#define END__( x, y ) DEL_END___( EXPAND2( MARKER_BEGIN_, x, y ), EXPAND2( MARKER_END_, x, y ) )

#define DELETE_START( x ) BEGIN___( EXPAND( MARKER_BEGIN_, x ) )
#define DELETE_END( x ) DEL_END___( EXPAND( MARKER_BEGIN_, x ), EXPAND( MARKER_END_, x ) )


static void __stdcall erase( uintptr_t start, uintptr_t end )
{
	uintptr_t size = end - start;

	ulong protection_flag{ };
	VirtualProtect( (void*)start, size, PAGE_EXECUTE_READWRITE, &protection_flag );

	for ( unsigned int i = 0; i < size; i++ )
	{
		int random = rand( ) % 0x90;
		memset( (void*)( start + i ), random, 1 );
	}

	VirtualProtect( (void*)start, size, protection_flag, &protection_flag );
}