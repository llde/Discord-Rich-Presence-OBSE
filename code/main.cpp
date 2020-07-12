#include <obse\obse_common\SafeWrite.h>
#include <obse\obse\PluginAPI.h>
#include "main.h"
#include "commands.h"

#include <locale.h>

void MessageHandler(OBSEMessagingInterface::Message* msg);
IDebugLog debugLog("Oblivion Discord Rich Presence.log");

BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved) {
	if (dwReason == DLL_PROCESS_ATTACH)
		discordHandle = (HMODULE)hDllHandle;
	return TRUE;
}
extern "C" {
	bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "Discord Rich Presence";
		info->version = 2;
		return !(obse->isEditor || obse->obseVersion < OBSE_VERSION_INTEGER);
	}

	bool OBSEPlugin_Load(const OBSEInterface* obse)
	{
		_MESSAGE("Locale :  %s", setlocale(LC_ALL, NULL));
	//	_MESSAGE("Changed locale to : %s ", setlocale(LC_ALL, ".UTF-8"));
		((OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging))->RegisterListener(obse->GetPluginHandle(), "OBSE", MessageHandler);
		HandleIniOptions();
		return true;
	}
};
void HandleIniOptions() {
//	_CrtSetDebugFillThreshold(0);
	char filename[MAX_PATH] = {0};
	memset(filename,0, MAX_PATH);
	GetModuleFileNameA(discordHandle, filename, MAX_PATH);
	char* end_ptr = strrchr(filename, '\\');
	strcpy_s(end_ptr + 1, MAX_PATH - (end_ptr - filename) -1, "discordrpc_obse.ini");
	bShowGold = GetPrivateProfileIntA("GENERAL", "bShowGold", 1, filename);
	bShowLevel = GetPrivateProfileIntA("GENERAL", "bShowLevel", 1, filename);
	bShowHealth = GetPrivateProfileIntA("GENERAL", "bShowHealth", 1, filename);
	bShowName = GetPrivateProfileIntA("GENERAL", "bShowName", 1, filename);
	bShowRace = GetPrivateProfileIntA("GENERAL", "bShowRace", 1, filename);
	bShowPaused = GetPrivateProfileIntA("GENERAL", "bShowPaused", 1, filename);
	bShowLockpicking = GetPrivateProfileIntA("GENERAL", "bShowLockpicking", 1, filename);
	bShowLocation = GetPrivateProfileIntA("GENERAL", "bShowLocation", 1, filename);
	bShowSleeping = GetPrivateProfileIntA("GENERAL", "bShowSleeping", 1, filename);
	bShowInventory = GetPrivateProfileIntA("GENERAL", "bShowInventory", 1, filename);
	bShowBooks = GetPrivateProfileIntA("GENERAL", "bShowBooks", 1, filename);
	bShowMainMenu = GetPrivateProfileIntA("GENERAL", "bShowMainMenu", 1, filename);
	bShowDialog = GetPrivateProfileIntA("GENERAL", "bShowDialog", 1, filename);
	bShowBarter = GetPrivateProfileIntA("GENERAL", "bShowBarter", 1, filename);
	bShowSpellmaking = GetPrivateProfileIntA("GENERAL", "bShowSpellmaking", 1, filename);
	bShowAlchemy = GetPrivateProfileIntA("General", "bShowAlchemy", 1, filename);
	bShowEnchanting = GetPrivateProfileIntA("General", "bShowEnchanting", 1, filename);
	bShowCredits = GetPrivateProfileIntA("GENERAL", "bShowCredits", 1, filename); 
}
void MessageHandler(OBSEMessagingInterface::Message* msg)
{
	switch (msg->type) {
	case OBSEMessagingInterface::kMessage_ExitGame_Console:
	case OBSEMessagingInterface::kMessage_ExitGame:
		Discord_Shutdown();
		break;
	case OBSEMessagingInterface::kMessage_PostPostLoad:
	{
		WriteRelCall(0x40D893, (UInt32)MainLoopHook);
		break;
	}
	default:
		break;
	}
}