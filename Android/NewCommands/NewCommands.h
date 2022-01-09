#pragma once
#include "pch.h"
#include "CLEO_SDK/cleo.h"
#include "ARMHook/ARMCall.h"

#define __handler_params void *handle, uint32_t *ip, uint16_t opcode, const char *name

extern cleo_ifs_t* cleo;
extern int GameId;
extern int GameVersion;

void FatalFormatInfo(const char* fmt, ...);
void LogFormatInfo(const char* fmt, ...);
void Wchar4ByteTo2Byte(char* dest, char* src, int size);

class NewCommands
{
public:
	static void INT_ADD(__handler_params);
	static void INT_SUB(__handler_params);
	static void INT_MUL(__handler_params);
	static void INT_DIV(__handler_params);
	static void GET_PED_POINTER(__handler_params);
	static void GET_VEHICLE_POINTER(__handler_params);
	static void GET_OBJECT_POINTER(__handler_params);
	static void CHDIR(__handler_params);

	static void GET_THIS_SCRIPT_STRUCT(__handler_params);
	static void GET_VAR_POINTER(__handler_params);
	static void PRINT_HELP_STRING(__handler_params);
	static void PRINT_BIG_STRING(__handler_params);
	static void PRINT_STRING(__handler_params);
	static void PRINT_STRING_NOW(__handler_params);
	static void PRINT_HELP_FORMATTED(__handler_params);
	static void PRINT_BIG_FORMATTED(__handler_params);
	static void PRINT_FORMATTED(__handler_params);
	static void PRINT_FORMATTED_NOW(__handler_params);

	static void BIT_AND(__handler_params);
	static void BIT_OR(__handler_params);
	static void BIT_XOR(__handler_params);
	static void BIT_NOT(__handler_params);
	static void BIT_MOD(__handler_params);
	static void BIT_SHR(__handler_params);
	static void BIT_SHL(__handler_params);
	static void BITWISE_AND_THING_WITH_THING(__handler_params);
	static void BITWISE_OR_THING_WITH_THING(__handler_params);
	static void BITWISE_XOR_THING_WITH_THING(__handler_params);
	static void BITWISE_NOT(__handler_params);
	static void BITWISE_MOD_THING_BY_THING(__handler_params);
	static void BITWISE_SHR_THING_BY_THING(__handler_params);
	static void BITWISE_SHL_THING_BY_THING(__handler_params);

	static void SET_CAR_LIGHT_DAMAGE_STATUS(__handler_params);
	//static void testa(__handler_params);
	//static void testb(__handler_params);
	NewCommands();
	static int GET_GAME_VER();
};

