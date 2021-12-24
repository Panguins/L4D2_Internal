#pragma once

class CBaseEntity;
class IClientNetworkable;
class IClientEntity;
class IClientUnknown;
class ICollideable;

#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

class VectorAligned : public vec3
{
public:
	VectorAligned( )
	{
	}

	VectorAligned( const vec3& vec )
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

struct Ray_t
{
	Ray_t( )
	{
	}

	VectorAligned m_Start;
	VectorAligned m_Delta;
	VectorAligned m_StartOffset;
	VectorAligned m_Extents;
	const matrix3x4* m_pWorldAxisTransform;
	bool m_IsRay;
	bool m_IsSwept;

	void Init( vec3 vecStart, vec3 vecEnd )
	{
		m_Delta = VectorAligned( vecEnd - vecStart );
		m_IsSwept = ( m_Delta.length_sqr( ) != 0 );
		m_Extents.is_zero( );
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.is_zero( );
		m_Start = vecStart;
	}

	void Init( const vec3& start, const vec3& end, const vec3& mins, const vec3& maxs )
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = nullptr;
		m_IsSwept = m_Delta.length_sqr( ) != 0;

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = m_Extents.length_sqr( ) < 1e-6;

		m_StartOffset = mins + maxs;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

struct cplane_t
{
	vec3 normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity( IClientEntity* pEntityHandle, int contentsMask )
	{
		return !( pEntityHandle == pSkip );
	}

	virtual TraceType_t GetTraceType( )
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual bool ShouldHitEntity( IClientEntity* pEntityHandle, int contentsMask )
	{
		return !( pEntityHandle == pSkip );
	}

	virtual TraceType_t GetTraceType( ) const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities( void* pPassEnt1, void* pPassEnt2 )
	{
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity( IClientEntity* pEntityHandle, int contentsMask )
	{
		return !( pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2 );
	}

	virtual TraceType_t GetTraceType( ) const
	{
		return TRACE_EVERYTHING;
	}

	void* pPassEntity1;
	void* pPassEntity2;
};

typedef bool( *ShouldHitFunc_t )( void* pHandleEntity, int contentsMask );

class CTraceFilterSimple : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	CTraceFilterSimple( const void* passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL );
	virtual bool ShouldHitEntity( void* pHandleEntity, int contentsMask );

	virtual void SetPassEntity( const void* pPassEntity )
	{
		m_pPassEnt = pPassEntity;
	}

	virtual void SetCollisionGroup( int iCollisionGroup )
	{
		m_collisionGroup = iCollisionGroup;
	}

	const void* GetPassEntity( void )
	{
		return m_pPassEnt;
	}

private:
	const void* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};

class CBaseTrace
{
public:
	vec3 startpos;
	vec3 endpos;
	cplane_t plane;

	float fraction;

	int contents;
	unsigned short dispFlags;

	bool allsolid;
	bool startsolid;

	CBaseTrace( )
	{
	}
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld( ) const;
	bool DidHitNonWorldEntity( ) const;
	int GetEntityIndex( ) const;
	bool DidHit( ) const;
	bool IsVisible( ) const;

public:

	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	CBaseEntity* m_pEnt;
	int hitbox;

	CGameTrace( )
	{
	}

private:
	CGameTrace( const CGameTrace& vOther );
};

typedef CGameTrace trace_t;

class ITraceListData
{
public:
	virtual ~ITraceListData( )
	{
	}

	virtual void Reset( ) = 0;
	virtual bool IsEmpty( ) = 0;
	virtual bool CanTraceRay( const Ray_t& ray ) = 0;
};

class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity( void* pHandleEntity ) = 0;
};

struct BrushSideInfo_t
{
	vec3 plane; // The plane of the brush side
	float planec;
	unsigned short bevel; // Bevel plane?
	unsigned short thin; // Thin?
};

enum DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int GetPointContents( const vec3& vecAbsPosition, int contentsMask = MASK_ALL, void** ppEntity = NULL ) = 0;

	// Returns the contents mask of the world only @ the world-space position (static props are ignored)
	virtual int GetPointContents_WorldOnly( const vec3& vecAbsPosition, int contentsMask = MASK_ALL ) = 0;

	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual int GetPointContents_Collideable( ICollideable* pCollide, const vec3& vecAbsPosition ) = 0;

	// Traces a ray against a particular entity
	virtual void ClipRayToEntity( const Ray_t& ray, unsigned int fMask, void* pEnt, trace_t* pTrace ) = 0;

	// Traces a ray against a particular entity
	virtual void ClipRayToCollideable( const Ray_t& ray, unsigned int fMask, ICollideable* pCollide, trace_t* pTrace ) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void TraceRay( const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace ) = 0;

	// A version that sets up the leaf and entity lists and allows you to pass those in for collision.
	virtual void SetupLeafAndEntityListRay( const Ray_t& ray, ITraceListData* pTraceData ) = 0;
	virtual void SetupLeafAndEntityListBox( const vec3& vecBoxMin, const vec3& vecBoxMax, ITraceListData* pTraceData ) = 0;
	virtual void TraceRayAgainstLeafAndEntityList( const Ray_t& ray, ITraceListData* pTraceData, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace ) = 0;

	// A version that sweeps a collideable through the world
	// abs start + abs end represents the collision origins you want to sweep the collideable through
	// vecAngles represents the collision angles of the collideable during the sweep
	virtual void SweepCollideable( ICollideable* pCollide, const vec3& vecAbsStart, const vec3& vecAbsEnd,
								   const vec3& vecAngles, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace ) = 0;

	// Enumerates over all entities along a ray
	// If triggers == true, it enumerates all triggers along a ray
	virtual void EnumerateEntities( const Ray_t& ray, bool triggers, IEntityEnumerator* pEnumerator ) = 0;

	// Same thing, but enumerate entitys within a box
	virtual void EnumerateEntities( const vec3& vecAbsMins, const vec3& vecAbsMaxs, IEntityEnumerator* pEnumerator ) = 0;

	// Convert a handle entity to a collideable.  Useful inside enumer
	virtual ICollideable* GetCollideable( void* pEntity ) = 0;

	// HACKHACK: Temp for performance measurments
	virtual int GetStatByIndex( int index, bool bClear ) = 0;

	//finds brushes in an AABB, prone to some false positives
	virtual void pad1( ) = 0;

	//Creates a CPhysCollide out of all displacements wholly or partially contained in the specified AABB
	virtual void GetCollidableFromDisplacementsInAABB( ) = 0;

	// gets the number of displacements in the world
	virtual int GetNumDisplacements( ) = 0;

	// gets a specific diplacement mesh
	virtual void GetDisplacementMesh( int nIndex, void* pMeshTriList ) = 0;

	//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
	virtual bool pad2( ) = 0;

	virtual bool PointOutsideWorld( const vec3& ptTest ) = 0; //Tests a point to see if it's outside any playable area

																// Walks bsp to find the leaf containing the specified point
	virtual int GetLeafContainingPoint( const vec3& ptTest ) = 0;

	virtual ITraceListData* AllocTraceListData( ) = 0;
	virtual void FreeTraceListData( ITraceListData* ) = 0;

	/// Used only in debugging: get/set/clear/increment the trace debug counter. See comment below for details.
	virtual int GetSetDebugTraceCounter( int value, DebugTraceCounterBehavior_t behavior ) = 0;
};