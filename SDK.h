#pragma once
#include <windows.h>
#include "dt_recv.h"
#include "vector.h"
#include "trace.h"
#include "enumeration.h"

template <typename fn>
__forceinline fn vfunc( void* classbase, int func_index )
{
	return ( *reinterpret_cast<fn**>( classbase ) )[func_index];
}

typedef unsigned long HFont;

class CBaseEntity;
class IClientNetworkable;
class IClientEntity;
class IClientUnknown;
struct model_t;

class CUserCmd
{
public:
	virtual ~CUserCmd( ) { };
	int32_t command_number;
	int32_t tick_count;
	vec3 viewangles;
	float forwardmove;
	float sidemove;
	float upmove;
	int32_t buttons;
	byte impulse;
	int32_t weaponselect;
	int32_t weaponsubtype;
	int32_t random_seed;
	int16_t mousedx;
	int16_t mousedy;
	bool hasbeenpredicted;
};

class ClientClass
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* szName;
	RecvTable* rtTable;
	ClientClass* pNextClass;
	int iClassID;
};

struct weapon_info_t
{
	void* vtable;
	bool bParsedScript;
	bool bLoadedHudElements;
	char szClassName[80];
	char szPrintName[80];
	char szViewModel[80];
	char szWorldModel[80];
	char szAnimationPrefix[16];
	std::int16_t pad1;
	int32_t iSlot;
	int32_t iPosition;
	int32_t iMaxClip1;
	int32_t iMaxClip2;
	int32_t iDefaultClip1;
	int32_t iDefaultClip12;
	int32_t iWeight;
	int32_t iRumble;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	int32_t iFlags;
};

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses( )
	{
		typedef ClientClass* ( __thiscall* GetAllClassesFn )( void* );
		return vfunc<GetAllClassesFn>( this, 7 )( this );
	}

	void IN_ActivateMouse( )
	{
		typedef void ( __thiscall* IN_ActivateMouseFn )( void* );
		return vfunc<IN_ActivateMouseFn>( this, 15 )( this );
	}

	void IN_DeactivateMouse( )
	{
		typedef void( __thiscall* IN_DeactivateMouseFn )( void* );
		return vfunc<IN_DeactivateMouseFn>( this, 16 )( this );
	}
};

struct mstudiobone_t
{
	int sznameindex;
	inline char * const GetName( void ) const { return ( (char *)this ) + sznameindex; }
	int parent;
	int bonecontroller[6];
	vec3 pos;
	float quat[4];
	vec3 rot;
	vec3 posscale;
	vec3 rotscale;
	matrix3x4 poseToBone;
	float qAlignment[4];
	int	flags;
	int	proctype;
	int	procindex;			  // procedural rule
	mutable int	physicsbone;	// index into physically simulated bone
	inline void * GetProcedure( ) const { if ( procindex == 0 ) return NULL; else return  (void *)( ( (byte *)this ) + procindex ); };
	int	surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps( void ) const { return ( (char *)this ) + surfacepropidx; }
	int contents;			   // See BSPFlags.h for the contents flags
	int	unused[8];			  // remove as appropriate
};

struct mstudiobbox_t
{
	int bone;
	int group;
	vec3 bbmin;
	vec3 bbmax;
	int szhitboxnameindex;
	int m_iPad01[3];
	float m_flRadius;
	int m_iPad02[4];
};

struct mstudiohitboxset_t
{
	int	sznameindex;
	inline char* const GetName( void ) const { return ( (char*)this ) + sznameindex; }
	int	numhitboxes;
	int	hitboxindex;
	inline mstudiobbox_t*   GetHitbox( int i ) const { return (mstudiobbox_t*)( ( (byte*)this ) + hitboxindex ) + i; };
};

struct studiohdr_t
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	vec3 eyeposition;
	vec3 illumposition;
	vec3 hull_min;
	vec3 hull_max;
	vec3 view_bbmin;
	vec3 view_bbmax;
	int flags;
	int numbones;
	int boneindex;
	inline mstudiobone_t *GetBone( int i ) const { return (mstudiobone_t *)( ( (byte *)this ) + boneindex ) + i; };
	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
	mstudiohitboxset_t* GetHitboxSet( int i ) const
	{
		return (mstudiohitboxset_t*)( ( (char*)this ) + hitboxsetindex ) + i;
	}
	inline mstudiobbox_t* GetHitbox( int i, int set ) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet( set );

		if ( !s )
			return NULL;

		return s->GetHitbox( i );
	}
	inline int GetHitboxCount( int set ) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet( set );
		if ( !s )
			return 0;
		return s->numhitboxes;
	}
	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;
	mutable int activitylistversion;
	mutable int eventsindexed;
	int numtextures;
	int textureindex;
	int numcdtextures;
	int cdtextureindex;
	int numskinref;
	int numskinfamilies;
	int skinindex;
	int numbodyparts;
	int bodypartindex;
	int numlocalattachments;
	int localattachmentindex;
	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;
	int numflexdesc;
	int flexdescindex;
	int numflexcontrollers;
	int flexcontrollerindex;
	int numflexrules;
	int flexruleindex;
	int numikchains;
	int ikchainindex;
	int nummouths;
	int mouthindex;
	int numlocalposeparameters;
	int localposeparamindex;
	int surfacepropindex;
	int keyvalueindex;
	int keyvaluesize;
	int numlocalikautoplaylocks;
	int localikautoplaylockindex;
	float mass;
	int contents;
	int numincludemodels;
	int includemodelindex;
	mutable void *virtualModel;
	int szanimblocknameindex;
	int numanimblocks;
	int animblockindex;
	mutable void *animblockModel;
	int bonetablebynameindex;
	void *pVertexBase;
	void *pIndexBase;
	byte constdirectionallightdot;
	byte rootLOD;
	byte numAllowedRootLODs;
	byte unused[1];
	int unused4;
	int numflexcontrollerui;
	int flexcontrolleruiindex;
	float flVertAnimFixedPointScale;
	int unused3[1];
	int studiohdr2index;
	int unused2[1];
};

class IVEngineClient
{
public:
	void GetScreenSize( int32_t& w, int32_t& h )
	{
		return vfunc<void( __thiscall* )( IVEngineClient*, int32_t&, int32_t& )>( this, 5 )( this, w, h );
	}

	void ClientCmd( const char* cmd )
	{
		return vfunc<void( __thiscall* )( IVEngineClient*, const char* )>( this, 7 )( this, cmd );
	}

	int const GetLocalPlayer( )
	{
		return vfunc<int( __thiscall* )( IVEngineClient* )>( this, 12 )( this );
	}

	void GetViewAngles( vec3& vAngles )
	{
		return vfunc<void( __thiscall* )( IVEngineClient*, vec3& )>( this, 19 )( this, vAngles );
	}

	vec3 GetViewAngles( )
	{
		vec3 vAngles;
		vfunc<void( __thiscall* )( IVEngineClient*, vec3& )>( this, 19 )( this, vAngles );
		return vAngles;
	}

	void SetViewAngles( vec3& vAngles )
	{
		return vfunc<void( __thiscall* )( IVEngineClient*, vec3& )>( this, 20 )( this, vAngles );
	}

	bool IsInGame( )
	{
		return vfunc<bool( __thiscall* )( IVEngineClient* )>( this, 26 )( this );
	}

	bool IsConnected( )
	{
		return vfunc<bool( __thiscall* )( IVEngineClient* )>( this, 27 )( this );
	}

	const matrix4x4& WorldToScreenMatrix( )
	{
		return vfunc<matrix4x4&( __thiscall* )( IVEngineClient* )>( this, 37 )( this );
	}

	void ClientCmd_Unrestricted( const char* cmd )
	{
		return vfunc<void( __thiscall* )( IVEngineClient*, const char* )>( this, 107 )( this, cmd );
	}
};

class CGlobalVarsBase
{
public:
	float     realtime;                     // 0x0000
	int       framecount;                   // 0x0004
	float     absoluteframetime;            // 0x0008
	float     curtime;                      // 0x0010
	float     frametime;                    // 0x0014
	int       maxClients;                   // 0x0018
	int       tickcount;                    // 0x001C
	float     interval_per_tick;            // 0x0020
	float     interpolation_amount;         // 0x0024
	int       simTicksThisFrame;            // 0x0028
	int       network_protocol;             // 0x002C
	void*     pSaveData;                    // 0x0030
	bool      m_bClient;                    // 0x0031
	bool      m_bRemoteClient;              // 0x0032
};

class IClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable( int entindex ) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle( HANDLE hEntity ) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle( HANDLE hEntity ) = 0;
	virtual CBaseEntity* GetClientEntity( int entindex ) = 0;
	virtual CBaseEntity* GetClientEntityFromHandle( HANDLE hEntity ) = 0;
	virtual int NumberOfEntities( bool include_non_networkable ) = 0;
	virtual int GetHighestEntityIndex( ) = 0;
	virtual void SetMaxEntities( int maxents ) = 0;
	virtual int GetMaxEntities( ) = 0;
};

class IVRenderView
{
public:
	inline void SetColorModulation( float r, float g, float b )
	{
		float color[3] = { r, g, b };
		return vfunc<void( __thiscall* )( IVRenderView*, float[] )>( this, 7 )( this, color );
	}
};

class IMaterial
{
public:
	inline void SetMaterialVarFlag( MaterialVarFlags_t flag, bool on )
	{
		return vfunc<void( __thiscall* )( IMaterial*, MaterialVarFlags_t, bool )>( this, 29 )( this, flag, on );
	}

	inline void ColorModuleate( float r, float g, float b )
	{
		return vfunc<void( __thiscall* )( IMaterial*, float, float, float )>( this, 28 )( this, r, g, b );
	}
};

class IMaterialSystem
{
public:
	IMaterial* FindMaterial( char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0 )
	{
		return vfunc<IMaterial*( __thiscall* )( IMaterialSystem*, const char*, const char*, bool, const char* )>( this, 71 )( this, pMaterialName, pTextureGroupName, complain, pComplainPrefix );
	}
};

class CModelInfo
{
public:
	inline void* GetModel( int nIndex )
	{
		return vfunc<void*( __thiscall* )( void*, int )>( this, 1 )( this, nIndex );
	}

	studiohdr_t* GetStudioModel( const model_t* model )
	{
		return vfunc<studiohdr_t*( __stdcall* )( const model_t* )>( this, 30 )( model );
	}
};

class IVModelRenderer
{
public:
	void ForcedMaterialOverride( IMaterial* newMaterial, int nOverrideType = 0 )
	{
		return vfunc<void( __thiscall* )( IVModelRenderer*, IMaterial*, int )>( this, 1 )( this, newMaterial, nOverrideType );
	}
};

class IPanel
{
public:
	const char* GetName( uint32_t iPanel )
	{
		return vfunc<const char*( __thiscall* )( IPanel*, uint32_t )>( this, 36 )( this, iPanel );
	}
};

class ISurface
{
private:
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
public:
	inline void DrawSetColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
	{
		const Color color{ r, g, b, a };
		return vfunc<void( __thiscall* )( ISurface*, Color )>( this, 10 )( this, color );
	}

	inline void DrawFilledRect( int x0, int y0, int x1, int y1 )
	{
		return vfunc<void( __thiscall* )( ISurface*, int, int, int, int )>( this, 12 )( this, x0, y0, x1, y1 );
	}

	inline void DrawOutlinedRect( int x0, int y0, int x1, int y1 )
	{
		return vfunc<void( __thiscall* )( ISurface*, int, int, int, int )>( this, 14 )( this, x0, y0, x1, y1 );
	}

	inline void DrawLine( int x0, int y0, int x1, int y1 )
	{
		return vfunc<void( __thiscall* )( ISurface*, int, int, int, int )>( this, 15 )( this, x0, y0, x1, y1 );
	}

	inline void DrawSetTextFont( HFont font )
	{
		return vfunc<void( __thiscall* )( ISurface*, HFont )>( this, 17 )( this, font );
	}

	inline void DrawSetTextColor( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
	{
		return vfunc<void( __thiscall* )( ISurface*, uint8_t, uint8_t, uint8_t, uint8_t )>( this, 19 )( this, r, g, b, a );
	}

	inline void DrawSetTextPos( uint32_t x, uint32_t y )
	{
		return vfunc<void( __thiscall* )( ISurface*, uint32_t, uint32_t )>( this, 20 )( this, x, y );
	}

	inline void DrawPrintText( const wchar_t* text, int textLen, FontDrawType_t DrawType = FONT_DRAW_DEFAULT )
	{
		return vfunc<void( __thiscall* )( ISurface*, const wchar_t*, int, FontDrawType_t )>( this, 22 )( this, text, textLen, DrawType );
	}

	inline void UnlockCursor( )
	{
		return vfunc<void( __thiscall* )( ISurface* )>( this, 58 )( this );
	}

	inline void LockCursor( )
	{
		return vfunc<void( __thiscall* )( ISurface* )>( this, 59 )( this );
	}

	inline HFont Create_Font( )
	{
		return vfunc<HFont( __thiscall* )( ISurface* )>( this, 63 )( this );
	}

	inline bool SetFontGlyphSet( HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0 )
	{
		return vfunc<bool( __thiscall* )( ISurface*, HFont, const char*, int, int, int, int, int, int, int )>( this, 64 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax );
	}

	inline void GetTextSize( unsigned long font, const wchar_t *text, int &wide, int &tall )
	{
		return vfunc< void( __thiscall* )( void*, HFont, const wchar_t*, int&, int& ) >( this, 72 )( this, font, text, wide, tall );
	}

	//OnScreenSizeChanged = 108

	inline void DrawFilledRectFade( int x0, int y0, int x1, int y1, uint32_t alpha0, uint32_t alpha1, bool horizontal )
	{
		return vfunc<void( __thiscall* )( ISurface*, int, int, int, int, uint32_t, uint32_t, bool )>( this, 115 )( this, x0, y0, x1, y1, alpha0, alpha1, horizontal );
	}
};

class CMoveData
{
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	unsigned long	m_nPlayerHandle;
	int				m_nImpulseCommand;
	vec3			m_vecViewAngles;
	vec3			m_vecAbsViewAngles;
	int				m_nButtons;
	int				m_nOldButtons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	vec3			m_vecVelocity;
	vec3			m_vecOldVelocity;
	float			somefloat;
	vec3			m_vecAngles;
	vec3			m_vecOldAngles;

	float			m_outStepHeight;
	vec3			m_outWishVel;
	vec3			m_outJumpVel;

	vec3			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;

	void			SetAbsOrigin( const vec3 &vec );
	const vec3&     GetAbsOrigin( ) const;

private:
	vec3			m_vecAbsOrigin;		// edict::origin
};

class IGameMovement
{
public:
	virtual			~IGameMovement( void ) { }

	virtual void	ProcessMovement( CBaseEntity *pPlayer, CMoveData *pMove ) = 0;
	virtual void	Reset( void ) = 0;
	virtual void	StartTrackPredictionErrors( CBaseEntity *pPlayer ) = 0;
	virtual void	FinishTrackPredictionErrors( CBaseEntity *pPlayer ) = 0;
	virtual void	DiffPrint( char const *fmt, ... ) = 0;

	virtual vec3 const&	GetPlayerMins( bool ducked ) const = 0;
	virtual vec3 const&	GetPlayerMaxs( bool ducked ) const = 0;
	virtual vec3 const&   GetPlayerViewOffset( bool ducked ) const = 0;

	virtual bool			IsMovingPlayerStuck( void ) const = 0;
	virtual CBaseEntity*	GetMovingPlayer( void ) const = 0;
	virtual void			UnblockPusher( CBaseEntity* pPlayer, CBaseEntity *pPusher ) = 0;

	virtual void SetupMovementBounds( CMoveData *pMove ) = 0;
};

class IMoveHelper
{
private:
	virtual void UnknownVirtual0( ) = 0;
public:
	virtual void SetHost( CBaseEntity* host ) = 0;
};

class IPrediction
{
public:
	void SetupMove( CBaseEntity* player, CUserCmd* cmd, IMoveHelper* pHelper, CMoveData* move )
	{
		vfunc<void( __thiscall* )( IPrediction*, CBaseEntity*, CUserCmd*, IMoveHelper*, CMoveData* )>( this, 19 )( this, player, cmd, pHelper, move );
	}
	void FinishMove( CBaseEntity* player, CUserCmd* cmd, CMoveData* move )
	{
		vfunc<void( __thiscall* )( IPrediction*, CBaseEntity*, CUserCmd*, CMoveData* )>( this, 20 )( this, player, cmd, move );
	}
};

class ICollideable
{
public:
	virtual ~ICollideable( ) { };

	SolidFlags_t GetSolid( )
	{
		return vfunc<SolidFlags_t( __thiscall* )( ICollideable* )>( this, 11 )( this );
	}

	unsigned short GetSolidFlags( )
	{
		return vfunc<unsigned short( __thiscall* )( ICollideable* )>( this, 12 )( this );
	}
};

class IClientUnknown
{
public:
	virtual ~IClientUnknown( ) { };

	ICollideable* GetCollideable( )
	{
		return vfunc<ICollideable*( __thiscall* )( IClientUnknown* )>( this, 3 )( this );
	}
};

class IClientThinkable
{
public:
	virtual ~IClientThinkable( ) { };
};

class IClientRenderable
{
public:
	virtual ~IClientRenderable( ) { };

	inline model_t* GetModel( )
	{
		return vfunc<model_t*( __thiscall* )( IClientRenderable* )>( this, 8 )( this );
	}

	inline int DrawModel( int flags, float alpha = 1.f )
	{
		return vfunc<int( __thiscall* )( IClientRenderable*, int, float )>( this, 9 )( this, flags, alpha );
	}

	inline bool SetupBones( matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		return vfunc<bool( __thiscall* )( IClientRenderable*, matrix3x4*, int, int, float )>( this, 13 )( this, pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
};

class IClientNetworkable
{
public:
	virtual ~IClientNetworkable( ) { };

	inline ClientClass* GetClientClass( )
	{
		return vfunc<ClientClass*( __thiscall* )( IClientNetworkable* )>( this, 1 )( this );
	}

	inline bool IsDormant( )
	{
		return vfunc<bool( __thiscall* )( IClientNetworkable* )>( this, 7 )( this );
	}

	inline const int GetIndex( )
	{
		return vfunc<int( __thiscall* )( IClientNetworkable* )>( this, 8 )( this );
	}
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual ~IClientEntity( ) { };
};

#define DEFCOLOR_SRC(name, r, g, b) constexpr static color_t name (uint8_t a = 255) { return color_t(r, g, b, a); }

class ColorF
{
	float R, G, B, A;
public:
	constexpr ColorF( ) : R( 0.f ), G( 0.f ), B( 0.f ), A( 0.f ) { }

	constexpr ColorF( float r, float g, float b, float a ) : R( r ), G( g ), B( b ), A( a ) { }

	constexpr ColorF( float r, float g, float b ) : R( r ), G( g ), B( b ), A( 1.f ) { }

	float r( ) const { return R; }
	float g( ) const { return G; }
	float b( ) const { return B; }
	float a( ) const { return A; }
	void r( float _r ) { R = _r; }
	void g( float _g ) { G = _g; }
	void b( float _b ) { B = _b; }
	void a( float _a ) { A = _a; }

	const float* raw( ) const { return &R; }
};

class color_t
{
	uint8_t R, G, B, A = 255;
public:
	constexpr color_t( ) : R( 0 ), G( 0 ), B( 0 ), A( 255 ) { }

	constexpr color_t( int r, int g, int b, int a ) : R( r ), G( g ), B( b ), A( a ) { }

	constexpr color_t( int r, int g, int b ) : R( r ), G( g ), B( b ), A( 255 ) { }

	uint8_t r( ) const { return R; }
	uint8_t g( ) const { return G; }
	uint8_t b( ) const { return B; }
	uint8_t a( ) const { return A; }
	void r( uint8_t _r ) { R = _r; }
	void g( uint8_t _g ) { G = _g; }
	void b( uint8_t _b ) { B = _b; }
	void a( uint8_t _a ) { A = _a; }

	uint8_t* r_ptr( ) { return &R; }
	uint8_t* g_ptr( ) { return &G; }
	uint8_t* b_ptr( ) { return &B; }
	uint8_t* a_ptr( ) { return &A; }

	DEFCOLOR_SRC( black, 0, 0, 0 );
	DEFCOLOR_SRC( white, 255, 255, 255 );

	color_t& operator =( const color_t& c )
	{
		R = c.r( );
		G = c.g( );
		B = c.b( );
		A = c.a( );
		return *this;
	}

	color_t operator+( const color_t& v ) const
	{
		return color_t( R + v.R, G + v.G, B + v.B, A + v.A );
	}

	explicit operator bool( ) const noexcept
	{
		return R > 0 || G > 0 || B > 0 || A > 0;
	}

	bool operator==( const color_t& c ) const
	{
		return R == c.r( ) && G == c.g( ) && B == c.b( );
	}

	static color_t FromHSB( float hue, float saturation, float brightness )
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * ( 1.0f - saturation );
		float q = brightness * ( 1.0f - saturation * f );
		float t = brightness * ( 1.0f - saturation * ( 1.0f - f ) );

		if ( h < 1 )
		{
			return color_t(
				(unsigned char)( brightness * 255 ),
				(unsigned char)( t * 255 ),
				(unsigned char)( p * 255 )
			);
		}
		if ( h < 2 )
		{
			return color_t(
				(unsigned char)( q * 255 ),
				(unsigned char)( brightness * 255 ),
				(unsigned char)( p * 255 )
			);
		}
		if ( h < 3 )
		{
			return color_t(
				(unsigned char)( p * 255 ),
				(unsigned char)( brightness * 255 ),
				(unsigned char)( t * 255 )
			);
		}
		if ( h < 4 )
		{
			return color_t(
				(unsigned char)( p * 255 ),
				(unsigned char)( q * 255 ),
				(unsigned char)( brightness * 255 )
			);
		}
		if ( h < 5 )
		{
			return color_t(
				(unsigned char)( t * 255 ),
				(unsigned char)( p * 255 ),
				(unsigned char)( brightness * 255 )
			);
		}
		return color_t(
			(unsigned char)( brightness * 255 ),
			(unsigned char)( p * 255 ),
			(unsigned char)( q * 255 )
		);
	}

	/*fColor ToFColor( ) const
	{
	return fColor( R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f );
	}*/

	ColorF base( )
	{
		//returned address of temp value before :>
		return ColorF( R / 255.0f, G / 255.0f, B / 255.0f );
	}

	static color_t blend( const color_t& first, const color_t& second, float t )
	{
		return color_t(
			first.r( ) + static_cast<int>( t * ( second.r( ) - first.r( ) ) ),
			first.g( ) + static_cast<int>( t * ( second.g( ) - first.g( ) ) ),
			first.b( ) + static_cast<int>( t * ( second.b( ) - first.b( ) ) ),
			first.a( ) + static_cast<int>( t * ( second.a( ) - first.a( ) ) )
		);
	}

	float Hue( )
	{
		typedef struct
		{
			float h, s, v;
		} hsv;
		hsv out;

		float min = static_cast<float>( R < G ? R : G );
		min = static_cast<float>( min < B ? min : B );

		float max = static_cast<float>( R > G ? R : G );
		max = static_cast<float>( max > B ? max : B );

		out.v = max;
		float delta = max - min;
		if ( delta < 0.0010f )
		{
			out.s = 0.f;
			out.h = 0.f;
			return out.h;
		}
		if ( max > 0.0f )
		{
			out.s = delta / max;
		}
		else
		{
			out.s = 0.0f;
			out.h = NAN;
			return out.h;
		}
		if ( R >= max )
			out.h = static_cast<float>( G - B ) / delta;
		else if ( G >= max )
			out.h = 2.0f + static_cast<float>( B - R ) / delta;
		else
			out.h = 4.0f + static_cast<float>( R - G ) / delta;

		out.h *= 60.0f;
		out.h /= 360.f;

		if ( out.h < 0.0f )
			out.h += 360.0f;

		return out.h;
	}

	float brightness( )
	{
		typedef struct
		{
			float h, s, v;
		} hsv;
		hsv out;

		float min = static_cast<float>( R < G ? R : G );
		min = static_cast<float>( min < B ? min : B );

		float max = static_cast<float>( R > G ? R : G );
		max = static_cast<float>( max > B ? max : B );

		out.v = max;
		float delta = max - min;
		if ( delta < 0.0010f )
		{
			out.s = 0.f;
			out.h = 0.f;
			return out.h;
		}
		if ( max > 0.0f )
		{
			out.s = delta / max;
		}
		else
		{
			out.s = 0.0f;
			out.h = NAN;
			return out.h;
		}
		if ( R >= max )
			out.h = static_cast<float>( G - B ) / delta;
		else if ( G >= max )
			out.h = 2.0f + static_cast<float>( B - R ) / delta;
		else
			out.h = 4.0f + static_cast<float>( R - G ) / delta;

		out.h *= 60.0f;
		out.h /= 360.f;

		if ( out.h < 0.0f )
			out.h += 360.0f;

		return out.v;
	}

	float saturation( )
	{
		typedef struct
		{
			float h, s, v;
		} hsv;
		hsv out;

		float min = static_cast<float>( R < G ? R : G );
		min = static_cast<float>( min < B ? min : B );

		float max = static_cast<float>( R > G ? R : G );
		max = static_cast<float>( max > B ? max : B );

		out.v = max;
		float delta = max - min;
		if ( delta < 0.0010f )
		{
			out.s = 0.f;
			out.h = 0.f;
			return out.h;
		}
		if ( max > 0.0f )
		{
			out.s = delta / max;
		}
		else
		{
			out.s = 0.0f;
			out.h = NAN;
			return out.h;
		}
		if ( R >= max )
			out.h = static_cast<float>( G - B ) / delta;
		else if ( G >= max )
			out.h = 2.0f + static_cast<float>( B - R ) / delta;
		else
			out.h = 4.0f + static_cast<float>( R - G ) / delta;

		out.h *= 60.0f;
		out.h /= 360.f;

		if ( out.h < 0.0f )
			out.h += 360.0f;

		return out.s;
	}

	void ScaleColor( float scaleR, float scaleG, float scaleB )
	{
		R = static_cast<int>( scaleR * R );
		G = static_cast<int>( scaleG * G );
		B = static_cast<int>( scaleB * B );
	}

	void ScaleAlpha( float scaleA )
	{
		A = static_cast<int>( scaleA * A );
	}
};


class ConVar
{
public:
	float GetFloat( )
	{
		typedef float( __thiscall* OriginalFn )( void* );
		return vfunc<OriginalFn>( this, 10 )( this );
	}

	int GetInt( )
	{
		typedef int( __thiscall* OriginalFn )( void* );
		return vfunc<OriginalFn>( this, 11 )( this );
	}

	void SetValue( const char* value )
	{
		typedef void( __thiscall* OriginalFn )( void*, const char* );
		return vfunc<OriginalFn>( this, 12 )( this, value );
	}

	void SetValue( float value )
	{
		typedef void( __thiscall* OriginalFn )( void*, float );
		return vfunc<OriginalFn>( this, 13 )( this, value );
	}

	void SetValue( int value )
	{
		typedef void( __thiscall* OriginalFn )( void*, int );
		return vfunc<OriginalFn>( this, 14 )( this, value );
	}
};

class ICVar
{
public:
	ConVar* FindVar( const char *var_name )
	{
		typedef ConVar*( __thiscall* OriginalFn )( void*, const char*  );
		return vfunc<OriginalFn>( this, 12 )( this, var_name );
	}
};

class CViewSetup
{
public:
	uint8_t pad_0000[52]; //0x0000
	float fov; //0x0034
};

class CInput
{
public:
	uint8_t pad_0000[161]; //0x0000
	bool m_fCameraInThirdPerson; //0x00A1
	uint8_t pad_00A2[2]; //0x00A2
	vec3 m_vecCameraOffset; //0x00A4
};
