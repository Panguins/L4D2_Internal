#pragma once
#include <stdint.h>

enum gradient_t
{
	horizontal,
	vertical,
};

enum font_t
{
	font_menu,
	font_esp,
	font_esp_small,
};

class c_render
{
public:
	bool fonts_initialized{ };

	c_render( );
	~c_render( );

	void start( );
	void finish( );

	void rect( color_t color, float x, float y, float w, float h );
	void filled_rect( color_t color, float x, float y, float w, float h );
	void _filled_rect( color_t color, float x, float y, float w, float h );
	void rect_gradient( gradient_t, color_t start, color_t end, float x, float y, float w, float h );
	void line( color_t color, float x1, float y1, float x2, float y2 );
	void string( font_t, color_t color, float x, float y, bool centered, const char* fmt, ... );
	void _string( font_t, color_t color, float x, float y, bool centered, const char* fmt, ... );
	void _string( font_t, color_t color, float x, float y, bool centered, const wchar_t* fmt, ... );
	float text_width( font_t, const char* fmt, ... );

private:
	void create_fonts( );

	HFont m_menu;
	HFont m_esp;
	HFont m_esp_small;
};