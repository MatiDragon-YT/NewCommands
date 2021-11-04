#pragma once
#include "pch.h"

#define ANDROID

enum eGameIdent
{
	GTA3,
	GTAVC,
	GTASA,
	GTALCS,
	GTAVCS
};

enum eGameVerInternal
{
	VER_NONE,
	VER_GTA3_1_4,
	VER_GTAVC_1_03,
	VER_GTASA_1_00,
	VER_GTASA_1_01,
	VER_GTASA_1_02,
	VER_GTASA_1_03,
	VER_GTASA_1_05,
	VER_GTASA_1_06,
	VER_GTASA_1_05_GER,
	VER_GTASA_1_07,
	VER_GTA3_1_6,
	VER_GTAVC_1_06,
	VER_GTASA_1_08,
	VER_GTALCS_2_2,
	VER_GTA3_1_8_OR_HIGHER,
	VER_GTAVC_1_09_OR_HIGHER,
	VER_GTASA_2_00_OR_HIGHER,
	VER_GTALCS_2_4_OR_HIGHER,
	VER_GTALCS_PSP_1_05_OR_HIGHER,
	VER_GTAVCS_PSP_1_02_OR_HIGHER
};

struct cleo_ifs_t
{
	// interface v1

	uint32_t(*GetInterfaceVersion)();	// currently = 1

	eGameIdent(*GetGameIdentifier)();
	eGameVerInternal(*GetGameVersionInternal)();

	const char* (*GetPackageName)();			// PSP: DiscId
	const char* (*GetPackageVersionStr)();		// PSP: DiscVersion
	uint32_t(*GetPackageVersionCode)();		// PSP: DiscVersionCode

	const char* (*GetCleoStorageDir)();
	const char* (*GetCleoPluginLoadDir)();		// PSP: same as StorageDir

	void (*PrintToCleoLog)(const char* str);

	const char* (*GetMainLibraryFileName)();
	void* (*GetMainLibraryLoadAddress)();

	typedef struct { uint32_t rva, size; } section_t;

	uint32_t(*GetMainLibraryExecutableSections)(section_t* sections, uint32_t size);
	void* (*FindExecutablePattern)(const char* pattern, uint32_t index);

#ifdef ANDROID
	void* (*GetMainLibrarySymbol)(const char* name);
#endif

	void (*MemWriteArr)(void* addr, uint8_t* arr, uint32_t size, bool protect);

#ifdef ANDROID
	void (*ReplaceThumbCall)(void* addr, void* func_to);
	void (*HookThumbFunc)(void* func, uint32_t startSize, void* func_to, void** func_orig);
	void (*ReplaceArmCall)(void* addr, void* func_to);
	void (*HookArmFunc)(void* func, uint32_t startSize, void* func_to, void** func_orig);
#else
	void (*ReplaceMipsCall)(void* addr, void* func_to);
	void (*HookMipsFunc)(void* func, uint32_t startSize, void* func_to, void** func_orig);
#endif

	// ip is a pointer inside scriptHandle structure, the structure is different in all games
	typedef void (*opcode_handler_t)(void* scriptHandle, uint32_t* ip, uint16_t opcode, const char* name);

	bool (*RegisterOpcode)(uint16_t opcode, opcode_handler_t handler);
	bool (*RegisterOpcodeFunction)(const char* name, opcode_handler_t handler);

	typedef struct { union { int32_t i; uint32_t u; float f; }; } data_t;

	data_t* (*ReadParam)(void* scriptHandle); // returned pointer has the data until next param read call (points into game's ScriptParams)
	data_t* (*GetPointerToScriptVar)(void* scriptHandle); // returned pointer is valid all the time (points into game's script handle or into global seg)
	bool (*ReadString8byte)(void* scriptHandle, char* buf, uint32_t size);
	bool (*ReadStringLong)(void* scriptHandle, char* buf, uint32_t size);	// supported only in III/VC/SA (so far)

	bool (*IsParamListEnd)(uint32_t ip);
	void (*SkipParamListEnd)(uint32_t* ip);

	void* (*GetRealCodePointer)(uint32_t ip);	// script ip to ptr, script ip has to be gained from handle->ip as *ip (handler param)
	uint32_t(*GetIpUsingRealCodePointer)(void* realPtr);	// ptr to script ip
	void* (*GetRealLabelPointer)(void* scriptHandle, uint32_t offset);	// offset has to be represented in the raw way i.e. opcode label param encoding
};

union ScriptVar
{
	int n;
	float f;
	char* c;
	void* p;
	unsigned long d;
};

typedef struct SA_RunningScript SA_Script;
typedef struct VC_RunningScript VC_Script;
typedef struct III_RunningScript III_Script;
typedef struct LCS_RunningScript LCS_Script;

struct SA_RunningScript
{
	SA_RunningScript*	next;//0x00
	SA_RunningScript*	prev;//0x04
	char				scriptName[8];//0x08
	char*				baseIp;//0x10
	char*				ip;//0x14
	char*				gosub_stack[8];//0x18
	short				sp;//0x38
	short				field_3A;
	ScriptVar			LocalVars[42];//0x3C
	bool				isActive;//0xE4
	bool				condResult;//0xE5
	bool				missionCleanupFlag;//0xE6
	bool				isExternal;//0xE7
	bool				textBlockOverride;//0xE8
	char				field_E9;
	char				field_EA;
	char				field_EB;
	int					wakeTime;//0xEC
	short				logicalOp;//0xF0
	bool				notFlag;//0xF2
	bool				wastedBustedCheckEnabled;//0xF3
	bool				wastedOrBusted;//0xF4
	char				field_F5;
	short				field_F6;
	char*				sceneSkip;//0xF8
	bool				missionFlag;//0xFC
	char				field_FD[3];
};

struct VC_RunningScript
{
	VC_RunningScript*	next;//0x00
	VC_RunningScript*	prev;//0x04
	char				scriptName[8];//0x08
	char*				ip;//0x10
	char*				gosub_stack[6];//0x14
	short				sp;//0x2C
	short				field_2E;
	ScriptVar			LocalVars[18];//0x30
	bool				isActive;//0x78
	bool				condResult;//0x79
	bool				missionCleanupFlag;//0x7A
	bool				aWake;//0x7B
	int					wakeTime;//0x7C
	short				logicalOp;//0x80
	bool				notFlag;//0x82
	bool				wastedBustedCheckEnabled;//0x83
	bool				wastedOrBusted;//0x84
	bool				missionFlag;//0x85
	char				field_86[2];
};

struct III_RunningScript
{
	III_RunningScript*	next;//0x00
	III_RunningScript*	prev;//0x04
	char				scriptName[8];//0x08
	char*				ip;//0x10
	char*				gosub_stack[6];//0x14
	short				sp;//0x2C
	short				field_2E;
	ScriptVar			LocalVars[18];//0x30
	bool				condResult;//0x78
	bool				missionCleanupFlag;//0x79
	bool				isActive;//0x7A
	bool				aWake;//0x7B
	int					wakeTime;//0x7C
	short				logicalOp;//0x80
	bool				notFlag;//0x82
	bool				wastedBustedCheckEnabled;//0x83
	bool				wastedOrBusted;//0x84
	bool				missionFlag;//0x85
	char				field_86[2];
};

struct LCS_RunningScript
{
	LCS_RunningScript*	next;//0x00
	LCS_RunningScript*	prev;//0x04
	int					id[2];//0x08
	char				scriptName[8];//0x10
	char*				ip;//0x18
	char*				gosub_stack[16];//0x1C
	short				sp;//0x5C
	short				field_5E;
	ScriptVar			LocalVars[96+8+2];//0x60 //0@-95@ time: -1@ -2@
	int					localsPointer;//0x208
	bool				isActive;//0x20C
	bool				condResult;//0x20D
	bool				missionCleanupFlag;//0x20E
	bool				aWake;//0x20F
	int					wakeTime;//0x210
	short				logicalOp;//0x214
	bool				notFlag;//0x216
	bool				wastedBustedCheckEnabled;//0x217
	bool				wastedOrBusted;//0x218
	bool				missionFlag;//0x219
	char				field_21A[2];
};