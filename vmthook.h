#pragma once
#include <vector>

class c_vmt
{
	int count( )
	{
		int vfunc_count{ };

		while ( m_original[vfunc_count] )
			vfunc_count++;

		return vfunc_count;
	}

	uintptr_t* m_table;
	uintptr_t* m_original;
	std::vector< uintptr_t > m_custom_table;
public:
	c_vmt( void* table )
	{
		if ( !table )
			return;

		m_table = reinterpret_cast< uintptr_t* >( table );
		m_original = *reinterpret_cast< uintptr_t** >( m_table );

		int vfunc_count = count( );
		for ( int i = -1; i < vfunc_count; ++i )
			m_custom_table.push_back( m_original[i] );

		auto data = m_custom_table.data( );
		*this->m_table = uintptr_t( &data[1] );
	}

	~c_vmt( ) { restore( ); }

	template< typename T = uintptr_t > T get_function( int index )
	{
		return( (T)( m_custom_table.at( index + 1 ) ) );
	}

	template< typename T = uintptr_t > T get_original( int index )
	{
		return( (T)( m_original[index] ) );
	}

	void hook( int index, uintptr_t new_func )
	{
		m_custom_table.at( index + 1 ) = new_func;
	}

	void unhook( int index )
	{
		m_custom_table.at( index + 1 ) = m_original[index];
	}

	void hook( int index, void* new_func )
	{
		hook( index, reinterpret_cast< uintptr_t >( new_func ) );
	}

	void restore( ) const
	{
		*m_table = uintptr_t( m_original );
	}
};
