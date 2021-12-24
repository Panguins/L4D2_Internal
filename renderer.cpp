#include "renderer.h"
#include <stdio.h>
#include <limits>
#include "l4d2.h"


c_render::c_render( )
{

}

c_render::~c_render( )
{

}

void c_render::start( )
{
	create_fonts( );

	//void __thiscall start_drawing ( void* surface ); 
	//55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC 14 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? 8B F9 80 3D
	static uintptr_t start_drawing_addr = uintptr_t( GetModuleHandleA( xor( "vguimatsurface.dll" ) ) ) + 0xB0D0;

	( ( void( __thiscall* )( ISurface* ) )( start_drawing_addr ) )( g_l4d2.surface );
}

void c_render::finish( )
{

	//void __thiscall end_drawing ( void* surface ); 
	//55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 51 56 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? 6A 00
	static uintptr_t end_drawing_addr = uintptr_t( GetModuleHandleA( xor( "vguimatsurface.dll" ) ) ) + 0xB2E0;


	( ( void( __thiscall* )( ISurface* ) )( end_drawing_addr ) )( g_l4d2.surface );
}

void c_render::line( color_t color, float x1, float y1, float x2, float y2 )
{
	g_l4d2.surface->DrawSetColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_l4d2.surface->DrawLine( uint32_t( x1 ), uint32_t( y1 ), uint32_t( x2 ), uint32_t( y2 ) );
}

void c_render::rect( color_t color, float x, float y, float w, float h )
{
	g_l4d2.surface->DrawSetColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_l4d2.surface->DrawOutlinedRect( uint32_t( x ), uint32_t( y ), uint32_t( w ), uint32_t( h ) );
}

void c_render::filled_rect( color_t color, float x, float y, float w, float h )
{
	g_l4d2.surface->DrawSetColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_l4d2.surface->DrawFilledRect( uint32_t( x ), uint32_t( y ), uint32_t( w ), uint32_t( h ) );
}

void c_render::_filled_rect( color_t color, float x, float y, float w, float h )
{
	g_l4d2.surface->DrawSetColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	g_l4d2.surface->DrawFilledRect( uint32_t( x ), uint32_t( y ), uint32_t( x + w ), uint32_t( y + h ) );
}

void c_render::rect_gradient( gradient_t type, color_t start, color_t end, float x, float y, float w, float h )
{
	color_t blend = color_t::blend( start, end, 0.5f );
	g_l4d2.surface->DrawSetColor( blend.r( ), blend.g( ), blend.b( ), blend.a( ) );
	g_l4d2.surface->DrawFilledRect( uint32_t( x ), uint32_t( y ), uint32_t( x + w ), uint32_t( y + h ) );

	g_l4d2.surface->DrawSetColor( start.r( ), start.g( ), start.b( ), start.a( ) );
	g_l4d2.surface->DrawFilledRectFade( uint32_t( x ), uint32_t( y ), uint32_t( x + w ), uint32_t( y + h ), 255, 0, type == horizontal );

	g_l4d2.surface->DrawSetColor( end.r( ), end.g( ), end.b( ), end.a( ) );
	g_l4d2.surface->DrawFilledRectFade( uint32_t( x ), uint32_t( y ), uint32_t( x + w ), uint32_t( y + h ), 0, 255, type == horizontal );
}

void c_render::string( font_t _font, color_t color, float x, float y, bool centered, const char* fmt, ... )
{
	char buffer[256];
	va_list args;
	va_start( args, fmt );
	vsprintf( buffer, fmt, args );
	va_end( args );

	HFont font;

	switch ( _font )
	{
	case font_t::font_menu:
		font = m_menu;
		break;
	case font_t::font_esp:
		font = m_esp;
		break;
	case font_t::font_esp_small:
		font = m_esp_small;
		break;
	default:
		font = m_menu;
		break;
	}

	if ( !font )
		return;

	size_t length = strlen( buffer ) + 1;
	wchar_t* wbuf = new wchar_t[length];
	mbstowcs_s( nullptr, wbuf, length, buffer, length - 1 );

	if (centered)
	{
		int w, h;
		g_l4d2.surface->GetTextSize( font, wbuf, w, h );

		x -= w / 2;
	}

	g_l4d2.surface->DrawSetTextFont( font );
	g_l4d2.surface->DrawSetTextColor( color.r( ), color.g( ), color.b( ), 255 );
	g_l4d2.surface->DrawSetTextPos( uint32_t( x ), uint32_t( y ) );
	g_l4d2.surface->DrawPrintText( wbuf, wcslen( wbuf ) );

	delete[] wbuf;
}

void c_render::_string( font_t _font, color_t color, float x, float y, bool centered, const char* fmt, ... )
{
	char buffer[256];
	va_list args;
	va_start( args, fmt );
	vsprintf( buffer, fmt, args );
	va_end( args );

	HFont font;

	switch ( _font )
	{
	case font_t::font_menu:
		font = m_menu;
		break;
	case font_t::font_esp:
		font = m_esp;
		break;
	case font_t::font_esp_small:
		font = m_esp_small;
		break;
	default:
		font = m_menu;
		break;
	}

	if ( !font )
		return;

	size_t length = strlen( buffer ) + 1;
	wchar_t* wbuf = new wchar_t[length];
	mbstowcs_s( nullptr, wbuf, length, buffer, length - 1 );

	if ( centered )
	{
		int w, h;
		g_l4d2.surface->GetTextSize( font, wbuf, w, h );

		x -= w / 2;
	}

	g_l4d2.surface->DrawSetTextFont( font );
	g_l4d2.surface->DrawSetTextColor( color.r( ), color.g( ), color.b( ), 255 );
	g_l4d2.surface->DrawSetTextPos( uint32_t( x ), uint32_t( y ) );
	g_l4d2.surface->DrawPrintText( wbuf, wcslen( wbuf ) );

	delete[] wbuf;
}

void c_render::_string( font_t _font, color_t color, float x, float y, bool centered, const wchar_t* fmt, ... )
{
	wchar_t wbuf[256];
	va_list args;
	va_start( args, fmt );
	vswprintf( wbuf, 256, fmt, args );
	va_end( args );

	HFont font;

	switch ( _font )
	{
	case font_t::font_menu:
		font = m_menu;
		break;
	case font_t::font_esp:
		font = m_esp;
		break;
	case font_t::font_esp_small:
		font = m_esp_small;
		break;
	default:
		font = m_menu;
		break;
	}

	if ( !font )
		return;

	if ( centered )
	{
		int w, h;
		g_l4d2.surface->GetTextSize( font, wbuf, w, h );

		x -= w / 2;
	}

	g_l4d2.surface->DrawSetTextFont( font );
	g_l4d2.surface->DrawSetTextColor( color.r( ), color.g( ), color.b( ), 255 );
	g_l4d2.surface->DrawSetTextPos( uint32_t( x ), uint32_t( y ) );
	g_l4d2.surface->DrawPrintText( wbuf, wcslen( wbuf ) );
}

float c_render::text_width( font_t _font, const char* fmt, ... )
{
	char buffer[256];
	va_list args;
	va_start( args, fmt );
	vsprintf( buffer, fmt, args );
	va_end( args );

	HFont font;

	switch ( _font )
	{
	case font_t::font_menu:
		font = m_menu;
		break;
	case font_t::font_esp:
		font = m_esp;
		break;
	case font_t::font_esp_small:
		font = m_esp_small;
		break;
	default:
		font = m_menu;
		break;
	}

	if ( !font )
		return 0.f;

	size_t length = strlen( buffer ) + 1;
	wchar_t* wbuf = new wchar_t[length];
	mbstowcs_s( nullptr, wbuf, length, buffer, length - 1 );

	int w, h;
	g_l4d2.surface->GetTextSize( font, wbuf, w, h );

	delete[] wbuf;

	return w;
}

void c_render::create_fonts( )
{
	if ( fonts_initialized )
		return;

	m_menu      = g_l4d2.surface->Create_Font( );
	m_esp       = g_l4d2.surface->Create_Font( );
	m_esp_small = g_l4d2.surface->Create_Font( );

	g_l4d2.surface->SetFontGlyphSet( m_menu, xor ( "Tahoma" ), 12, FW_NORMAL, 0, 0, FONTFLAG_NONE );
	g_l4d2.surface->SetFontGlyphSet( m_esp, xor ( "Tahoma" ), 12, FW_NORMAL, 0, 0, FONTFLAG_NONE );
	g_l4d2.surface->SetFontGlyphSet( m_esp, xor ( "Tahoma" ), 11, FW_NORMAL, 0, 0, FONTFLAG_NONE );


	fonts_initialized = true;
}