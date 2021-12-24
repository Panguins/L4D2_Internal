#pragma once

namespace interfaces
{
	struct InterfaceNode
	{
		void* ( *get_fn )( );
		const char* name;
		InterfaceNode* next;
	};

	__forceinline uintptr_t get_jmp_target( uintptr_t ptr )
	{
		const auto relative = *reinterpret_cast<int*>( ptr + 1 );
		ptr += 5;
		ptr += relative;
		return ptr;
	}

	__forceinline std::string get_first_number( std::string const & str )
	{
		std::size_t const n = str.find_first_of( xors( "0123456789" ) );
		if ( n != std::string::npos )
		{
			std::size_t const m = str.find_first_not_of( xors( "0123456789" ), n );
			return str.substr( n, m != std::string::npos ? m - n : m );
		}
		return std::string( );
	}

	__forceinline void* grab_interface_address( const char* module_name, const char* interface_name )
	{
		const auto module = GetModuleHandleA( module_name );

		uintptr_t ptr = uintptr_t( GetProcAddress( module, xors( "CreateInterface" ) ) );

		// push    ebp
		// mov     ebp, esp
		// pop     ebp
		// jmp     sub_106E4A60

		ptr += 4;
		ptr = get_jmp_target( ptr );

		// push    ebp
		// mov     ebp, esp
		// push    esi
		// mov     esi, dword_12EEBBF8

		InterfaceNode* highest_interface_ptr = nullptr;
		int highest_interface_version = -1;

		for ( auto node = **reinterpret_cast<InterfaceNode***>( ptr + 6 ); node; node = node->next )
		{
			char versionless_name[64] = { };
			size_t size = 0;
			// copy until end / until we hit a digit
			for ( size = 0; node->name[size] != '\0' && !isdigit( node->name[size] ); ++size )
				versionless_name[size] = node->name[size];

			versionless_name[size] = '\0';

			// found it
			if ( !strcmp( versionless_name, interface_name ) )
			{
				// lets always get the latest version
				int version = atoi( get_first_number( node->name ).c_str( ) );
				if ( version > highest_interface_version )
				{
					highest_interface_version = version;
					highest_interface_ptr = node;
				}
			}
		}

		if ( !highest_interface_ptr )
			return nullptr;

		printf( "INTERFACE %s | Version: %d \n", interface_name, highest_interface_version );

		return highest_interface_ptr->get_fn( );
	}

	template <typename T>
	__forceinline T* grab_interface( const char* module_name, const char* interface_name, bool version_unknown = false )
	{
		if ( version_unknown )
			return reinterpret_cast<T*>( grab_interface_address( module_name, interface_name ) );

		void* create_interface_addr = GetProcAddress( GetModuleHandleA( module_name ), xors( "CreateInterface" ) );
		auto create_interface_fn = ( void*( __cdecl* )( const char*, void* ) )( create_interface_addr );

		void* interface_address = create_interface_fn( interface_name, nullptr );

		printf( " > %s | 0x%X \n", interface_name, uintptr_t( interface_address ) );

		return (T*)( interface_address );
	}
}