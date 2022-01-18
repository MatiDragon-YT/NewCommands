#pragma once
#include <set>

class GameFunc {
public:
	std::set<FILE*>* openedFiles;
	void initScriptsEvent();
};



class CPed;
class CVehicle;
class CObject;

class CPools
{
public:
	static CPed* GetPed(int handle);
	static CVehicle* GetVehicle(int handle);
	static CObject* GetObject(int handle);
};

/*
class CFileMgr
{
public:
	static void SetDirMyDocuments()
	{
		return CArmCall::Function<void>(reinterpret_cast<int>(cleo->GetMainLibrarySymbol("_ZN8CFileMgr17SetDirMyDocumentsEv")));
	}
};*/

//_ZN8CFileMgr17SetDirMyDocumentsEv

//_ZN8CFileMgr6SetDirEPKc




class CHud
{
public:
	static void SetHelpMessage(char const* key, wchar_t* text, bool quickMessage, bool permanent, bool addToBrief, unsigned int flag); //SA
	static void SetHelpMessage(wchar_t* text, bool quickMessage, bool permanent, bool addToBrief); //VC
	static void SetHelpMessage(wchar_t* text, bool quickMessage); //GTA3
	static void SetHelpMessage(wchar_t* text, bool quickMessage, bool permanent); //LCS
};

class CMessages
{
public:
	static void AddBigMessageQ(wchar_t* text, unsigned int time, unsigned short style);
	static void AddMessage(char const* key, wchar_t* text, unsigned int time, unsigned short flag, bool bPreviousBrief); //SA
	static void AddMessage(wchar_t* text, unsigned int time, unsigned short flag); //VC GTA3 LCS
	static void AddMessageJumpQ(char const* key, wchar_t* text, unsigned int time, unsigned short flag, bool bPreviousBrief); //SA
	static void AddMessageJumpQ(wchar_t* text, unsigned int time, unsigned short flag); //VC GTA3
	static void AddMessageJumpQ(wchar_t* text, unsigned int time, unsigned short flag, unsigned char up1, unsigned char up2); //LCS
};



class CDamageManager
{
public:
	enum eLights {
		LIGHT_FRONT_LEFT = 0,
		LIGHT_FRONT_RIGHT = 1,
		LIGHT_REAR_RIGHT = 2, //GTA3 LCS: LIGHT_REAR_LEFT = 2
		LIGHT_REAR_LEFT = 3   //GTA3 LCS: LIGHT_REAR_RIGHT = 3
	};
	void SetLightStatus(eLights light, unsigned int status);
	unsigned int GetLightStatus(eLights light);
};

class CTheScripts
{
public:
	static void Init();
};