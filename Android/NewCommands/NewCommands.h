#pragma once
#include "cleo.h"

#define __handler_params void *handle, uint32_t *ip, uint16_t opcode, const char *name

cleo_ifs_t* cleo;

class NewCommands
{
public:
	static void INT_ADD(__handler_params);
	static void INT_SUB(__handler_params);
	static void INT_MUL(__handler_params);
	static void INT_DIV(__handler_params);
	NewCommands();
};

