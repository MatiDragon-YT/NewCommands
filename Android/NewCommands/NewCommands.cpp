#include "NewCommands.h"
#include "CRunningScript.h"
#include "GameFunc.h"
#include <algorithm>
#include <iostream>

using namespace std;

GameFunc Game;
cleo_ifs_t* cleo = nullptr;
int GameVersion = -1;
int GameId = -1;
uintptr_t LibAddr;
ScriptVar Params[50];
std::string CleoFileDir1;
std::string CleoFileDir2 = "/storage/emulated/0/Android/data/com.rockstargames.gtasa/files/";
//std::string s2 = cleo->GetCleoStorageDir();

char* ReadString(void* handle, uint32_t* ip, char* buf, unsigned char size);
int format(void* handle, uint32_t* ip, char* str, size_t len, const char* format);
void SkipUnusedParam(void* handle, uint32_t* ip);

//SA(CLEO4) VC/III(CLEO 2.0) -> Android SA/VC/III/LCS:
/*
0A8C=4,write_memory %1d% size %2d% value %3d% virtual_protect %4d% //
0A8D=4,%4d% = read_memory %1d% size %2d% virtual_protect %3d% // 
*/

//0A8E=3,%3d% = %1d% + %2d%; int -> 0B8E=3,%3d% = %1d% + %2d%; int
void NewCommands::INT_ADD(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n + Params[1].n;
}

//0A8F=3,%3d% = %1d% - %2d%; int -> 0B8F=3,%3d% = %1d% - %2d%; int
void NewCommands::INT_SUB(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n - Params[1].n;
}

//0A90=3,%3d% = %1d% * %2d%; int -> 0B90=3,%3d% = %1d% * %2d%; int
void NewCommands::INT_MUL(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[2].p = Params[0].n * Params[1].n;
}

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

//0A99=1,set_current_directory %1buserdir/rootdir%
void NewCommands::CHDIR(__handler_params)
{
	int* ipptr = reinterpret_cast<int*>(cleo->GetRealCodePointer(*ip));
	unsigned char type = *(unsigned char*)ipptr;
	if (type >= SCRIPTPARAM_STATIC_INT_32BITS && type <= SCRIPTPARAM_LOCAL_NUMBER_ARRAY)
	{
		Params[0].n = cleo->ReadParam(handle)->i;
		if (Params[0].n == -1)
			chdir("/");
		else if (Params[0].n == 0)
			reinterpret_cast<SA_Script*>(handle)->CleoFileDir = Params[0].n;
		else
			reinterpret_cast<SA_Script*>(handle)->CleoFileDir = Params[0].n;
	}
	else
	{
		char buf[0xFF];
		Params[0].c = ReadString(handle, ip, buf, sizeof(buf));
		chdir(Params[0].c);
	}
}

//0A9A=3,%3d% = open_file %1s% mode %2d%; IF and SET
void NewCommands::OPEN_FILE(__handler_params)
{
	char buf[0xFF], mode[0x10];
	Params[0].c = ReadString(handle, ip, buf, sizeof(buf));
	int* ipptr = reinterpret_cast<int*>(cleo->GetRealCodePointer(*ip));
	unsigned char type = *(unsigned char*)ipptr;
	if (type >= SCRIPTPARAM_STATIC_INT_32BITS && type <= SCRIPTPARAM_LOCAL_NUMBER_ARRAY)
	{
		union
		{
			unsigned uParam;
			char strParam[4];
		}param;
		param.uParam = cleo->ReadParam(handle)->i;
		strcpy(mode, param.strParam);
	}
	else
		ReadString(handle, ip, mode, sizeof(mode));

	auto file = fopen(Params[0].c, mode);
	cleo->GetPointerToScriptVar(handle)->i = reinterpret_cast<int>(file);
	if (file)
		Game.openedFiles->insert(file);
	reinterpret_cast<CRunningScript*>(handle)->UpdateCompareFlag(file != nullptr);
}

//0A9B=1,close_file %1d%
void NewCommands::CLOSE_FILE(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	fclose((FILE *)Params[0].n);
	Game.openedFiles->erase((FILE*)Params[0].n);
}

//0A9C=2,%2d% = file %1d% size
void NewCommands::GET_FILE_SIZE(__handler_params)
{
	FILE* file = reinterpret_cast<FILE*>(cleo->ReadParam(handle)->i);
	auto savedPos = ftell(file);
	fseek(file, 0, SEEK_END);
	cleo->GetPointerToScriptVar(handle)->u = static_cast<unsigned>(ftell(file));
	fseek(file, savedPos, SEEK_SET);
}

//0A9D=3,read_file %1d% size %2d% to %3d%
void NewCommands::READ_FROM_FILE(__handler_params)
{
	FILE* file = reinterpret_cast<FILE*>(cleo->ReadParam(handle)->i);
	unsigned int size = cleo->ReadParam(handle)->u;
	void* buf = reinterpret_cast<void*>(cleo->ReadParam(handle)->i);
	fread(buf, size, 1, file);
}

//0A9E=3,write_file %1d% size %2d% from %3d%
void NewCommands::WRITE_TO_FILE(__handler_params)
{
	FILE* file = reinterpret_cast<FILE*>(cleo->ReadParam(handle)->i);
	unsigned int size = cleo->ReadParam(handle)->u;
	void* buf = reinterpret_cast<void*>(cleo->ReadParam(handle)->i);
	fwrite(buf, size, 1, file);
	fflush(file);
}

//0A9F=1,%1d% = get_this_script_struct
void NewCommands::GET_THIS_SCRIPT_STRUCT(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[0].p = reinterpret_cast<int>(handle);
}

/*
0AA0 = 1, gosub_if_false % 1p %
0AA1 = 0, return_if_false
0AA2 = 2, % 2h % = load_dynamic_library % 1s%; IFand SET
0AA3 = 1, free_dynamic_library % 1h %
0AA4 = 3, % 3d % = get_dynamic_library_procedure % 1s % library % 2d%; IFand SET
0AA5 = -1, call_function % 1d % num_params % 2h % pop % 3h %
0AA6 = -1, call_method % 1d % struct % 2d % num_params % 3h % pop % 4h %
0AA7 = -1, call_function_return % 1d % num_params % 2h % pop % 3h %
0AA8 = -1, call_method_return % 1d % struct % 2d % num_params % 3h % pop % 4h %
0AA9 = 0, is_game_version_original
0AAA = 2, % 2d % = get_script_struct_named % 1s %
0AAB = 1, does_file_exist % 1s %
0AAC = 2, % 2d % = load_audio_stream % 1d %
0AAD = 2, set_audio_stream % 1d % state % 2d %
0AAE = 1, remove_audio_stream % 1d %
0AAF = 2, % 2d % = get_audio_stream_length % 1d %
0AB0 = 1, is_key_pressed % 1d %
0AB1 = -1, cleo_call % 1p %
0AB2 = -1, cleo_return % 1d %
0AB3 = 2, cleo_shared_var % 1d % = % 2d %
0AB4 = 2, % 2d % = cleo_shared_var % 1d %
0AB5 = 3, store_char % 1d % closest_vehicle_to % 2d % closest_ped_to % 3d %
0AB6 = 3, get_target_blip_coords_to % 1d % %2d % %3d % // IF and SET
0AB7 = 2, get_car % 1d % number_of_gears_to % 2d %
0AB8 = 2, get_car % 1d % current_gear_to % 2d %
0AB9 = 2, get_audio_stream % 1d % state_to % 2d %
0ABA = 1, terminate_all_custom_scripts_with_this_name % 1s %
0ABB = 2, % 2d % = audio_stream % 1d % volume
0ABC = 2, set_audio_stream % 1d % volume % 2d %
0ABD = 1, car % 1d % siren_on
0ABE = 1, car % 1d % engine_on
0ABF = 2, set_car % 1d % engine_state_to % 2d %
0AC0 = 2, set_audio_stream % 1d % looped % 2d %
0AC1 = 2, % 2d % = load_audio_stream_with_3d_support % 1d%; IFand SET
0AC2 = 4, link_3d_audio_stream % 1d % at_coords % 2d % %3d % %4d %
0AC3 = 2, link_3d_audio_stream % 1d % to_object % 2d %
0AC4 = 2, link_3d_audio_stream % 1d % to_actor % 2d %
0AC5 = 2, link_3d_audio_stream % 1d % to_car % 2d %
0AC6 = 2, % 2d % = label % 1p % pointer
*/

//0AC7=2,%2d% = var %1d% pointer
void NewCommands::GET_VAR_POINTER(__handler_params)
{
	Params[0].n = reinterpret_cast<int>(cleo->GetPointerToScriptVar(handle));
	Params[1].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[1].p = Params[0].n;	
}
/*
0AC8 = 2, % 2d % = allocate_memory_size % 1d %
0AC9 = 1, free_allocated_memory % 1d %
*/

//0ACA=1,show_text_box %1s%
void NewCommands::PRINT_HELP_STRING(__handler_params)
{
	/*
	*ip += 1;
	unsigned char length = *(unsigned char*)*ip;
	strncpy((char*)(*ip), (char*)(*ip + 1), length);
	*(unsigned char*)(*ip += length) = 00;
	*ip -= length;
	char buf[0x100];
	strcpy(buf, (char*)*ip);
	*ip += (strlen((char*)*ip) + 1);*/

	static char message_buf[0x1FE];
	char buf[0xFF];
	ReadString(handle, ip, buf, sizeof(buf));
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	switch (GameId)
	{
	case GTASA:
		CHud::SetHelpMessage(0, reinterpret_cast<wchar_t*>(message_buf), 0, 0, 0, 0);
		break;
	case GTAVC:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0, 0, 0);
		break;
	case GTA3:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0);
		break;
	case GTALCS:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0, 0);
		break;
	default:
		break;
	}
}

//0ACB=3,show_styled_text %1s% time %2d% style %3d%
void NewCommands::PRINT_BIG_STRING(__handler_params)
{
	static char message_buf[0x1FE];
	char buf[0xFF];
	ReadString(handle, ip, buf, sizeof(buf));
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	CMessages::AddBigMessageQ(reinterpret_cast<wchar_t*>(message_buf), Params[0].n, Params[1].n - 1);
}

//0ACC=2,show_text_lowpriority %1s% time %2d%
void NewCommands::PRINT_STRING(__handler_params)
{
	static char message_buf[0x1FE];
	char buf[0xFF];
	ReadString(handle, ip, buf, sizeof(buf));
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	Params[0].n = cleo->ReadParam(handle)->i;
	switch (GameId)
	{
	case GTASA:
		CMessages::AddMessage(0, reinterpret_cast<wchar_t*>(message_buf), Params[0].n, 0, 0);
		break;
	case GTAVC:
	case GTA3:
	case GTALCS:
		CMessages::AddMessage(reinterpret_cast<wchar_t*>(message_buf), Params[0].n, 0);
		break;
	default:
		break;
	}
}

//0ACD=2,show_text_highpriority %1s% time %2d%
void NewCommands::PRINT_STRING_NOW(__handler_params)
{
	static char message_buf[0x1FE];
	char buf[0xFF];
	ReadString(handle, ip, buf, sizeof(buf));
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	Params[0].n = cleo->ReadParam(handle)->i;
	switch (GameId)
	{
	case GTASA:
		CMessages::AddMessageJumpQ(0, reinterpret_cast<wchar_t*>(message_buf), Params[0].n, 0, 0);
		break;
	case GTAVC:
	case GTA3:
		CMessages::AddMessageJumpQ(reinterpret_cast<wchar_t*>(message_buf), Params[0].n, 0);
		break;
	case GTALCS:
		CMessages::AddMessageJumpQ(reinterpret_cast<wchar_t*>(message_buf), Params[0].n, 0, 0, 0);
		break;
	default:
		break;
	}
}

//0ACE=-1,show_formatted_text_box %1s%
void NewCommands::PRINT_HELP_FORMATTED(__handler_params)
{
	static char message_buf[0x1FE], buf[0xFF];
	char fmt[0xFF];
	ReadString(handle, ip, fmt, sizeof(fmt));
	format(handle, ip, buf, sizeof(buf), fmt);
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	switch (GameId)
	{
	case GTASA:
		CHud::SetHelpMessage(0, reinterpret_cast<wchar_t*>(message_buf), 0, 0, 0, 0);
		break;
	case GTAVC:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0, 0, 0);
		break;
	case GTA3:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0);
		break;
	case GTALCS:
		CHud::SetHelpMessage(reinterpret_cast<wchar_t*>(message_buf), 0, 0);
		break;
	default:
		break;
	}
	SkipUnusedParam(handle, ip);
}

//0ACF=-1,show_formatted_styled_text %1s% time %2d% style %3d%
void NewCommands::PRINT_BIG_FORMATTED(__handler_params)
{
	static char message_buf[0x1FE], buf[0xFF];
	char fmt[0xFF];
	ReadString(handle, ip, fmt, sizeof(fmt));
	Params[1].n = cleo->ReadParam(handle)->i;
	Params[2].n = cleo->ReadParam(handle)->i;
	format(handle, ip, buf, sizeof(buf), fmt);
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	CMessages::AddBigMessageQ(reinterpret_cast<wchar_t*>(message_buf), Params[1].n, Params[2].n - 1);
	SkipUnusedParam(handle, ip);
}

//0AD0=-1,show_formatted_text_lowpriority %1s% time %2d%
void NewCommands::PRINT_FORMATTED(__handler_params)
{
	static char message_buf[0x1FE], buf[0xFF];
	char fmt[0xFF];
	ReadString(handle, ip, fmt, sizeof(fmt));
	Params[1].n = cleo->ReadParam(handle)->i;
	format(handle, ip, buf, sizeof(buf), fmt);
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	switch (GameId)
	{
	case GTASA:
		CMessages::AddMessage(0, reinterpret_cast<wchar_t*>(message_buf), Params[1].n, 0, 0);
		break;
	case GTAVC:
	case GTA3:
	case GTALCS:
		CMessages::AddMessage(reinterpret_cast<wchar_t*>(message_buf), Params[1].n, 0);
		break;
	default:
		break;
	}
	SkipUnusedParam(handle, ip);
}
//0AD1=-1,show_formatted_text_highpriority %1s% time %2d%
void NewCommands::PRINT_FORMATTED_NOW(__handler_params)
{
	static char message_buf[0x1FE], buf[0xFF];
	char fmt[0xFF];
	ReadString(handle, ip, fmt, sizeof(fmt));
	Params[1].n = cleo->ReadParam(handle)->i;
	format(handle, ip, buf, sizeof(buf), fmt);
	Wchar4ByteTo2Byte(message_buf, buf, sizeof(buf));
	switch (GameId)
	{
	case GTASA:
		CMessages::AddMessageJumpQ(0, reinterpret_cast<wchar_t*>(message_buf), Params[1].n, 0, 0);
		break;
	case GTAVC:
	case GTA3:
		CMessages::AddMessageJumpQ(reinterpret_cast<wchar_t*>(message_buf), Params[1].n, 0);
		break;
	case GTALCS:
		CMessages::AddMessageJumpQ(reinterpret_cast<wchar_t*>(message_buf), Params[1].n, 0, 0, 0);
		break;
	default:
		break;
	}
	SkipUnusedParam(handle, ip);
}

/*
0AD2 = 2, % 2d % = player % 1d % targeted_actor // IF and SET
0AD3 = -1, % 1d % = string_format % 2s %
0AD4 = -1, % 3d % = scan_string % 1d % format % 2s %
0AD5 = 3, file % 1d % seek % 2d % from_origin % 3d % // IF and SET
0AD6 = 1, is_end_of_file_reached % 1d %
0AD7 = 3, read_string_from_file % 1d % to % 2d % size % 3d % // IF and SET
0AD8 = 2, write_string_to_file % 1d % from % 2d % // IF and SET
0AD9 = -1, write_formatted_text % 2d % to_file % 1d %
0ADA = -1, % 3d % = scan_file % 1d % format % 2d % // IF and SET
0ADB = 2, % 2d % = vehicle_model % 1o % name
0ADC = 1, test_cheat % 1d %
0ADD = 1, spawn_car_with_model % 1o % like_a_cheat
0ADE = 2, % 2d % = text_label_string % 1d %
0ADF = 2, add_text_label % 1d % text % 2d %
0AE0 = 1, remove_text_label % 1d %
0AE1 = 7, % 7d % = random_char_near_point % 1d % %2d % %3d % in_radius % 4d % find_next % 5h % pass_deads % 6h % // IF and SET
0AE2 = 7, % 7d % = random_vehicle_near_point % 1d % %2d % %3d % in_radius % 4d % find_next % 5h % pass_wrecked % 6h % // IF and SET
0AE3 = 6, % 6d % = random_object_near_point % 1d % %2d % %3d % in_radius % 4d % find_next % 5h % // IF and SET
0AE4 = 1, does_directory_exist % 1d %
0AE5 = 1, create_directory % 1d%; IFand SET
0AE6 = 3, % 2d % = find_first_file % 1d % get_filename_to % 3d%; IFand SET
0AE7 = 2, % 2d % = find_next_file % 1d%; IFand SET
0AE8 = 1, find_close % 1d %
0AE9 = 1, pop_float % 1d %
0AEA = 2, % 2d % = ped_struct % 1d % handle
0AEB = 2, % 2d % = vehicle_struct % 1d % handle
0AEC = 2, % 2d % = object_struct % 1d % handle
0AED = 3, % 3d % = float% 1d % to_string_format % 2d %
0AEE = 3, % 3d % = pow % 1d % base % 2d % // all floats
0AEF = 3, % 3d % = log % 1d % base % 2d % // all floats
*/

//; CLEO 4 IntOperations.cleo

//0B10=3,%3d% = %1d% AND %2d%
void NewCommands::BIT_AND(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n & Params[1].n;
}

//0B11=3,%3d% = %1d% OR %2d%
void NewCommands::BIT_OR(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n | Params[1].n;
}

//0B12=3,%3d% = %1d% XOR %2d%
void NewCommands::BIT_XOR(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n ^ Params[1].n;
}

//0B13=2,%2d% = NOT %1d%
void NewCommands::BIT_NOT(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = ~Params[0].n;
}

//0B14=3,%3d% = %1d% MOD %2d%
void NewCommands::BIT_MOD(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n % Params[1].n;
}

//0B15=3,%3d% = %1d% SHR %2d%
void NewCommands::BIT_SHR(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n >> Params[1].n;
}

//0B16=3,%3d% = %1d% SHL %2d%
void NewCommands::BIT_SHL(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	cleo->GetPointerToScriptVar(handle)->i = Params[0].n << Params[1].n;
}

//0B17=2,%1d% &= %2d%
void NewCommands::BITWISE_AND_THING_WITH_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p &= Params[1].n;
}

//0B18=2,%1d% |= %2d%
void NewCommands::BITWISE_OR_THING_WITH_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p |= Params[1].n;
}

//0B19=2,%1d% ^= %2d%
void NewCommands::BITWISE_XOR_THING_WITH_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p ^= Params[1].n;
}

//0B1A=1,~ %1d%
void NewCommands::BITWISE_NOT(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	*(int*)Params[0].p = ~*(int*)Params[0].p;
}

//0B1B=2,%1d% %= %2d%
void NewCommands::BITWISE_MOD_THING_BY_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p %= Params[1].n;
}

//0B1C=2,%1d% >>= %2d%
void NewCommands::BITWISE_SHR_THING_BY_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p >>= Params[1].n;
}

//0B1D=2,%1d% <<= %2d%
void NewCommands::BITWISE_SHL_THING_BY_THING(__handler_params)
{
	Params[0].p = &cleo->GetPointerToScriptVar(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->i;
	*(int*)Params[0].p <<= Params[1].n;
}

//0D5C=3,%3d% = get_car %1d% light %2d% damage_state
void NewCommands::GET_CAR_LIGHT_DAMAGE_STATUS(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->u;
	Params[2].p = &cleo->GetPointerToScriptVar(handle)->u;
	CVehicle* Vehicle = CPools::GetVehicle(Params[0].n);
	CDamageManager* Damage;
	switch (GameId)
	{
	case GTASA:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + (GET_GAME_VER() ? 0x5B4 : 0x5B0));
		break;
	case GTAVC:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x2A4);
		break;
	case GTA3:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x28C);
		break;
	case GTALCS:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x34C);
		break;
	}
	*(unsigned int*)Params[2].p = Damage->GetLightStatus(static_cast<CDamageManager::eLights>(Params[1].n));
}

//0D5D=3,set_car %1d% light %2d% damage_state %3d%
void NewCommands::SET_CAR_LIGHT_DAMAGE_STATUS(__handler_params)
{
	Params[0].n = cleo->ReadParam(handle)->i;
	Params[1].n = cleo->ReadParam(handle)->u;
	Params[2].n = cleo->ReadParam(handle)->u;
	CVehicle* Vehicle = CPools::GetVehicle(Params[0].n);
	CDamageManager* Damage;
	switch (GameId)
	{
	case GTASA:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + (GET_GAME_VER() ? 0x5B4 : 0x5B0));
		break;
	case GTAVC:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x2A4);
		break;
	case GTA3:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x28C);
		break;
	case GTALCS:
		Damage = reinterpret_cast<CDamageManager*>((int)Vehicle + 0x34C);
		break;
	}
	Damage->SetLightStatus(static_cast<CDamageManager::eLights>(Params[1].n), Params[2].n);
}

void FatalFormatInfo(const char* fmt, ...)
{
	char str[256];
	va_list lst;
	va_start(lst, fmt);
	vsprintf(str, fmt, lst);
	va_end(lst);
	cleo->PrintToCleoLog(str);
	exit(1);
}

void LogFormatInfo(const char* fmt, ...)
{
	char str[256];
	va_list lst;
	va_start(lst, fmt);
	vsprintf(str, fmt, lst);
	va_end(lst);
	cleo->PrintToCleoLog(str);
}

void Wchar4ByteTo2Byte(char* dest, char* src, int size)
{
	int a = 0;
	for (int i = 0; i < size; i++)
	{
		dest[a] = src[i];
		dest[a += 1] = 00;
		a++;
	}
}

char* ReadString(void* handle, uint32_t* ip, char* buf, unsigned char size)
{
	std::fill(buf, buf + size, '\0');
	int* ipptr = reinterpret_cast<int*>(cleo->GetRealCodePointer(*ip));
	if (GameId == GTALCS)
	{
		char* str = reinterpret_cast<char*>(cleo->ReadParam(handle)->i);
		strncpy(buf, str, size - 1);
		return buf;
	}
	unsigned char type = *(unsigned char*)ipptr;
	//LogFormatInfo("type: %d", type);
	switch (type)
	{
	case SCRIPTPARAM_STATIC_INT_32BITS:
	case SCRIPTPARAM_GLOBAL_NUMBER_VARIABLE:
	case SCRIPTPARAM_LOCAL_NUMBER_VARIABLE:
	case SCRIPTPARAM_STATIC_INT_8BITS:
	case SCRIPTPARAM_STATIC_INT_16BITS:
	case SCRIPTPARAM_STATIC_FLOAT:
	case SCRIPTPARAM_GLOBAL_NUMBER_ARRAY:
	case SCRIPTPARAM_LOCAL_NUMBER_ARRAY:
	{
		char* str = reinterpret_cast<char*>(cleo->ReadParam(handle)->i);
		strncpy(buf, str, size - 1);
		break;
	}
	case SCRIPTPARAM_STATIC_PASCAL_STRING:
		cleo->ReadStringLong(handle, buf, size);
		break;
	case SCRIPTPARAM_STATIC_SHORT_STRING:
		//cleo->ReadString8byte(handle, buf, size);
		//break;
	case SCRIPTPARAM_GLOBAL_SHORT_STRING_VARIABLE:
	case SCRIPTPARAM_LOCAL_SHORT_STRING_VARIABLE:
	case SCRIPTPARAM_GLOBAL_SHORT_STRING_ARRAY:
	case SCRIPTPARAM_LOCAL_SHORT_STRING_ARRAY:
	case SCRIPTPARAM_STATIC_LONG_STRING:
	case SCRIPTPARAM_GLOBAL_LONG_STRING_VARIABLE:
	case SCRIPTPARAM_LOCAL_LONG_STRING_VARIABLE:
	case SCRIPTPARAM_GLOBAL_LONG_STRING_ARRAY:
	case SCRIPTPARAM_LOCAL_LONG_STRING_ARRAY:
	{
		CRunningScript* script = reinterpret_cast<CRunningScript*>(handle);
		script->ReadTextLabelFromScript(buf, size);
		break;
	}
	default:
		if (GameId == GTA3 || GameId == GTAVC)
			cleo->ReadString8byte(handle, buf, size);
		else
			cleo->PrintToCleoLog("XMDS!!!!");
		break;
	}
	buf[size - 1] = 00;
	return buf;
}

// perform 'sprintf'-operation for parameters, passed through SCM
int format(void* handle, uint32_t* ip, char* str, size_t len, const char* format)
{
	unsigned int written = 0;
	const char* iter = format;
	char bufa[256], fmtbufa[64], * fmta;

	while (*iter)
	{
		while (*iter && *iter != '%')
		{
			if (written++ >= len)
				return -1;
			*str++ = *iter++;
		}
		if (*iter == '%')
		{
			if (iter[1] == '%')
			{
				if (written++ >= len)
					return -1;
				*str++ = '%'; /* "%%"->'%' */
				iter += 2;
				continue;
			}

			//get flags and width specifier
			fmta = fmtbufa;
			*fmta++ = *iter++;
			while (*iter == '0' ||
				*iter == '+' ||
				*iter == '-' ||
				*iter == ' ' ||
				*iter == '*' ||
				*iter == '#')
			{
				if (*iter == '*')
				{
					char* buffiter = bufa;
					//get width
					Params[0].d = cleo->ReadParam(handle)->i;
					sprintf(buffiter, "%d", Params[0].d);
					while (*buffiter)
						*fmta++ = *buffiter++;
				}
				else
					*fmta++ = *iter;
				iter++;
			}

			//get immidiate width value
			while (isdigit(*iter))
				*fmta++ = *iter++;

			//get precision
			if (*iter == '.')
			{
				*fmta++ = *iter++;
				if (*iter == '*')
				{
					char* buffiter = bufa;
					Params[0].d = cleo->ReadParam(handle)->i;
					sprintf(buffiter, "%d", Params[0].d);
					while (*buffiter)
						*fmta++ = *buffiter++;
				}
				else
					while (isdigit(*iter))
						*fmta++ = *iter++;
			}
			//get size
			if (*iter == 'h' || *iter == 'l')
				*fmta++ = *iter++;

			switch (*iter)
			{
			case 's':
			{
				static const char none[] = "(null)";
				char buf[0xFF];
				const char* astr = ReadString(handle, ip, buf, sizeof(buf));
				const char* striter = astr ? astr : none;
				while (*striter)
				{
					if (written++ >= len)
						return -1;
					*str++ = *striter++;
				}
				iter++;
				break;
			}

			case 'c':
				if (written++ >= len)
					return -1;
				Params[0].n = cleo->ReadParam(handle)->i;
				*str++ = (char)Params[0].n;
				iter++;
				break;

			default:
			{
				/* For non wc types, use system sprintf and append to wide char output */
				/* FIXME: for unrecognised types, should ignore % when printing */
				char* bufaiter = bufa;
				if (*iter == 'p' || *iter == 'P')
				{
					Params[0].d = cleo->ReadParam(handle)->i;
					sprintf(bufaiter, "%08X", Params[0].d);
				}
				else
				{
					*fmta++ = *iter;
					*fmta = '\0';
					if (*iter == 'a' || *iter == 'A' ||
						*iter == 'e' || *iter == 'E' ||
						*iter == 'f' || *iter == 'F' ||
						*iter == 'g' || *iter == 'G')
					{
						Params[0].f = cleo->ReadParam(handle)->f;
						sprintf(bufaiter, fmtbufa, Params[0].f);
					}
					else
					{
						Params[0].p = reinterpret_cast<void*>(cleo->ReadParam(handle)->i);
						sprintf(bufaiter, fmtbufa, Params[0].p);
					}
				}
				while (*bufaiter)
				{
					if (written++ >= len)
						return -1;
					*str++ = *bufaiter++;
				}
				iter++;
				break;
			}
			}
		}
	}
	if (written >= len)
		return -1;
	*str++ = 0;
	return (int)written;
}

void SkipUnusedParam(void* handle, uint32_t* ip)
{
	char buf[0xFF];
	while (!cleo->IsParamListEnd(*ip))
	{
		if (GameId != GTALCS)
		{
			int* ipptr = reinterpret_cast<int*>(cleo->GetRealCodePointer(*ip));
			unsigned char type = *(unsigned char*)ipptr;
			if (type >= SCRIPTPARAM_STATIC_INT_32BITS && type <= SCRIPTPARAM_LOCAL_NUMBER_ARRAY)
				cleo->ReadParam(handle);
			else
				ReadString(handle, ip, buf, sizeof(buf));
		}
		else
			cleo->ReadParam(handle);
	}
	cleo->SkipParamListEnd(ip);
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
	//fatal("ip: %d", ip);= fatal("ip: %d", cleo->GetIpUsingRealCodePointer(ip)); //脚本结构IP的地址（handle + 0x14）

	/*
	fatal("ip: %d", cleo->GetRealCodePointer(*ip));//get script->ip
	fatal("ip: %d", *ip);
	SA_Script* script = reinterpret_cast<SA_Script*>(handle);
	fatal("ip: %d", script->ip);
	   //IP地址指向的指针
}*/

void ScriptsEvent() //0x003F3AE8
{
	cleo->PrintToCleoLog("XMDS23333333333");
	CTheScripts::Init();
	std::for_each(Game.openedFiles->begin(), Game.openedFiles->end(), fclose);
	Game.openedFiles->clear();
	cleo->PrintToCleoLog("ppppppppppppppppp");
	return;
}

NewCommands::NewCommands()
{
	cleo->PrintToCleoLog("'libNewCommands.so' plugin init, by: XMDS");
	int ifv = cleo->GetInterfaceVersion();
	if (ifv != 1)
		FatalFormatInfo("Current CLEO library Interface version(v%d), NewCommands Only support v1 version.", ifv);
	GameId = cleo->GetGameIdentifier();
	int GameVer = GET_GAME_VER();
	if (GameVer == -1)
		FatalFormatInfo("'libNewCommands.so' plugin does not support your game version: %d-%d-%s", GameId, GameVersion, cleo->GetPackageName());
	/*
	cleo->PrintToCleoLog(cleo->GetCleoPluginLoadDir());
	cleo->PrintToCleoLog(cleo->GetCleoStorageDir());
	cleo->PrintToCleoLog(cleo->GetMainLibraryFileName());
	*/
	//uintptr_t adr = ARMHook::GetLibraryAddress("libGTASA.so");
	LibAddr = reinterpret_cast<uintptr_t>(cleo->GetMainLibraryLoadAddress());
	Game.initScriptsEvent();
	CleoFileDir1 = cleo->GetCleoStorageDir();
	cleo->PrintToCleoLog(CleoFileDir1.c_str());
	cleo->RegisterOpcode(0x0B8E, INT_ADD);
	cleo->RegisterOpcode(0x0B8F, INT_SUB);
	cleo->RegisterOpcode(0x0B90, INT_MUL);
	cleo->RegisterOpcode(0x0B91, INT_DIV);
	cleo->RegisterOpcode(0x0A96, GET_PED_POINTER);
	cleo->RegisterOpcode(0x0A97, GET_VEHICLE_POINTER);
	cleo->RegisterOpcode(0x0A98, GET_OBJECT_POINTER);

	cleo->RegisterOpcode(0x0A99, CHDIR);
	cleo->RegisterOpcode(0x0A9A, OPEN_FILE);
	cleo->RegisterOpcode(0x0A9B, CLOSE_FILE);
	cleo->RegisterOpcode(0x0A9C, GET_FILE_SIZE);
	cleo->RegisterOpcode(0x0A9D, READ_FROM_FILE);
	cleo->RegisterOpcode(0x0A9E, WRITE_TO_FILE);

	cleo->RegisterOpcode(0x0A9F, GET_THIS_SCRIPT_STRUCT);
	cleo->RegisterOpcode(0x0AC7, GET_VAR_POINTER);
	cleo->RegisterOpcode(0x0ACA, PRINT_HELP_STRING);
	cleo->RegisterOpcode(0x0ACB, PRINT_BIG_STRING);
	cleo->RegisterOpcode(0x0ACC, PRINT_STRING);
	cleo->RegisterOpcode(0x0ACD, PRINT_STRING_NOW);
	cleo->RegisterOpcode(0x0ACE, PRINT_HELP_FORMATTED);
	cleo->RegisterOpcode(0x0ACF, PRINT_BIG_FORMATTED);
	cleo->RegisterOpcode(0x0AD0, PRINT_FORMATTED);
	cleo->RegisterOpcode(0x0AD1, PRINT_FORMATTED_NOW);

	cleo->RegisterOpcode(0x0B10, BIT_AND);
	cleo->RegisterOpcode(0x0B11, BIT_OR);
	cleo->RegisterOpcode(0x0B12, BIT_XOR);
	cleo->RegisterOpcode(0x0B13, BIT_NOT);
	cleo->RegisterOpcode(0x0B14, BIT_MOD);
	cleo->RegisterOpcode(0x0B15, BIT_SHR);
	cleo->RegisterOpcode(0x0B16, BIT_SHL);
	cleo->RegisterOpcode(0x0B17, BITWISE_AND_THING_WITH_THING);
	cleo->RegisterOpcode(0x0B18, BITWISE_OR_THING_WITH_THING);
	cleo->RegisterOpcode(0x0B19, BITWISE_XOR_THING_WITH_THING);
	cleo->RegisterOpcode(0x0B1A, BITWISE_NOT);
	cleo->RegisterOpcode(0x0B1B, BITWISE_MOD_THING_BY_THING);
	cleo->RegisterOpcode(0x0B1C, BITWISE_SHR_THING_BY_THING);
	cleo->RegisterOpcode(0x0B1D, BITWISE_SHL_THING_BY_THING);

	cleo->RegisterOpcode(0x0D5C, GET_CAR_LIGHT_DAMAGE_STATUS);
	cleo->RegisterOpcode(0x0D5D, SET_CAR_LIGHT_DAMAGE_STATUS);
	//cleo->RegisterOpcode(0x0C04, testa);
	//cleo->RegisterOpcode(0x0C05, testb);
	cleo->PrintToCleoLog("'libNewCommands.so' plugin init succeed");
}

int NewCommands::GET_GAME_VER()
{
	GameVersion = cleo->GetGameVersionInternal();
	switch (GameVersion)
	{
	case VER_GTA3_1_6:
	case VER_GTAVC_1_06://1.07
	case VER_GTASA_1_08:
		return false;
	case VER_GTA3_1_8_OR_HIGHER:
	case VER_GTAVC_1_09_OR_HIGHER:
	case VER_GTASA_2_00_OR_HIGHER:
	case VER_GTALCS_2_4_OR_HIGHER:
		return true;
	default:
		break;
	}
	return -1;
}

extern "C" __attribute__((visibility("default"))) void plugin_init(cleo_ifs_t * ifs)
{
	cleo = ifs;
	NewCommands();
}