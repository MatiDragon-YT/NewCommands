#include "GameFunc.h"
#include "NewCommands.h"

CPed* CPools::GetPed(int handle)
{
	return CArmCall::Function<CPed*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools6GetPedEi")), handle);
}

CVehicle* CPools::GetVehicle(int handle)
{
	return CArmCall::Function<CVehicle*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools10GetVehicleEi")), handle);
}

CObject* CPools::GetObject(int handle)
{
	return CArmCall::Function<CObject*, int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN6CPools9GetObjectEi")), handle);
}

//SA
void CHud::SetHelpMessage(char const* key, wchar_t* text, bool quickMessage, bool permanent, bool addToBrief, unsigned int flag)
{
	return CArmCall::Function<void, char const*, wchar_t*, bool, bool, bool, unsigned int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN4CHud14SetHelpMessageEPKcPtbbbj")), key, text, quickMessage, permanent, addToBrief, flag);
}

//VC
void CHud::SetHelpMessage(wchar_t* text, bool quickMessage, bool permanent, bool addToBrief)
{
	return CArmCall::Function<void, wchar_t*, bool, bool, bool>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN4CHud14SetHelpMessageEPtbbb")), text, quickMessage, permanent, addToBrief);
}

//GTA3
void CHud::SetHelpMessage(wchar_t* text, bool quickMessage)
{
	return CArmCall::Function<void, wchar_t*, bool>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN4CHud14SetHelpMessageEPtb")), text, quickMessage);
}

//LCS
void CHud::SetHelpMessage(wchar_t* text, bool quickMessage, bool permanent)
{
	return CArmCall::Function<void, wchar_t*, bool, bool>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN4CHud14SetHelpMessageEPtbb")), text, quickMessage, permanent);
}


void CMessages::AddBigMessageQ(wchar_t* text, unsigned int time, unsigned short style)
{
	return CArmCall::Function<void, wchar_t*, unsigned int, unsigned short>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages14AddBigMessageQEPtjt")), text, time, style);
}

//SA
void CMessages::AddMessage(char const* key, wchar_t* text, unsigned int time, unsigned short flag, bool bPreviousBrief)
{
	return CArmCall::Function<void, char const*, wchar_t*, unsigned int, unsigned short, bool>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages10AddMessageEPKcPtjtb")), key, text, time, flag, bPreviousBrief);
}

//VC GTA3 LCS
void CMessages::AddMessage(wchar_t* text, unsigned int time, unsigned short flag)
{
	return CArmCall::Function<void, wchar_t*, unsigned int, unsigned short>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages10AddMessageEPtjt")), text, time, flag);
}

//SA
void CMessages::AddMessageJumpQ(char const* key, wchar_t* text, unsigned int time, unsigned short flag, bool bPreviousBrief)
{
	return CArmCall::Function<void, char const*, wchar_t*, unsigned int, unsigned short, bool>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages15AddMessageJumpQEPKcPtjtb")), key, text, time, flag, bPreviousBrief);
}

//VC GTA3
void CMessages::AddMessageJumpQ(wchar_t* text, unsigned int time, unsigned short flag)
{
	return CArmCall::Function<void, wchar_t*, unsigned int, unsigned short>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages15AddMessageJumpQEPtjt")), text, time, flag);
}

//LCS
void CMessages::AddMessageJumpQ(wchar_t* text, unsigned int time, unsigned short flag, unsigned char up1, unsigned char up2)
{
	return CArmCall::Function<void, wchar_t*, unsigned int, unsigned short, unsigned char, unsigned char>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN9CMessages15AddMessageJumpQEPtjthh")), text, time, flag, up1, up2);
}



void CDamageManager::SetLightStatus(eLights light, unsigned int status)
{
	return CArmCall::Method<void, CDamageManager*, eLights, unsigned int>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN14CDamageManager14SetLightStatusE7eLightsj")), this, light, status);
}