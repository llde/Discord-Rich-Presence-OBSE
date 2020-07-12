#pragma once

UInt32 locationNameID = NULL;

void GetCurrentCellName(const char** dst);
void BuildStateStr();
void BuildDetailsStr();

char* stateStr = (char*)malloc(0x200);
char* detailsStr = (char*)malloc(0x200);

void UpdateRPC() {
	if (IsAtMainMenu())
	{
		if (bShowMainMenu) UpdateDiscordPresence("At Main Menu à", "");
	}
	else
	{
		BuildStateStr();
		BuildDetailsStr();
		UpdateDiscordPresence(stateStr, detailsStr);
	} 
}

char* buffer = (char*)malloc(0x1000);
void BuildStateStr()
{
	if (!stateStr) return;
	*stateStr = '\0';
	*buffer = 0;

	if (bShowName)
	{
		const char* playerName = GetFullName(*g_thePlayer);
		strcat_s(stateStr, MAX_PATH, playerName);
	}
	if (bShowRace)
	{
		if (strlen(stateStr) > 0) {
			sprintf(buffer, " (%s)", GetRaceName());
		}
		else {
			sprintf(buffer, "%s", GetRaceName());
		}
		strcat_s(stateStr, MAX_PATH, buffer);
	}
	if (bShowLevel)
	{
		int level = GetPlayerLevel();
		if (level <= 0) level = 1;
		else if (level >= 999) level = 999;

		if (strlen(stateStr) > 0) strcat(stateStr, " | ");
		sprintf(buffer, "LVL: %d", level);
		strcat_s(stateStr, MAX_PATH, buffer);
	}

	if (bShowHealth)
	{
		if (strlen(stateStr) > 0) strcat(stateStr, " | ");

		float playerHealth = GetPlayerHealthPercentage() * 100.0F;
		if (playerHealth < 0.0F) playerHealth = 0.0F;
		if (playerHealth > 100.0F) playerHealth = 100.0F;

		sprintf(buffer, "HP: %.2f%%", playerHealth);
		strcat_s(stateStr, MAX_PATH, buffer);
	}

	if (bShowGold)
	{
		if (strlen(stateStr) > 0) strcat_s(stateStr, MAX_PATH, " | ");

		int gold = GetPlayerGold();
		if (gold < 0) gold = 0;
		if (gold > 9999) {
			strcat_s(stateStr, MAX_PATH, "9999+ gold");
		}
		else {
			sprintf(buffer, "%d gold", gold);
			strcat_s(stateStr, MAX_PATH, buffer);
		}
	}

}

void BuildDetailsStr()
{
	*detailsStr = '\0';
	UInt32 currentMode = g_interfaceManager->currentMode[0] & 0xFF;
	if (currentMode == 2)
	{
		int menuID = g_interfaceManager->GetTopVisibleMenuID();
		switch (menuID) {
		case kMenuType_SleepWait:
			if (bShowSleeping) strcat_s(detailsStr, MAX_PATH, "Sleeping");
			break;
		case kMenuType_Pause:
			if (bShowPaused) strcat_s(detailsStr, MAX_PATH, "Paused");
			break;
		case kMenuType_LockPick:
			if (bShowLockpicking) strcat_s(detailsStr, MAX_PATH, "Picking a lock");
			break;
		case kMenuType_Spellmaking:
			if (bShowSpellmaking) strcat_s(detailsStr, MAX_PATH, "Making a spell");
		case kMenuType_Alchemy:
			if (bShowAlchemy) strcat_s(detailsStr, MAX_PATH, "Making a potion");
		case kMenuType_Enchantment:
			if (bShowEnchanting) strcat_s(detailsStr, MAX_PATH, "Enchanting");
		case kMenuType_BigFour:
		case kMenuType_Inventory:
		case kMenuType_Stats:
		case kMenuType_Magic:
		case kMenuType_Map:
			if (bShowInventory) strcat_s(detailsStr, MAX_PATH, "Browsing inventory");
			break;
		case kMenuType_Book:
			if (bShowBooks) strcat_s(detailsStr, MAX_PATH, "Reading");
			break;
		case kMenuType_Persuasion:
		case kMenuType_Dialog:
			if (bShowDialog)
			{
				Character* speaker = (Character*)ThisStdCall(0x57AC20, NULL);
				if (speaker) {
					const char* npcName = GetFullName(speaker);
					if (!npcName || !*npcName) {
						strcat_s(detailsStr, MAX_PATH, "Speaking");
					}
					else {
						sprintf_s(detailsStr, MAX_PATH, "Speaking to %s", npcName);
					}
				}
			}
			break;
		case kMenuType_Negotiate:
		case kMenuType_Container:
			if (bShowBarter) {
				ContainerMenu *menu = (ContainerMenu*)GetMenuByType(kMenuType_Container);
				if (menu && menu->isBarter) {
					const char* npcName = GetFullName(menu->refr);
					if (!npcName || !*npcName)
					{
						strcat_s(detailsStr, MAX_PATH, "Trading");
					}
					else
					{
						sprintf_s(detailsStr, MAX_PATH, "Trading with %s", npcName);
					}
				}
			}
			break;
		case kMenuType_Credits:
			if (bShowCredits) strcat_s(detailsStr, MAX_PATH, "Watching credits");
		default:
			break;
		}
	}
	if (bShowLocation) {
		const char*  buff = NULL;
		GetCurrentCellName(&buff);
		if (buff == NULL) buff = "The NULL";
		if (strlen(detailsStr) && strlen(buff)) {
			strcat_s(detailsStr, MAX_PATH, " in ");
		}
		strcat_s(detailsStr, MAX_PATH, buff);
	}
}