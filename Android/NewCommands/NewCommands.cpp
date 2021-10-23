#include "NewCommands.h"


void NewCommands::INT_ADD(__handler_params)
{
	int p = cleo->ReadParam(handle)->i;
	int a = cleo->ReadParam(handle)->i;
	int* c = &cleo->GetPointerToScriptVar(handle)->i;
	*c = p + a;
}

void NewCommands::INT_SUB(__handler_params)
{
	int p = cleo->ReadParam(handle)->i;
	int a = cleo->ReadParam(handle)->i;
	int* c = &cleo->GetPointerToScriptVar(handle)->i;
	*c = p - a;
}

void NewCommands::INT_MUL(__handler_params)
{
	int p = cleo->ReadParam(handle)->i;
	int a = cleo->ReadParam(handle)->i;
	int* c = &cleo->GetPointerToScriptVar(handle)->i;
	*c = p * a;
}

void NewCommands::INT_DIV(__handler_params)
{
	int p = cleo->ReadParam(handle)->i;
	int a = cleo->ReadParam(handle)->i;
	int* c = &cleo->GetPointerToScriptVar(handle)->i;
	*c = p / a;
}

NewCommands::NewCommands()
{
	cleo->PrintToCleoLog("NewCommands plugin init");
	cleo->RegisterOpcode(0x0C00, INT_ADD);
	cleo->RegisterOpcode(0x0C01, INT_SUB);
	cleo->RegisterOpcode(0x0C02, INT_MUL);
	cleo->RegisterOpcode(0x0C03, INT_DIV);
	cleo->PrintToCleoLog("NewCommands plugin init succeed");
}

extern "C" __attribute__((visibility("default"))) void plugin_init(cleo_ifs_t * ifs)
{
	cleo = ifs;
	NewCommands();
}