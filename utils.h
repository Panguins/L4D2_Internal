#pragma once
#include <Vector>

namespace util
{
	static __forceinline void get_all_files( std::vector< std::string >& out, const std::string& directory )
	{
		HANDLE           dir;
		WIN32_FIND_DATA  file_data;

		if ( ( dir = FindFirstFile( ( directory + "/*" ).c_str( ), &file_data ) ) == INVALID_HANDLE_VALUE )
			return;

		do
		{
			const std::string  file_name = file_data.cFileName;
			std::string        full_file_name = directory + "/" + file_name;
			const bool         is_directory = ( file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;

			if ( file_name[0] == '.' || !strstr( file_name.c_str( ), ".cfg" ) )
				continue;

			if ( is_directory )
				continue;

			// cull .// and .cfg
			out.push_back( ( full_file_name.substr( 0, full_file_name.find_last_of( "." ) ).substr( 3 ) ) );
		}
		while ( FindNextFile( dir, &file_data ) );

		FindClose( dir );
	}
}
