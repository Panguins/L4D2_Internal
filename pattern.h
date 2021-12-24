#pragma once
#include <string>
#include <sstream>
#include <iterator>
#include <Vector>

namespace pattern
{
	inline bool bin_match( uint8_t* code, const std::vector< uint8_t >& pattern )
	{
		for ( size_t j = 0; j < pattern.size( ); j++ )
		{
			if ( pattern[j] && code[j] != pattern[j] )
			{
				return false;
			}
		}
		return true;
	}

	template< typename t = uintptr_t > static t first_match( uintptr_t start, std::string sig, size_t len, std::ptrdiff_t skip = 0 )
	{
		std::istringstream iss( sig );
		std::vector< std::string > tokens{ std::istream_iterator< std::string >{ iss }, std::istream_iterator< std::string >{} };
		std::vector< uint8_t > sig_bytes;

		for ( const auto& hex_byte : tokens )
		{
			sig_bytes.push_back( static_cast< uint8_t >( std::strtoul( hex_byte.c_str( ), nullptr, 16 ) ) );
		}

		if ( sig_bytes.empty( ) || sig_bytes.size( ) < 2 )
		{
			return t{ };
		}

		for ( size_t i{ }; i < len; i++ )
		{
			uint8_t* code_ptr = reinterpret_cast< uint8_t* >( start + i );

			if ( code_ptr[0] != sig_bytes.at( 0 ) )
			{
				continue;
			}

			if ( bin_match( code_ptr, sig_bytes ) )
			{
				return( (t)( start + i + skip ) );
			}
		}

		return t{ };
	}

	//ultimately the function you want to call to sigscan ( ida style )
	template< typename t = uintptr_t > static t first_code_match( HMODULE start, std::string sig, std::ptrdiff_t skip = 0 )
	{
		auto ntoskrnl = reinterpret_cast< PIMAGE_DOS_HEADER >( start );

		if ( !ntoskrnl ) return t{ };

		//DOS header, verifies if module is valid
		if ( ntoskrnl->e_magic != 0x5a4d )
		{
			return t{ };
		}

		auto nt_hdrs = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< uintptr_t >( ntoskrnl ) + ntoskrnl->e_lfanew );

		return first_match< t >( reinterpret_cast< uintptr_t >( ntoskrnl ) + nt_hdrs->OptionalHeader.BaseOfCode, sig, nt_hdrs->OptionalHeader.SizeOfCode, skip );
	}
}
