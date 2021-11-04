#include "MemoryCall.h"
#include "GameFunc.h"

//SA(CLEO4) VC/III(CLEO 2.0) -> Android SA/VC/III/LCS:
//0A8E=3,%3d% = %1d% + %2d%; int -> 0B8E=3,%3d% = %1d% + %2d%; int
void NewCommands::INT_ADD(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n + Params[1].n;
}

//SA(CLEO4) VC/III(CLEO 2.0) -> Android SA/VC/III/LCS:
//0A8F=3,%3d% = %1d% - %2d%; int -> 0B8F=3,%3d% = %1d% - %2d%; int
void NewCommands::INT_SUB(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n - Params[1].n;
}

//SA(CLEO4) VC/III(CLEO 2.0) -> Android SA/VC/III/LCS:
//0A90=3,%3d% = %1d% * %2d%; int -> 0B90=3,%3d% = %1d% * %2d%; int
void NewCommands::INT_MUL(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n * Params[1].n;
}

//SA(CLEO4) VC/III(CLEO 2.0) -> Android SA/VC/III/LCS:
//0A91=3,%3d% = %1d% / %2d%; int -> 0B91=3,%3d% = %1d% / %2d%; int
void NewCommands::INT_DIV(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n / Params[1].n;
}

/*
0A92=-1,stream_custom_script %1s%
0A93=0,terminate_this_custom_script
0A94=-1,load_and_launch_custom_mission %1s%
0A95=0,save_this_custom_script
*/

//0A96=2,%2d% = ped %1d% struct
void NewCommands::GET_PED_POINTER(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[1].p = reinterpret_cast<int>(CPools::GetPed(Params[0].n));
}

//0A97=2,%2d% = vehicle %1d% struct
void NewCommands::GET_VEHICLE_POINTER(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[1].p = reinterpret_cast<int>(CPools::GetVehicle(Params[0].n));
}

//0A98=2,%2d% = object %1d% struct
void NewCommands::GET_OBJECT_POINTER(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[1].p = reinterpret_cast<int>(CPools::GetObject(Params[0].n));
}

/*
void NewCommands::testa(__handler_params)
{

	//int* c = &cleo->GetPointerToScriptVar(handle)->i;
	//*c = reinterpret_cast<int>(handle);
	SA_Script* script = reinterpret_cast<SA_Script*>(handle);
	fatal("sctr: %04X ", script);
	fatal("sctr: %s ", script->scriptName);
	fatal("sctr: %04X ", reinterpret_cast<int>(handle));
}

void NewCommands::testb(__handler_params)
{
	int* x = &cleo->GetPointerToScriptVar(handle)->i;
	*x = 2021;
	//int* y = reinterpret_cast<int*>()cleo->GetMainLibrarySymbol("_ZN6CPools6GetPedEi");
}
*/

NewCommands::NewCommands()
{
	cleo->PrintToCleoLog("NewCommands plugin init, by: XMDS");
	int ifv = cleo->GetInterfaceVersion();
	if (ifv != 1)
		fatal("Current CLEO library Interface version(v%d), Only support v1 version.", ifv);

	int GV = cleo->GetGameVersionInternal();
	if (GV > VER_GTALCS_2_4_OR_HIGHER)
		fatal("Unknown game version: %d-%s", GV, cleo->GetPackageName());
	/*
	cleo->PrintToCleoLog(cleo->GetCleoPluginLoadDir());
	cleo->PrintToCleoLog(cleo->GetCleoStorageDir());
	cleo->PrintToCleoLog(cleo->GetMainLibraryFileName());
	*/
	cleo->RegisterOpcode(0x0B8E, INT_ADD);
	cleo->RegisterOpcode(0x0B8F, INT_SUB);
	cleo->RegisterOpcode(0x0B90, INT_MUL);
	cleo->RegisterOpcode(0x0B91, INT_DIV);
	cleo->RegisterOpcode(0x0A96, GET_PED_POINTER);
	cleo->RegisterOpcode(0x0A97, GET_VEHICLE_POINTER);
	cleo->RegisterOpcode(0x0A98, GET_OBJECT_POINTER);
	//cleo->RegisterOpcode(0x0C04, testa);
	//cleo->RegisterOpcode(0x0C05, testb);
	cleo->PrintToCleoLog("NewCommands plugin init succeed");
}

extern "C" __attribute__((visibility("default"))) void plugin_init(cleo_ifs_t * ifs)
{
	cleo = ifs;
	NewCommands();
}