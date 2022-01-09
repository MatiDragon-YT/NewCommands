#pragma once
#pragma pack(push, 1)

enum eScriptParameterType
{
	SCRIPTPARAM_END_OF_ARGUMENTS,
	SCRIPTPARAM_STATIC_INT_32BITS,
	SCRIPTPARAM_GLOBAL_NUMBER_VARIABLE,
	SCRIPTPARAM_LOCAL_NUMBER_VARIABLE,
	SCRIPTPARAM_STATIC_INT_8BITS,
	SCRIPTPARAM_STATIC_INT_16BITS,
	SCRIPTPARAM_STATIC_FLOAT,

	// Types below are only available in GTA SA

	// Number arrays
	SCRIPTPARAM_GLOBAL_NUMBER_ARRAY,
	SCRIPTPARAM_LOCAL_NUMBER_ARRAY,
	SCRIPTPARAM_STATIC_SHORT_STRING,
	SCRIPTPARAM_GLOBAL_SHORT_STRING_VARIABLE,
	SCRIPTPARAM_LOCAL_SHORT_STRING_VARIABLE,
	SCRIPTPARAM_GLOBAL_SHORT_STRING_ARRAY,
	SCRIPTPARAM_LOCAL_SHORT_STRING_ARRAY,
	SCRIPTPARAM_STATIC_PASCAL_STRING, 
	SCRIPTPARAM_STATIC_LONG_STRING,
	SCRIPTPARAM_GLOBAL_LONG_STRING_VARIABLE,
	SCRIPTPARAM_LOCAL_LONG_STRING_VARIABLE,
	SCRIPTPARAM_GLOBAL_LONG_STRING_ARRAY,
	SCRIPTPARAM_LOCAL_LONG_STRING_ARRAY,

	//only VC/III
	VC_III_SCRIPTPARAM_STATIC_PASCAL_STRING = 14,
};

enum eLCSScriptParameterType
{
	ARGUMENT_END = 0,
	ARGUMENT_INT_ZERO,
	ARGUMENT_FLOAT_ZERO,
	ARGUMENT_FLOAT_1BYTE,
	ARGUMENT_FLOAT_2BYTES,
	ARGUMENT_FLOAT_3BYTES,
	ARGUMENT_INT32,
	ARGUMENT_INT8,
	ARGUMENT_INT16,
	ARGUMENT_FLOAT,
	ARGUMENT_TIMERA,
	ARGUMENT_TIMERB,
	ARGUMENT_LOCAL = 12,//12 = 0@ --- 107 = 95@
	ARGUMENT_LOCAL_ARRAY = 108, //108 = 0@[0] --- 203 = 0@[95]
	ARGUMENT_GLOBAL = 204, //26 NUM_GLOBAL_SLOTS
	ARGUMENT_GLOBAL_ARRAY = 230, //26 NUM_GLOBAL_SLOTS_ARRAY
	MAX_ARGUMENT = 256,
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
	SA_RunningScript* next;//0x00
	SA_RunningScript* prev;//0x04
	char  scriptName[8];//0x08
	char* baseIp;//0x10
	char* ip;//0x14
	char* gosub_stack[8];//0x18
	short sp;//0x38
	short field_3A;
	ScriptVar LocalVars[42];//0x3C //0@-39@ time: 40@ 41@
	bool isActive;//0xE4
	bool condResult;//0xE5
	bool missionCleanupFlag;//0xE6
	bool isExternal;//0xE7
	bool textBlockOverride;//0xE8
	char field_E9;
	char field_EA;
	char field_EB;
	int wakeTime;//0xEC
	short logicalOp;//0xF0
	bool notFlag;//0xF2
	bool wastedBustedCheckEnabled;//0xF3
	bool wastedOrBusted;//0xF4
	char field_F5;
	short field_F6;
	char* sceneSkip;//0xF8
	bool isMission;//0xFC
	char field_FD[3];
};

struct VC_RunningScript
{
	VC_RunningScript* next;//0x00
	VC_RunningScript* prev;//0x04
	char scriptName[8];//0x08
	int ip;//0x10
	char* gosub_stack[6];//0x14
	short sp;//0x2C
	short field_2E;
	ScriptVar LocalVars[18];//0x30
	bool isActive;//0x78
	bool condResult;//0x79
	bool missionCleanupFlag;//0x7A
	bool aWake;//0x7B
	int	wakeTime;//0x7C
	short logicalOp;//0x80
	bool notFlag;//0x82
	bool wastedBustedCheckEnabled;//0x83
	bool wastedOrBusted;//0x84
	bool isMission;//0x85
	char field_86[2];
};

struct III_RunningScript
{
	III_RunningScript* next;//0x00
	III_RunningScript* prev;//0x04
	char scriptName[8];//0x08
	int ip;//0x10
	char* gosub_stack[6];//0x14
	short sp;//0x2C
	short field_2E;
	ScriptVar LocalVars[18];//0x30
	bool condResult;//0x78
	bool missionCleanupFlag;//0x79
	bool isActive;//0x7A
	bool aWake;//0x7B
	int wakeTime;//0x7C
	short logicalOp;//0x80
	bool notFlag;//0x82
	bool wastedBustedCheckEnabled;//0x83
	bool wastedOrBusted;//0x84
	bool isMission;//0x85
	char field_86[2];
};

struct LCS_RunningScript
{
	LCS_RunningScript* next;//0x00
	LCS_RunningScript* prev;//0x04
	int processId;//0x08
	char field_0C;
	char scriptName[8];//0x10
	int ip;//0x18
	char* gosub_stack[16];//0x1C
	short sp;//0x5C
	short field_5E;
	ScriptVar LocalVars[96 + 8 + 2];//0x60 //0@-95@ time: -1@ -2@
	int	baseLocalVarsIndex;//0x208
	bool isActive;//0x20C
	bool condResult;//0x20D
	bool missionCleanupFlag;//0x20E
	bool aWake;//0x20F
	int wakeTime;//0x210
	short logicalOp;//0x214
	bool notFlag;//0x216
	bool wastedBustedCheckEnabled;//0x217
	bool wastedOrBusted;//0x218
	bool isMission;//0x219
	char field_21A[2];
};

class CRunningScript
{
public:
	void UpdateCompareFlag(unsigned char flag);
	void ReadTextLabelFromScript(char* pBuffer, unsigned char nBufferLength);
};
#pragma pack(pop)