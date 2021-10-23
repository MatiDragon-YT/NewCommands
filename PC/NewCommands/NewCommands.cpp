#include "plugin.h"
#include "VC.CLEO.h"
#include "CWeather.h"
#include "CClock.h"
#include "CTimer.h"
#include "CDraw.h"
#include "CHud.h"
#include "CMenuManager.h"
#include "CCamera.h"
#include "CAnimManager.h"
#include "CAnimGroup.h"
#include "ObjectScalingExtension.h"

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  5
#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

using namespace plugin;
tScriptVar* Params;
int GV = -1;

//std::vector<CAnimationStyleDescriptor> AnimGroup(61);
std::vector<CAnimGroup> AnimGroup(61);
std::vector<char*> IFPList(0);

void Hook_405604();
void PatchAnimGroup();
void CustomIFPList();
void GetIFPData(char* IFPName);
void LoadNewIFP();

eOpcodeResult WINAPI GET_NEXT_WEATHER(CScript* script);
eOpcodeResult WINAPI SET_NEXT_WEATHER(CScript* script);
eOpcodeResult WINAPI GET_RAIN_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_RAIN_INTENSITY(CScript* script); 
eOpcodeResult WINAPI GET_RAINBOW_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_RAINBOW_INTENSITY(CScript* script);
eOpcodeResult WINAPI GET_FOGGY_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_FOGGY_INTENSITY(CScript* script);
eOpcodeResult WINAPI GET_CLOUD_COVERAGE_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_CLOUD_COVERAGE_INTENSITY(CScript* script);
eOpcodeResult WINAPI GET_SUN_GLARE_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_SUN_GLARE_INTENSITY(CScript* script);
eOpcodeResult WINAPI GET_WIND_INTENSITY(CScript* script);
eOpcodeResult WINAPI SET_WIND_INTENSITY(CScript* script);
eOpcodeResult WINAPI GET_TRAFFIC_LIGHT_BRIGHTNESS(CScript* script);
eOpcodeResult WINAPI SET_TRAFFIC_LIGHT_BRIGHTNESS(CScript* script);
eOpcodeResult WINAPI GET_FORCED_WEATHER(CScript* script);

eOpcodeResult WINAPI GET_TIME_IS_SECONDS(CScript* script);
eOpcodeResult WINAPI SET_TIME_IS_SECONDS(CScript* script);
eOpcodeResult WINAPI GET_STORED_TIME(CScript* script);
eOpcodeResult WINAPI SET_STORE_TIME(CScript* script);
eOpcodeResult WINAPI GET_LOCAL_TIME(CScript* script);
eOpcodeResult WINAPI GET_TIME_SCALE(CScript* script);
eOpcodeResult WINAPI FRAME_MOD(CScript* script);
eOpcodeResult WINAPI PASS_TIME(CScript* script);
eOpcodeResult WINAPI SET_TIME_UPDATE(CScript* script);

eOpcodeResult WINAPI GET_CURRENT_RESOLUTION(CScript* script);
eOpcodeResult WINAPI FIX_RESOLUTION_CONVERSION(CScript* script);
eOpcodeResult WINAPI GET_FADE_COLOUR(CScript* script);
eOpcodeResult WINAPI SET_FADE_ALPHA(CScript* script);
eOpcodeResult WINAPI SET_DRAW_ASPECT_RATIO(CScript* script);
eOpcodeResult WINAPI GET_DRAW_ASPECT_RATIO(CScript* script);
eOpcodeResult WINAPI SET_DRAW_FOV(CScript* script);
eOpcodeResult WINAPI GET_DRAW_FOV(CScript* script);
eOpcodeResult WINAPI DISPLAY_HUD(CScript* script);
eOpcodeResult WINAPI IS_HUD_VISIBLE(CScript* script);
eOpcodeResult WINAPI IS_RADAR_VISIBLE(CScript* script);


eOpcodeResult WINAPI LOAD_CUSTOM_ANIMATION(CScript* script);

eOpcodeResult WINAPI SET_OBJECT_SCALE(CScript* script);
eOpcodeResult WINAPI GET_OBJECT_SCALE(CScript* script);

eOpcodeResult WINAPI SET_TOMMY_FATNESS(CScript* script);

class NewCommands {
public:
    NewCommands() {

        if (CLEO_GetVersion() < CLEO_VERSION)
        {
            MessageBox(HWND_DESKTOP, "not support old CLEO version, please update your CLEO library version.", "NewCommands.cleo", MB_ICONERROR);
            return;
        }
        GV = GetGameVersion();
        if (GV != GAME_10EN)
        {
            MessageBox(HWND_DESKTOP, "NewCommands is Only supports GTA VC 1.0 EN game version.", "NewCommands.cleo", MB_ICONERROR);
            return;
        }
        patch::RedirectJump(0x405604, Hook_405604);
        Params = CLEO_GetParamsAddress();
        Opcodes::RegisterOpcode(0x0C00, GET_NEXT_WEATHER);
        Opcodes::RegisterOpcode(0x0C01, SET_NEXT_WEATHER);
        Opcodes::RegisterOpcode(0x0C02, GET_RAIN_INTENSITY);
        Opcodes::RegisterOpcode(0x0C03, SET_RAIN_INTENSITY);
        Opcodes::RegisterOpcode(0x0C04, GET_RAINBOW_INTENSITY);
        Opcodes::RegisterOpcode(0x0C05, SET_RAINBOW_INTENSITY);
        Opcodes::RegisterOpcode(0x0C06, GET_FOGGY_INTENSITY);
        Opcodes::RegisterOpcode(0x0C07, SET_FOGGY_INTENSITY);
        Opcodes::RegisterOpcode(0x0C08, GET_CLOUD_COVERAGE_INTENSITY);
        Opcodes::RegisterOpcode(0x0C09, SET_CLOUD_COVERAGE_INTENSITY);
        Opcodes::RegisterOpcode(0x0C0A, GET_SUN_GLARE_INTENSITY);
        Opcodes::RegisterOpcode(0x0C0B, SET_SUN_GLARE_INTENSITY);
        Opcodes::RegisterOpcode(0x0C0C, GET_WIND_INTENSITY);
        Opcodes::RegisterOpcode(0x0C0D, SET_WIND_INTENSITY);
        Opcodes::RegisterOpcode(0x0C0E, GET_TRAFFIC_LIGHT_BRIGHTNESS);
        Opcodes::RegisterOpcode(0x0C0F, SET_TRAFFIC_LIGHT_BRIGHTNESS);
        Opcodes::RegisterOpcode(0x0C10, GET_FORCED_WEATHER);

        Opcodes::RegisterOpcode(0x0C11, GET_TIME_IS_SECONDS);
        Opcodes::RegisterOpcode(0x0C12, SET_TIME_IS_SECONDS);
        Opcodes::RegisterOpcode(0x0C13, GET_STORED_TIME);
        Opcodes::RegisterOpcode(0x0C14, SET_STORE_TIME);
        Opcodes::RegisterOpcode(0x0C15, GET_LOCAL_TIME);
        Opcodes::RegisterOpcode(0x0C16, GET_TIME_SCALE);
        Opcodes::RegisterOpcode(0x0C17, FRAME_MOD);
        Opcodes::RegisterOpcode(0x0C18, PASS_TIME);
        Opcodes::RegisterOpcode(0x0C19, SET_TIME_UPDATE);

        Opcodes::RegisterOpcode(0x0C1A, GET_CURRENT_RESOLUTION);
        Opcodes::RegisterOpcode(0x0C1B, FIX_RESOLUTION_CONVERSION);
        Opcodes::RegisterOpcode(0x0C1C, GET_FADE_COLOUR);
        Opcodes::RegisterOpcode(0x0C1D, SET_FADE_ALPHA);
        Opcodes::RegisterOpcode(0x0C1E, SET_DRAW_ASPECT_RATIO);
        Opcodes::RegisterOpcode(0x0C1F, GET_DRAW_ASPECT_RATIO);
        Opcodes::RegisterOpcode(0x0C20, SET_DRAW_FOV);
        Opcodes::RegisterOpcode(0x0C21, GET_DRAW_FOV);
        Opcodes::RegisterOpcode(0x0C22, DISPLAY_HUD);
        Opcodes::RegisterOpcode(0x0C23, IS_HUD_VISIBLE);
        Opcodes::RegisterOpcode(0x0C24, IS_RADAR_VISIBLE);


        

        Opcodes::RegisterOpcode(0x0C30, LOAD_CUSTOM_ANIMATION);

        Opcodes::RegisterOpcode(0x0C50, SET_OBJECT_SCALE);
        Opcodes::RegisterOpcode(0x0C51, GET_OBJECT_SCALE);

        Opcodes::RegisterOpcode(0x0C60, SET_TOMMY_FATNESS);
        return;
    }
} newCommands;

__declspec(naked) void Hook_405604()
{
    __asm
    {
        mov eax, 6458F0h;
        call eax;
        pop ecx;
        pop ecx;
    }
    PatchAnimGroup();
    __asm
    {
        mov eax, 40560Bh;
        jmp eax;
    }
}

void PatchAnimGroup()
{
    CustomIFPList();
    CAnimGroup * DefaultPool = (CAnimGroup*)0x6857B0;
    for (int i = 0; i < 61; i++)
    {
        AnimGroup[i] = DefaultPool[i];
    }
    for (unsigned int i = 0; i < IFPList.size(); i++)
        GetIFPData(IFPList[i]);
    patch::SetPointer(0x40598A, &AnimGroup[0].groupName);
    patch::SetPointer(0x40545F, &AnimGroup[0].blockName);
    patch::SetPointer(0x405531, &AnimGroup[0].blockName);
    patch::SetPointer(0x4054C3, &AnimGroup[0].modelIndex);
    patch::SetPointer(0x405519, &AnimGroup[0].animsCount);
    patch::SetPointer(0x40551F, &AnimGroup[0].animNames);
    patch::SetPointer(0x4054FF, &AnimGroup[0].animDesc);
    patch::SetPointer(0x405559, &AnimGroup[0].animDesc);

    patch::SetChar(0x4049C9, AnimGroup.size());
    patch::SetChar(0x4055CB, AnimGroup.size());
    patch::SetChar(0x40560C, AnimGroup.size());
    patch::SetInt(0x40561A, 8 + 0x14 * AnimGroup.size());
    patch::SetInt(0x48BD84, AnimGroup.size());
    patch::SetChar(0x48BE63, AnimGroup.size());
    LoadNewIFP();
}

void CustomIFPList()
{
    WIN32_FIND_DATA FileData;
    HANDLE hFind;
    char* CurPath = new char[512];
    GetCurrentDirectory(512, CurPath);
    const char* IFPath = "\\CLEO\\CLEO_ANIM\\*.ifp";
    strcat(CurPath, IFPath);
    hFind = FindFirstFile(CurPath, &FileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        IFPList.push_back(new char[sizeof(FileData.cFileName)]);
        strcpy(IFPList[IFPList.size() - 1], FileData.cFileName);
        while (FindNextFile(hFind, &FileData) != 0)
        {
            IFPList.push_back(new char[sizeof(FileData.cFileName)]);
            strcpy(IFPList[IFPList.size() - 1], FileData.cFileName);
        }
    }
    FindClose(hFind);
}

void GetIFPData(char* IFPName)
{
    CAnimGroup temp;
    temp.modelIndex = 1;
    std::vector<char*>* templist = new std::vector<char*>(0);
    std::vector<animDesc>* tempindex = new std::vector<animDesc>(0);
    temp.blockName = new char[strlen(IFPName) - 4];
    strncpy(temp.blockName, IFPName, strlen(IFPName) - 4);
    temp.blockName[strlen(IFPName) - 4] = 0;
    char* CurPath = new char[512];
    GetCurrentDirectory(512, CurPath);
    const char* IFPath = "\\CLEO\\CLEO_ANIM\\";
    strcat(CurPath, IFPath);
    strcat(CurPath, IFPName);
    FILE* ifp;
    if (ifp = fopen(CurPath, "rb"))
    {
        fseek(ifp, 0xC, SEEK_SET);
        int strsize;
        //std::cout << "1" << std::endl;
        fread(&strsize, 4, 1, ifp);
        fseek(ifp, 4, SEEK_CUR);
        temp.groupName = new char[strsize - 4];
        fgets(temp.groupName, strsize - 3, ifp);
        if (strsize % 4 != 0)
            fseek(ifp, 4 - (strsize % 4), SEEK_CUR);
        int i = 0;
        char* NAME = new char[5];
        NAME[4] = 0;
        //std::cout << "2" << std::endl;
        fread(NAME, 4, 1, ifp);
        while (!(feof(ifp)))
        {
            if (strcmp(NAME, "NAME") != 0)
                break;
            //std::cout << "3" << std::endl;
            fread(&strsize, 4, 1, ifp);
            //std::cout << strsize << std::endl;
            char* tempchar = new char[strsize];
            fgets(tempchar, strsize + 1, ifp);
            templist->push_back(tempchar);
            animDesc temind = { i,2 };
            tempindex->push_back(temind);
            //std::cout << "Animation" << i << " Name:" << ifpgroupname << std::endl;
            if (strsize % 4 != 0)
                fseek(ifp, 4 - (strsize % 4), SEEK_CUR);
            fseek(ifp, 4, SEEK_CUR);
            //std::cout << "4" << std::endl;
            fread(&strsize, 4, 1, ifp);
            fseek(ifp, strsize, SEEK_CUR);
            i++;
            //std::cout << "5" << std::endl;
            fread(NAME, 4, 1, ifp);
        }
        fclose(ifp);
        temp.animsCount = i;
        temp.animNames = &((*templist)[0]);
        temp.animDesc = &((*tempindex)[0]);
        AnimGroup.push_back(temp);
    }
    delete[]CurPath;
}

void LoadNewIFP()
{
    for (unsigned int i = 0; i < IFPList.size(); i++)
    {
        char* tempchr = new char[sizeof(IFPList[i]) + 16];
        strcpy(tempchr, "CLEO\\CLEO_ANIM\\");
        strcat(tempchr, IFPList[i]);
        //gtalog(tempchr);
        RwStream* stream;
        stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, tempchr);
        CAnimManager::LoadAnimFile(stream, 1, 0);
        RwStreamClose(stream, 0);
        delete[]tempchr;
    }
}

//Opcode: 0C00=1,%1d% = get_next_weather
//commands: GET_NEXT_WEATHER
eOpcodeResult WINAPI GET_NEXT_WEATHER(CScript* script)
{
    Params[0].nVar = CWeather::NewWeatherType;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C01=1,set_next_weather %1d%
//commands: SET_NEXT_WEATHER
eOpcodeResult WINAPI SET_NEXT_WEATHER(CScript* script)
{
    script->Collect(1);
    CWeather::NewWeatherType = Params[0].nVar;
    return OR_CONTINUE;
}

//Opcode: 0C02=1,%1d% = get_rain_intensity
//commands: GET_RAIN_INTENSITY
eOpcodeResult WINAPI GET_RAIN_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0x975340;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C03=1,set_rain_intensity %1d%
//commands: SET_RAIN_INTENSITY
eOpcodeResult WINAPI SET_RAIN_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0x975340 = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C04=1,%1d% = get_rainbow_intensity
//commands: GET_RAINBOW_INTENSITY
eOpcodeResult WINAPI GET_RAINBOW_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0xA0D05C;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C05=1,set_rainbow_intensity %1d%
//commands: SET_RAINBOW_INTENSITY
eOpcodeResult WINAPI SET_RAINBOW_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0xA0D05C = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C06=1,%1d% = get_foggy_intensity
//commands: GET_FOGGY_INTENSITY
eOpcodeResult WINAPI GET_FOGGY_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0x94DDC0;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C07=1,set_foggy_intensity %1d%
//commands: SET_FOGGY_INTENSITY
eOpcodeResult WINAPI SET_FOGGY_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0x94DDC0 = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C08=1,%1d% = get_cloud_coverage_intensity
//commands: GET_CLOUD_COVERAGE_INTENSITY
eOpcodeResult WINAPI GET_CLOUD_COVERAGE_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0x974BE8;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C09=1,set_cloud_coverage_intensity %1d%
//commands: SET_CLOUD_COVERAGE_INTENSITY
eOpcodeResult WINAPI SET_CLOUD_COVERAGE_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0x974BE8 = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C0A=1,%1d% = get_sun_glare_intensity
//commands: GET_SUN_GLARE_INTENSITY
eOpcodeResult WINAPI GET_SUN_GLARE_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0xA0FD7C;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C0B=1,set_sun_glare_intensity %1d%
//commands: SET_SUN_GLARE_INTENSITY
eOpcodeResult WINAPI SET_SUN_GLARE_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0xA0FD7C = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C0C=1,%1d% = get_wind_intensity
//commands: GET_WIND_INTENSITY
eOpcodeResult WINAPI GET_WIND_INTENSITY(CScript* script)
{
    Params[0].fVar = *(float*)0x97533C;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C0D=1,set_wind_intensity %1d%
//commands: SET_WIND_INTENSITY
eOpcodeResult WINAPI SET_WIND_INTENSITY(CScript* script)
{
    script->Collect(1);
    *(float*)0x97533C = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C0E=1,%1d% = get_traffic_light_brightness
//commands: GET_TRAFFIC_LIGHT_BRIGHTNESS
eOpcodeResult WINAPI GET_TRAFFIC_LIGHT_BRIGHTNESS(CScript* script)
{
    Params[0].fVar = *(float*)0x97F2D4;
    script->Store(1);
    return OR_CONTINUE;
}

//Opcode: 0C0F=1,set_traffic_light_brightness %1d%
//commands: SET_TRAFFIC_LIGHT_BRIGHTNESS
eOpcodeResult WINAPI SET_TRAFFIC_LIGHT_BRIGHTNESS(CScript* script)
{
    script->Collect(1);
    *(float*)0x97F2D4 = Params[0].fVar;
    return OR_CONTINUE;
}

//Opcode: 0C10=1,%1d% = get_forced_weather
//commands: GET_FORCED_WEATHER
eOpcodeResult WINAPI GET_FORCED_WEATHER(CScript* script)
{
    Params[0].nVar = CWeather::ForcedWeatherType;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_TIME_IS_SECONDS(CScript* script)
{
    Params[0].nVar = CClock::ms_nGameClockSeconds;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_TIME_IS_SECONDS(CScript* script)
{
    script->Collect(1);
    int h, m, s;
    s = Params[0].nVar;
    while (s >= 60)
    {
        s -= 60;
        m++;
    }
    CClock::ms_nGameClockSeconds = s;
    m += CClock::ms_nGameClockMinutes;
    h = CClock::ms_nGameClockHours;
    CClock::SetGameClock(h, m);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_STORED_TIME(CScript* script)
{
    Params[0].nVar = CClock::ms_Stored_nGameClockHours;
    Params[1].nVar = CClock::ms_Stored_nGameClockMinutes;
    Params[2].nVar = CClock::ms_Stored_nGameClockSeconds;
    script->Store(3);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_STORE_TIME(CScript* script)
{
    script->Collect(3);
    CClock::ms_Stored_nGameClockHours = Params[0].nVar;
    CClock::ms_Stored_nGameClockMinutes = Params[1].nVar;
    CClock::ms_Stored_nGameClockSeconds = Params[2].nVar;
    *(bool*)0xA10B3C = true;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_LOCAL_TIME(CScript* script)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    Params[0].nVar = time.wYear;
    Params[1].nVar = time.wMonth;
    Params[2].nVar = time.wDayOfWeek;
    Params[3].nVar = time.wDay;
    Params[4].nVar = time.wHour;
    Params[5].nVar = time.wMinute;
    Params[6].nVar = time.wSecond;
    Params[7].nVar = time.wMilliseconds;
    script->Store(8);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_TIME_SCALE(CScript* script)
{
    //Params[0].fVar = CTimer::ms_fTimeStep;
    Params[0].fVar = CTimer::ms_fTimeScale;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI FRAME_MOD(CScript* script)
{
    script->Collect(1);
    script->UpdateCompareFlag(CTimer::m_FrameCounter % Params[0].nVar == 0);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI PASS_TIME(CScript* script)
{
    script->Collect(1);
    plugin::Call<0x42B8A0, int>(Params[0].nVar);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_TIME_UPDATE(CScript* script)
{
    script->Collect(1);
    if (!Params[0].nVar)
    {
        injector::MakeRET(0x487010, 0, true);
    }
    else
    {
        injector::WriteMemory(0x487010, 0x6A, true);
    }
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_CURRENT_RESOLUTION(CScript* script)
{
    Params[0].nVar = RsGlobal.maximumWidth;
    Params[1].nVar = RsGlobal.maximumHeight;
    script->Store(2);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI FIX_RESOLUTION_CONVERSION(CScript* script)
{
    script->Collect(4);
    float rw = static_cast <float> (*(int*)0xA0FD04);
    float rh = static_cast <float> (*(int*)0xA0FD08);
    rw /= 640.0; rh /= 448.0;
    Params[0].fVar *= rw;
    Params[1].fVar *= rh;
    Params[2].fVar *= rw;
    Params[3].fVar *= rh;
    script->Store(4);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_FADE_COLOUR(CScript* script)
{
    Params[0].nVar = CDraw::FadeRed;
    Params[1].nVar = CDraw::FadeGreen;
    Params[2].nVar = CDraw::FadeBlue;
    Params[3].nVar = CDraw::FadeValue; //aplha
    script->Store(4);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_FADE_ALPHA(CScript* script)
{
    script->Collect(1);
    CDraw::FadeValue = Params[0].nVar;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_DRAW_ASPECT_RATIO(CScript* script)
{
    script->Collect(1);
    CDraw::ms_fAspectRatio = Params[0].fVar;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_DRAW_ASPECT_RATIO(CScript* script)
{
    Params[0].fVar = CDraw::ms_fAspectRatio;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_DRAW_FOV(CScript* script)
{
    script->Collect(1);
    CDraw::ms_fFOV = Params[0].fVar;
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_DRAW_FOV(CScript* script)
{
    Params[0].fVar = CDraw::ms_fFOV;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI DISPLAY_HUD(CScript* script)
{
    script->Collect(1);
    if (Params[0].nVar)
    {
        //CHud::m_Wants_To_Draw_Hud = true;
        FrontendMenuManager.m_PrefsShowHud = true;
    }
    else
    {
        //CHud::m_Wants_To_Draw_Hud = false;
        FrontendMenuManager.m_PrefsShowHud = false;
    }
    return OR_CONTINUE;
}

eOpcodeResult WINAPI IS_HUD_VISIBLE(CScript* script)
{
    if (CHud::m_Wants_To_Draw_Hud
        && FrontendMenuManager.m_PrefsShowHud
        && *(char*)0xA10AF6 != 1 
        && !TheCamera.m_WideScreenOn)
    {
        script->UpdateCompareFlag(true);
        return OR_CONTINUE;
    }
    script->UpdateCompareFlag(false);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI IS_RADAR_VISIBLE(CScript* script)
{
    if (FrontendMenuManager.m_nRadarMode != 2
        && !CHud::bScriptDontDisplayRadar
        && CHud::m_Wants_To_Draw_Hud
        && (CHud::m_ItemToFlash == 8 && CTimer::m_FrameCounter & 8 || CHud::m_ItemToFlash != 8)
        && *(char*)0xA10AF6 != 1
        && !TheCamera.m_WideScreenOn)
    {
        script->UpdateCompareFlag(true);
        return OR_CONTINUE;
    }
    script->UpdateCompareFlag(false);
    return OR_CONTINUE;
}


//Opcode: 0C30=4,load_custom_animation_ifp_name %1s% anim_name %2s% to_animgroup %3d% anim %4d% //if and set
//commands: LOAD_CUSTOM_ANIMATION
eOpcodeResult WINAPI LOAD_CUSTOM_ANIMATION(CScript* script)
{
    script->Collect(2);
    //CPed* ped = CPools::GetPed(Params[0].nVar);
    char* ifp_name = Params[0].cVar;
    char* anim_name = Params[1].cVar;
    int groupID, animID;
    bool T = false;
    for (unsigned int i = 0; i < AnimGroup.size(); i++)
    {
        if (_strnicmp(AnimGroup[i].blockName, ifp_name, strlen(ifp_name)) == 0)
        {
            for (int j = 0; j < AnimGroup[i].animsCount; j++)
            {
                if (_strnicmp(AnimGroup[i].animNames[j], anim_name, strlen(anim_name)) == 0)
                {
                    groupID = i;
                    animID = j;
                    T = true;
                    delete[]anim_name;
                    delete[]ifp_name;
                    break;
                }
            }
            if (T)
                break;
        }
    }
    if (!T)
    {
        Params[0].nVar = -1;
        Params[1].nVar = -1;
        //Params[2].nVar = -1;
        script->Store(2);
        script->UpdateCompareFlag(false);
        return OR_CONTINUE;
    }
    //AnimGroup[groupID].animDesc[animID].flags = Params[2].nVar;
    //Params[0].nVar = AnimGroup[groupID].animDesc[animID].flags;
    animID = AnimGroup[groupID].animDesc[animID].animId;
    //CAnimManager::BlendAnimation(ped->m_pRwClump, groupID, animID, Params[3].fVar);
    Params[0].nVar = groupID;
    Params[1].nVar = animID;
    script->Store(2);
    script->UpdateCompareFlag(true);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_OBJECT_SCALE(CScript* script)
{
    script->Collect(4);
    CObject* object = CPools::GetObject(Params[0].nVar);
    ObjectScalingExtension::SetObjectScale(object, Params[1].fVar, Params[2].fVar, Params[3].fVar);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI GET_OBJECT_SCALE(CScript* script)
{
    script->Collect(1);
    CObject* object = CPools::GetObject(Params[0].nVar);
    Params[0].fVar = ObjectScalingExtension::GetObjectScaleX(object);
    Params[1].fVar = ObjectScalingExtension::GetObjectScaleY(object);
    Params[2].fVar = ObjectScalingExtension::GetObjectScaleZ(object);
    script->Store(3);
    return OR_CONTINUE;
}

eOpcodeResult WINAPI SET_TOMMY_FATNESS(CScript* script)
{
    script->Collect(1);
    *(float*)0x68E058 = Params[0].fVar;
    return OR_CONTINUE;
}