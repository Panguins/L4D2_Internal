#include "input.h"
#include <string>
#include "context.h"

c_input_manager g_input;

const char* const key_names[] =
{
	"unknown",
	"mouse1",
	"mouse2",
	"cancel",
	"mouse3",
	"mouse4",
	"mouse5",
	"unknown",
	"Backspace",
	"Tab",
	"unknown",
	"unknown",
	"Clear",
	"Return",
	"unknown",
	"unknown",
	"Shift",
	"Control",
	"Alt",
	"pause",
	"Caps Lock",
	"kana",
	"unknown",
	"junja",
	"final",
	"kanji",
	"unknown",
	"Escape",
	"convert",
	"nonconvert",
	"accept",
	"modechange",
	"Space",
	"prior",
	"next",
	"end",
	"home",
	"Left",
	"Up",
	"Right",
	"Down",
	"select",
	"print",
	"execute",
	"snapshot",
	"ins",
	"delete",
	"help",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"lwin",
	"rwin",
	"apps",
	"unknown",
	"unknown",
	"numpad0",
	"numpad1",
	"numpad2",
	"numpad3",
	"numpad4",
	"numpad5",
	"numpad6",
	"numpad7",
	"numpad8",
	"numpad9",
	"multiply",
	"add",
	"separator",
	"subtract",
	"decimal",
	"divide",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"f11",
	"f12",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"f21",
	"f22",
	"f23",
	"f24",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"numlock",
	"scroll",
	"oem_nec_equal",
	"oem_fj_masshou",
	"oem_fj_touroku",
	"oem_fj_loya",
	"oem_fj_roya",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"lshift",
	"rshift",
	"lcontrol",
	"rcontrol",
	"lmenu",
	"rmenu",
};

void c_input_manager::capture_mouse_move( long lparam )
{
	m_mouse_pos[0] = LOWORD( lparam );
	m_mouse_pos[1] = HIWORD( lparam );
}

bool c_input_manager::register_key_press( VirtualKeyEvents_t key_event, VirtualKeys_t key )
{
	switch ( key_event )
	{
	case KEYDOWN:
	{
		if ( is_valid_key( key ) )
			m_pressed_keys[key] = true;
		return true;
	}
	case KEYUP:
	{
		if ( is_valid_key( key ) )
			m_pressed_keys[key] = false;
		return true;
	}
	case SYSKEYDOWN:
	{ //WTF IS THIS STUPID SHIT, WHY IS ALT LITERALLY THE ONLY FUCKING KEY UNDER SYSKEYDOWN OUT OF ALL THE MODIFIER KEYS?
		if ( key == KEYS_ALT )
			m_pressed_keys[key] = true;
		break;
	}
	case SYSKEYUP:
	{
		if ( key == KEYS_ALT )
			m_pressed_keys[key] = false;
		break;
	}
	case LBUTTONDOWN:
		m_pressed_keys[KEYS_MOUSE1] = true;
		return true;
	case LBUTTONUP:
		m_pressed_keys[KEYS_MOUSE1] = false;
		return true;
	case RBUTTONDOWN:
		m_pressed_keys[KEYS_MOUSE2] = true;
		return true;
	case RBUTTONUP:
		m_pressed_keys[KEYS_MOUSE2] = false;
		return true;
	case MBUTTONDOWN:
		m_pressed_keys[KEYS_MOUSE3] = true;
		return true;
	case MBUTTONUP:
		m_pressed_keys[KEYS_MOUSE3] = false;
		return true;
	case XBUTTONDOWN:
	{
		bool pressed_xbutton = static_cast<bool>( HIWORD( key ) - 1 ); //should result in mouse4 as false, and mouse5 as true
		m_pressed_keys[pressed_xbutton ? KEYS_MOUSE5 : KEYS_MOUSE4] = true;
		return true;
	}
	case XBUTTONUP:
	{
		bool pressed_xbutton = static_cast<bool>( HIWORD( key ) - 1 ); //should result in mouse4 as false, and mouse5 as true
		m_pressed_keys[pressed_xbutton ? KEYS_MOUSE5 : KEYS_MOUSE4] = false;
		return true;
	}
	case MOUSEWHEEL:
	{
		short scroll_input = (short)HIWORD( key );
		m_scroll_wheel_state = scroll_input > 0 ? 1 : -1;
		return true;
	}
	}

	return key_event == 0x200 || key_event == 0x203 || key_event == 0x206 || key_event == 0x209; //gotta block WM_MOUSEFIST | WM_LBUTTONDBLCLK | WM_RBUTTONDBLCLK | WM_MBUTTONDBLCLK
}

bool c_input_manager::is_key_pressed( int key )
{
	auto k = static_cast< VirtualKeys_t >( key );
	return is_valid_key( k ) && m_pressed_keys[k];
}

const char* c_input_manager::get_key_name( VirtualKeys_t key )
{
	if ( !is_valid_key( key ) )
		return key_names[KEYS_NONE];

	return key_names[key];
}

VirtualKeys_t c_input_manager::is_any_key_pressed( )
{
	for ( size_t i{ }; i < KEYS_MAX; ++i )
	{
		if ( m_pressed_keys[i] )
		{
			return VirtualKeys_t( i );
		}
	}

	return KEYS_NONE;
}

int c_input_manager::get_scroll_state( )
{
	int current_state = m_scroll_wheel_state;
	m_scroll_wheel_state = 0;
	return current_state;
}

char c_input_manager::get_pressed_char( VirtualKeys_t* out )
{
	size_t pressed_character{ };
	for ( size_t i{ }; i < KEYS_MAX; ++i )
	{
		if ( is_key_pressed( VirtualKeys_t( i ) ) )
		{
			if ( ( i >= KEYS_A && i <= KEYS_Z )
				 || ( i >= KEYS_N0 && i <= KEYS_N9 ) )
			{
				pressed_character = i;
			}
		}
	}

	if ( pressed_character )
	{
		if ( out )
		{
			*out = VirtualKeys_t( pressed_character );
		}

		if ( is_key_pressed( KEYS_SHIFT ) )
		{
			if ( pressed_character >= KEYS_A
				 && pressed_character <= KEYS_Z )
				return char( pressed_character );

			//gay way to shift it to symbols
			if ( pressed_character >= KEYS_N0
				 && pressed_character <= KEYS_N9 )
			{
				switch ( pressed_character )
				{
				case KEYS_N0:
					return ')';
				case KEYS_N1:
					return '!';
				case KEYS_N2:
					return '@';
				case KEYS_N3:
					return '#';
				case KEYS_N4:
					return '$';
				case KEYS_N5:
					return '%';
				case KEYS_N6:
					return '^';
				case KEYS_N7:
					return '&';
				case KEYS_N8:
					return '*';
				case KEYS_N9:
					return '(';
				}
			}
		}
		else
		{
			if ( pressed_character >= KEYS_A
				 && pressed_character <= KEYS_Z )
				return char( tolower( pressed_character ) );

			if ( pressed_character >= KEYS_N0
				 && pressed_character <= KEYS_N9 )
				return char( pressed_character );
		}
	}
	else if ( is_key_pressed( KEYS_SPACE ) )
	{
		if ( out )
			*out = KEYS_SPACE;

		return ' ';
	}
	else if ( is_key_pressed( KEYS_BACK ) )
	{
		if ( out )
			*out = KEYS_BACK;

		return 0;
	}

	if ( out )
		*out = KEYS_NONE;

	return 0;
}