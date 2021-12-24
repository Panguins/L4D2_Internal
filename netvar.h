#pragma once
#include <unordered_map>
#include "dt_recv.h"

class c_netvar_manager
{
	const bool dump = false;
public:
	void dump_netvars( ClientClass* client_classes );

	uint16_t get_offset( fnv_t hash ) const { return m_propmap.at( hash ).second; }
private:
	std::unordered_map<fnv_t, std::pair<RecvProp*, uint16_t>> m_propmap;

	__forceinline void dump_recursive( const char* base_class, RecvTable* table, uint16_t offset );

}; extern c_netvar_manager g_netvar;

static __declspec( noinline ) uint16_t get_netvar_offset( fnv_t hash )
{
	return g_netvar.get_offset( hash );
}

#define PNETVAR_OFFSET(funcname, type, classname, var, extra) __forceinline type* funcname() \
{ \
	constexpr fnv_t hash = fnv_hash(classname "->" var); \
	static uint16_t offset = 0;\
	if(!offset) offset = extra + get_netvar_offset(hash); \
	return reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETVAR_OFFSET(funcname, type, classname, var, extra) __forceinline type& funcname() \
{ \
	constexpr fnv_t hash = fnv_hash(classname "->" var); \
	static uint16_t offset = 0;\
	if(!offset) offset = extra + get_netvar_offset(hash); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define PNETVAR(funcname, type, classname, var) PNETVAR_OFFSET(funcname, type, classname, var, 0)
#define NETVAR(funcname, type, classname, var) NETVAR_OFFSET(funcname, type, classname, var, 0)

#define NIGVAR(funcname, type, offset ) __forceinline type& funcname() \
{ \
	return *reinterpret_cast<type*>( ( uintptr_t ) this + offset ); \
}