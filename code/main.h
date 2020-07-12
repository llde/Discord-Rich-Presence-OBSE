#pragma once
#include "obse/PluginAPI.h"
#include "obse/GameMenus.h"
#include "obse/GameObjects.h"
#include "obse/GameData.h"
#include "obse/GameProcess.h"
#include "obse/GameTasks.h"
#include "obse/GameRTTI.h"
#include "obse/GameOSDepend.h"
#include "obse/GameTiles.h"
#include "obse/GameAPI.h"
#include "obse/obse_common/SafeWrite.h"
#include <time.h>
#include <discord-rpc\include\discord_rpc.h>
#include <obse\obse\GameData.h>
#include <clocale>
#include <mbstring.h>
#include <mbctype.h>

void HandleIniOptions();
HMODULE discordHandle;
extern void UpdateRPC();

int64_t StartTime;
bool bShowGold = true;
bool bShowLevel = true;
bool bShowName = true;
bool bShowHealth = true;
bool bShowPaused = true;
bool bShowLockpicking = true;
bool bShowLocation = true;
bool bShowSleeping = true;
bool bShowInventory = true;
bool bShowBooks = true;
bool bShowDialog = true;
bool bShowBarter = true;
bool bShowSpellmaking = true;
bool bShowAlchemy = true;
bool bShowEnchanting = true;
bool bShowMainMenu = true;
bool bShowCredits = true;
bool bShowRace = true;
bool bInitialized = false;
InterfaceManager* g_interfaceManager;

bool versionCheck(const OBSEInterface* obse) {
	return !(obse->isEditor || obse->obseVersion < OBSE_VERSION_INTEGER);
}

WCHAR* temporaryHolder = (WCHAR*)calloc(512, sizeof(WCHAR));
char* newState = (char*)calloc(512, sizeof(char));
char* newDetails = (char*)calloc(512, sizeof(char));

bool NormalizeToUTF8(char* str, char* dest) {
	if (str == NULL) return false;
	memset(temporaryHolder, 0, 512 * sizeof(WCHAR));
	memset(dest, 0, 512 * sizeof(char));
	int res = MultiByteToWideChar(CP_ACP, 0, str, -1, temporaryHolder, 512);
	if (res == 0) return false;
	res = WideCharToMultiByte(CP_UTF8, 0, temporaryHolder, -1, dest, 512, NULL, NULL);
	if (res == 0) return false;
	return true;
}
static void UpdateDiscordPresence(char* state, char* details)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	//setlocale(LC_ALL, ".UTF-8");
	if (strlen(state) > 0)
		if (NormalizeToUTF8(state, newState) == false) {
			_MESSAGE("Can't normalize string %s . error:  %x", state, GetLastError());
			discordPresence.state = state;
		}
		else {
			discordPresence.state = newState;
		}
	if (strlen(details) > 0)
		if (NormalizeToUTF8(details, newDetails) == false) {
			_MESSAGE("Can't normalize string %s . error:  %x", details, GetLastError());
			discordPresence.details = details;
		}
		else {
			discordPresence.details = newDetails;
		}
	discordPresence.startTimestamp = StartTime;
	discordPresence.largeImageKey = "main2";
	discordPresence.instance = 0;
	Discord_UpdatePresence(&discordPresence);
	Discord_RunCallbacks();
}

static void DiscordInit()
{
	const char* initialisationID = "723600413680599110";
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(initialisationID, &handlers, 0, NULL);
	StartTime = time(0);
	g_interfaceManager = InterfaceManager::GetSingleton();
	bInitialized = true;
	UpdateRPC();
}

UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	return ThisStdCall(0x57CF60, g_interfaceManager);
}

int GetPlayerGold() {
	return ThisStdCall(0x4DE160, *g_thePlayer);
}

double GetPlayerHealthPercentage() {
	double baseHealth = (*g_thePlayer)->GetBaseActorValue(kActorVal_Health);
	if (baseHealth) {
		return (*g_thePlayer)->GetAV_F(kActorVal_Health) / baseHealth;
	}
	else {
		return 1.0;
	}
}

int GetPlayerLevel() {
	return ThisStdCall(0x5E1FD0, *g_thePlayer);
}
const char* GetRaceName() {
	const char* name = ((TESNPC*)(*g_thePlayer)->baseForm)->race.race->fullName.name.m_data;
	if (name == NULL) name = ((TESNPC*)(*g_thePlayer)->baseForm)->race.race->GetEditorName();
	return name;
}


void GetCurrentCellName(const char** dst) {
	TESObjectCELL* current_cell = (*g_thePlayer)->parentCell;
	*dst = current_cell->fullName.name.m_data;
	if (*dst != NULL) return;
	if (current_cell->IsInterior()) {
		*dst = current_cell->GetEditorName();
	}
	else {	
		ExtraRegionList* list = (ExtraRegionList*)current_cell->extraData.GetByType(kExtraData_RegionList);
		if (list != NULL) {
			TESRegionList* tl = list->regionList;
			if (tl != NULL) {
				for (TESRegionList::Entry* next = &tl->regionList; next != NULL; next = next->next) {
					TESRegion* region = next->region;
					if (region == NULL) {
						continue;
					}
					for (TESRegion::RegionDataEntry* entry = &(region->dataList->entry); entry != NULL; entry = entry->next) {
						TESRegionDataMap* map = (TESRegionDataMap*)Oblivion_DynamicCast(entry->data, 0, RTTI_TESRegionData, RTTI_TESRegionDataMap, 0);
						if (map != NULL) {
							if (map->regionName.m_data) {
								*dst = map->regionName.m_data;
								return;
							}
						}
					}
				}
			}
		}
		TESWorldSpace* world = current_cell->worldSpace;
		if (*dst == NULL) *dst = world->fullName.name.m_data;
		if (*dst == NULL) *dst = current_cell->GetEditorName();
		if (*dst == NULL) *dst = world->GetEditorName();
	}
}

bool IsAtMainMenu()
{
	return *(byte*)0xB3C0EC;

}

SInt16 callDelay = 1200;
UInt8 initDelay = 10;
void MainLoopHook() {
	if (!bInitialized) {
		if (!initDelay) DiscordInit();
		--initDelay;
	}
	if (callDelay) {
		--callDelay;
	}
	else {
		UpdateRPC();
		callDelay = 1200;
	}
	ThisStdCall(0x498F00, NULL);
}