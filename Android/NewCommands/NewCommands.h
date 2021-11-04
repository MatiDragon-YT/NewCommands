#pragma once
#include "cleo.h"

ScriptVar Params[255];
cleo_ifs_t* cleo;

#define __handler_params void *handle, uint32_t *ip, uint16_t opcode, const char *name

void fatal(const char* fmt, ...)
{
	char str[256];
	va_list lst;
	va_start(lst, fmt);
	vsprintf(str, fmt, lst);
	va_end(lst);
	cleo->PrintToCleoLog(str);
	exit(1);
}

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
	//static void testa(__handler_params);
	//static void testb(__handler_params);
	NewCommands();
};

