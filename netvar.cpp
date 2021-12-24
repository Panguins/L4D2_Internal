#include "netvar.h"
#include "hash.h"

#include <vector>
#include <functional>

struct netvar_dump_t
{
	std::string class_name;
	std::string netvar_name;
	uintptr_t offset;
	std::string type_name;
};

struct class_dump_t
{
	std::string class_name;
	int class_id;
};


c_netvar_manager g_netvar;

std::vector<netvar_dump_t> netvar_map;
std::vector<class_dump_t> class_map;

void c_netvar_manager::dump_netvars( ClientClass* client_classes )
{
	DELETE_START( 0 );
	netvar_map.clear( );

	for ( auto client_class = client_classes; client_class; client_class = client_class->pNextClass )
	{
		if ( client_class->rtTable )
			dump_recursive( client_class->szName, client_class->rtTable, 0 );

		if ( dump )
		{
			class_dump_t class_dump;
			class_dump.class_name = client_class->szName;
			class_dump.class_id = client_class->iClassID;
			class_map.push_back( class_dump );
		}
	}


	if ( dump )
	{
		auto netvars_file = fopen( xors( "netvars_file.txt" ), "w" );
		if ( netvars_file && !netvar_map.empty( ) )
		{

			int netvar_count = int( netvar_map.size( ) ) - 1;
			for ( int i = netvar_count; i >= 0; i-- )
			{
				netvar_dump_t* netvar = &netvar_map.at( i );
				fprintf( netvars_file, "%s -> %s | %s |0x%X \n", netvar->class_name.c_str( ), netvar->netvar_name.c_str( ), netvar->type_name.c_str( ),  netvar->offset );
			}

			fclose( netvars_file );
		}
		
		auto class_file = fopen( xors( "class_file.txt" ), "w" );
		if ( class_file && !class_map.empty( ) )
		{
			std::function<bool( const class_dump_t&, const class_dump_t& )> class_sort_fn;
			class_sort_fn = []( const class_dump_t& a, const class_dump_t& b ) -> bool
			{
				return a.class_id > b.class_id;
			};

			std::sort( class_map.begin( ), class_map.end( ), class_sort_fn );

			for ( size_t i = 0; i < class_map.size( ); i++ )
			{
				class_dump_t* class_dump = &class_map.at( i );
				fprintf( class_file, "%s = %d, \n", class_dump->class_name.c_str( ), class_dump->class_id );
			}

			fclose( class_file );
		}
	}
	
	DELETE_END( 0 );
}

void c_netvar_manager::dump_recursive( const char* base_class, RecvTable* table, uint16_t offset )
{
	for ( auto i = 0; i < table->m_nProps; ++i )
	{
		auto prop = &table->m_pProps[i];

		// skip trash array items
		if ( !prop || isdigit( prop->m_pVarName[0] ) )
			continue;

		// we dont care about the base class, we already know that
		if ( strcmp( prop->m_pVarName, xors( "baseclass" ) ) == 0 )
			continue;

		// skip shitty tables
		if ( prop->m_RecvType == DPT_DataTable &&
			 prop->m_pDataTable != nullptr &&
			 prop->m_pDataTable->m_pNetTableName[0] == 'D' )
		{
			dump_recursive( base_class, prop->m_pDataTable, offset + prop->m_Offset );
		}

		char hash_name[128];
		strcpy( hash_name, base_class );
		strcat( hash_name, xors( "->" ) );
		strcat( hash_name, prop->m_pVarName );

		if ( dump)
		{
			netvar_dump_t netvar_dump;
			netvar_dump.class_name = base_class;
			netvar_dump.netvar_name = prop->m_pVarName;
			netvar_dump.offset = offset + prop->m_Offset;

			switch ( prop->m_RecvType)
			{
			case DPT_Int:
				netvar_dump.type_name = xors( "int" );
				break;
			case DPT_Float:
				netvar_dump.type_name = xors( "float" );
				break;
			case DPT_Vector:
				netvar_dump.type_name = xors( "vec3" );
				break;
			case DPT_VectorXY:
				netvar_dump.type_name = xors( "vec2" );
				break;
			case DPT_String:
				netvar_dump.type_name = xors( "string" );
				break;
			case DPT_Array:
				netvar_dump.type_name = xors( "array" );
				break;
			case DPT_DataTable:
				netvar_dump.type_name = xors( "table" );
				break;
			default:
				netvar_dump.type_name = xors( "wtf" );
				break;
			}

			netvar_map.push_back( netvar_dump );
		}

		auto hash = fnv_hash( static_cast<const char*>( hash_name ) );

		m_propmap[hash] = std::make_pair( prop, offset + prop->m_Offset );
	}
}