#pragma once

class c_fakelag
{
	int   m_choked_ticks{ };
	bool  m_should_choke{ };
	bool* m_send_packet{ };

public:
	void setup( uintptr_t ebp );
	void init( );

	void choke( ) { m_should_choke = true; }
};