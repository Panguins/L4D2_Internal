#pragma once

enum classID
{
	CWorld = 260,
	CTerrorPlayer = 231,
	SurvivorBot = 274,
	CCSTeam = 50,
	CTerrorPlayerResource = 232,
	CVoteController = 252,
	CTerrorGameRulesProxy = 228,
	CBeamSpotlight = 32,
	CSprite = 177,
	CWeaponAmmoSpawn = 255,
	CWeaponSpawn = 259,
	CFuncPlayerInfectedClip = 86,
	CFuncSimpleLadder = 90,
	CRopeKeyframe = 156,
	C_BaseEntity = 15,
	CFogController = 75,
	CShadowControl = 160,
	CEnvTonemapController = 65,
	CEnvDetailController = 57,
	CWaterLODControl = 254,
	CTerrorViewModel = 233,
	CPhysicsProp = 127,
	CSteamJet = 180,
	CPistol = 131,
	CBasePropDoor = 21,
	CFuncAreaPortalWindow = 78,
	Infected = 263,
	CPostProcessController = 138,
	CParticleSystem = 124,
	CShotgun_Chrome = 161,
	CColorCorrection = 45,
	CFuncOccluder = 84,
	CFirstAidKit = 73,
	CDynamicProp = 52,
	CSurvivorPosition = 184,
	CBaseUpgradeItem = 29,
	CSpotlightEnd = 176,
	CPropDoorRotatingCheckpoint = 143,
	CBeam = 31,
	CFireSmoke = 70,
	CBreakable = 35,
	CSceneEntity = 158,
	Smoker = 269,
	CSmokerClaw = 166,
	CTongue = 245,
	Hunter = 262,
	CHunterClaw = 100,
	CCSRagdoll = 49,
	Jockey = 264,
	CJockeyClaw = 112,
	CBoomerClaw = 34,
	Charger = 99,
	CChargerClaw = 41,
	Spitter = 271,
	CSpitterClaw = 174,
	CSpitterProjectile = 175,
	CInsectSwarm = 104,
	Witch = 276,
	Tank = 275,
	CTankClaw = 186,
	Boomer = 0,
	CBaseCSGrenadeProjectile = 13,
	CMolotov = 118,
	CMolotovProjectile = 119,
	CInferno = 101,
	CEntityFlame = 55,
	CPipeBomb = 129,
	CPipeBombProjectile = 130,
	CItem_Adrenaline = 105,
	CAutoShotgun = 2,
	CTerrorMeleeWeapon = 230,
	CChainsaw = 39,
	CColaBottles = 44,
	CItemDefibrillator = 109,
	CFireworkCrate = 72,
	CGasCan = 94,
	CGnome = 95,
	CGrenadeLauncher = 96,
	CSniperRifle = 171,
	COxygenTank = 120,
	CPainPills = 121,
	CMagnumPistol = 116,
	CPropaneTank = 142,
	CPumpShotgun = 148,
	CFireCrackerBlast = 69,
	CAssaultRifle = 1,
	CRifle_AK47 = 152,
	CRifle_Desert = 153,
	CRifle_M60 = 154,
	CRifle_SG552 = 155,
	CShotgun_SPAS = 162,
	CSubMachinegun = 181,
	CSMG_MP5 = 164,
	CSMG_Silenced = 165,
	CSniper_AWP = 168,
	CSniper_Military = 169,
	CSniper_Scout = 170,
	CItemUpgradePackExplosive = 110,
	CItemUpgradePackIncendiary = 111,
	CItem_VomitJar = 106,
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

enum controls_t
{
	IN_ATTACK = ( 1 << 0 ),
	IN_JUMP = ( 1 << 1 ),
	IN_DUCK = ( 1 << 2 ),
	IN_FORWARD = ( 1 << 3 ),
	IN_BACK = ( 1 << 4 ),
	IN_USE = ( 1 << 5 ),
	IN_CANCEL = ( 1 << 6 ),
	IN_LEFT = ( 1 << 7 ),
	IN_RIGHT = ( 1 << 8 ),
	IN_MOVELEFT = ( 1 << 9 ),
	IN_MOVERIGHT = ( 1 << 10 ),
	IN_ATTACK2 = ( 1 << 11 ),
	IN_RUN = ( 1 << 12 ),
	IN_RELOAD = ( 1 << 13 ),
	IN_ALT1 = ( 1 << 14 ),
	IN_ALT2 = ( 1 << 15 ),
	IN_SCORE = ( 1 << 16 ),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = ( 1 << 17 ),	// Player is holding the speed key
	IN_WALK = ( 1 << 18 ),	// Player holding walk key
	IN_ZOOM = ( 1 << 19 ),	// Zoom key for HUD zoom
	IN_WEAPON1 = ( 1 << 20 ),	// weapon defines these bits
	IN_WEAPON2 = ( 1 << 21 ),	// weapon defines these bits
	IN_BULLRUSH = ( 1 << 22 ),
};

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)	// player jumping out of water
#define FL_ONTRAIN				(1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)	// Is a player
#define FL_FAKECLIENT			(1<<8)	// Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<9)	// In water

#define FW_DONTCARE         0
#define FW_THIN             100
#define FW_EXTRALIGHT       200
#define FW_LIGHT            300
#define FW_NORMAL           400
#define FW_MEDIUM           500
#define FW_SEMIBOLD         600
#define FW_BOLD             700
#define FW_EXTRABOLD        800
#define FW_HEAVY            900

enum FontFlags_t
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};


enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST = 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,	// This is something may be collideable but fires touch functions
								// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
								FSOLID_NOT_STANDABLE = 0x0010,	// You can't stand on this
								FSOLID_VOLUME_CONTENTS = 0x0020,	// Contains volumetric contents (like water)
								FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
								FSOLID_USE_TRIGGER_BOUNDS = 0x0080,	// Uses a special trigger bounds separate from the normal OBB
								FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
								FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects

								FSOLID_MAX_BITS = 10
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = ( 1 << 0 ),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
	MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),
	MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
	MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
	MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
	MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
	MATERIAL_VAR_ALPHATEST = ( 1 << 8 ),
	MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
	MATERIAL_VAR_MODEL = ( 1 << 11 ),
	MATERIAL_VAR_FLAT = ( 1 << 12 ),
	MATERIAL_VAR_NOCULL = ( 1 << 13 ),
	MATERIAL_VAR_NOFOG = ( 1 << 14 ),
	MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
	MATERIAL_VAR_DECAL = ( 1 << 16 ),
	MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ),
	MATERIAL_VAR_UNUSED = ( 1 << 18 ),
	MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
	MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ),
	MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
	MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ),
	MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
	MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
	MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
	MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
};