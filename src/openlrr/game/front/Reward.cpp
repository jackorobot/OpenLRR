// Reward.cpp : 
//

#include "../../engine/input/Input.h"
#include "../../engine/core/Utils.h"
#include "../../engine/core/Files.h"
#include "../../engine/audio/3DSound.h"
#include "../../engine/Main.h"

#include "../audio/SFX.h"
#include "../interface/Pointers.h"
#include "../interface/ToolTip.h"
#include "../mission/Objective.h"
#include "../mission/NERPsFile.h"
#include "../Game.h"

#include "FrontEnd.h"
#include "Reward.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

 // <LegoRR.exe @00553980>
LegoRR::Reward_Globs& LegoRR::rewardGlobs = *(LegoRR::Reward_Globs*)0x00553980;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 /// CUSTOM: Show tooltips to highlight what a reward counter value is describing.
void LegoRR::Reward_HandleItemToolTip(const Area2F* box, RewardLevelItem* rewardItem)
{
	if (Gods98::msx() >= box->x && Gods98::msx() < box->x + box->width &&
		Gods98::msy() >= box->y && Gods98::msy() < box->y + box->height)
	{
		/// POINTER MATH: Cast to ptrdiff_t before subtraction to avoid C++ pointer math conversion.
		///               RewardLevelItems don't store their type, so grab that from the memory address.
		const Reward_Type itemType = static_cast<Reward_Type>(((ptrdiff_t)rewardItem - (ptrdiff_t)rewardGlobs.level->itemPtr) / sizeof(RewardLevelItem));

		// We don't want to display tooltips for score, since it's labeled right on the clipboard.
		if (itemType != Reward_Score && itemType < Reward_Type_Count) {

			// Hijack one of the non-predefined tooltip types that won't conflict and use that here.
			ToolTip_SetContent(ToolTip_UnitSelect, "%s", rewardItem->Text);
			ToolTip_SetSFX(ToolTip_UnitSelect, SFX_NULL);
			ToolTip_Activate(ToolTip_UnitSelect);
		}
	}
}


// <LegoRR.exe @0045f2f0>
bool32 __cdecl LegoRR::Reward_Initialise(void)
{
	// Constants
	std::array<char const*, 10> constexpr ITEM_NAMES {
		"Crystals",
		"Ore",
		"Diggable",
		"Constructions",
		"Caverns",
		"Figures",
		"RockMonsters",
		"Oxygen",
		"Timer",
		"Score"
	};

	// Initialize globals
	rewardGlobs.base = reinterpret_cast<RewardLevel*>(Gods98::Mem_Alloc(sizeof(RewardLevel)));
	std::memset(rewardGlobs.base, 0, sizeof(RewardLevel));
	rewardGlobs.base->itemPtr = rewardGlobs.base->items;
	rewardGlobs.base->itemCount = std::size(rewardGlobs.base->items);
	for (std::size_t idx = 0; idx < ITEM_NAMES.size(); ++idx)
	{
		RewardLevelItem* item = &rewardGlobs.base->items[idx];
		std::sprintf(item->name, "%s", ITEM_NAMES[idx]);
		Reward_LoadItemImages(item);
		Reward_LoadItemText(item);
		Reward_LoadItemFlics(item);
		Reward_LoadItemFonts(item);
		Reward_LoadItemSounds(item);
		Reward_LoadItemBoxImages(item);
	}

	return TRUE;
}

// <LegoRR.exe @0045f4b0>
void __cdecl LegoRR::Reward_LoadItemSounds(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "Sounds", rewardItem->name);
	rewardItem->SoundName = Gods98::Config_GetStringValue(rewardGlobs.config, id);
}

// <LegoRR.exe @0045f4f0>
void __cdecl LegoRR::Reward_LoadItemFonts(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "Fonts", rewardItem->name);
	char const* const fontName = Gods98::Config_GetStringValue(rewardGlobs.config, id);

	// Load found font name, if it exists
	if (fontName != nullptr)
	{
		Gods98::Font* font = Gods98::Font_Load(fontName);
		if (font != nullptr)
		{
			rewardItem->Font = font;
			return;
		}
	}

	// Font name not found or loaded, using default font
	rewardItem->Font = rewardGlobs.font;
}

// <LegoRR.exe @0045f550>
void __cdecl LegoRR::Reward_LoadItemBoxImages(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "BoxImages", rewardItem->name);
	char* boxImageName = Gods98::Config_GetStringValue(rewardGlobs.config, id);

	// Box image name not found
	if (boxImageName == nullptr)
	{
		// If reduce images flag enabled, try to find the reduce image
		if (!(Gods98::mainGlobs.flags & Gods98::MainFlags::MAIN_FLAG_REDUCEIMAGES))
		{
			char reduceName[64] {};
			std::sprintf(reduceName, "!%s", rewardItem->name);
			char const* const reduceId = Config_ID(rewardGlobs.gameName, "Reward", "Images", reduceName);
			boxImageName = Gods98::Config_GetStringValue(rewardGlobs.config, reduceId);
		}

		// If still no image found, exit this function
		if (boxImageName == nullptr)
		{
			return;
		}
	}

	// Load found box image name, if it exists
	char* stringParts[100] {};
	Gods98::Util_Tokenise(boxImageName, stringParts, "|");
	char valueBuffer[128] {};
	std::sprintf(valueBuffer, "%s", stringParts[0]);
	rewardItem->BoxImagePosition.x = std::atoi(stringParts[1]);
	rewardItem->BoxImagePosition.y = std::atoi(stringParts[2]);
	rewardItem->BoxImage = Gods98::Image_LoadBMPScaled(valueBuffer, 0, 0);
	if (rewardItem->BoxImage != nullptr)
	{
		Gods98::Image_SetupTrans(rewardItem->BoxImage, 0, 0, 0, 0, 0, 0);
		rewardItem->flags |= RewardItemFlags::REWARDITEM_FLAG_BOXIMAGES;
		Gods98::Mem_Free(boxImageName); // Else we leak the memory?
	}

	return;
}

// <LegoRR.exe @0045f6a0>
void __cdecl LegoRR::Reward_LoadItemImages(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "Images", rewardItem->name);
	char* imageName = Gods98::Config_GetStringValue(rewardGlobs.config, id);

	// Image name not found
	if (imageName == nullptr)
	{
		// If reduce images flag enabled, try to find the reduce image
		if (!(Gods98::mainGlobs.flags & Gods98::MainFlags::MAIN_FLAG_REDUCEIMAGES))
		{
			char reduceName[64] {};
			std::sprintf(reduceName, "!%s", rewardItem->name);
			char const* const reduceId = Config_ID(rewardGlobs.gameName, "Reward", "Images", reduceName);
			imageName = Gods98::Config_GetStringValue(rewardGlobs.config, reduceId);
		}

		// If still no image found, exit this function
		if (imageName == nullptr)
		{
			return;
		}
	}

	// Load found image name, if it exists
	char* stringParts[100] {};
	Gods98::Util_Tokenise(imageName, stringParts, "|");
	char valueBuffer[128] {};
	std::sprintf(valueBuffer, "%s", stringParts[0]);
	rewardItem->ImagePosition.x = std::atoi(stringParts[1]);
	rewardItem->ImagePosition.y = std::atoi(stringParts[2]);
	rewardItem->Image = Gods98::Image_LoadBMPScaled(valueBuffer, 0, 0);
	if (rewardItem->Image != nullptr)
	{
		Gods98::Image_SetupTrans(rewardItem->Image, 0, 0, 0, 0, 0, 0);
		rewardItem->flags |= RewardItemFlags::REWARDITEM_FLAG_IMAGES;
		Gods98::Mem_Free(imageName); // Else we leak the memory?
	}

	return;
}

// <LegoRR.exe @0045f7f0>
void __cdecl LegoRR::Reward_LoadItemText(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "Text", rewardItem->name);
	char* const textValue = Gods98::Config_GetStringValue(rewardGlobs.config, id);

	// If text not found, return immediately
	if (textValue == nullptr)
	{
		return;
	}

	char* stringParts[100] {};
	Gods98::Util_Tokenise(textValue, stringParts, "|");
	std::sprintf(rewardItem->Text, "%s", stringParts[0]);
	Front_Util_StringReplaceChar(rewardItem->Text, '_', ' ');
	rewardItem->TextPosition.x = std::atoi(stringParts[1]);
	rewardItem->TextPosition.y = std::atoi(stringParts[2]);
	rewardItem->flags |= RewardItemFlags::REWARDITEM_FLAG_VALUETEXT
		| RewardItemFlags::REWARDITEM_FLAG_NAMETEXT
		| RewardItemFlags::REWARDITEM_FLAG_NAMETEXT_HALFTRANS;
	Gods98::Mem_Free(textValue);
}

// <LegoRR.exe @0045f8b0>
void __cdecl LegoRR::Reward_LoadItemFlics(RewardLevelItem* rewardItem)
{
	char const* const id = Config_ID(rewardGlobs.gameName, "Reward", "Flics", rewardItem->name);
	char* flicName = Gods98::Config_GetStringValue(rewardGlobs.config, id);

	// Flic name not found
	if (flicName == nullptr)
	{
		// If reduce flics flag enabled, try to find the reduce flic
		if (!(Gods98::mainGlobs.flags & Gods98::MainFlags::MAIN_FLAG_REDUCEFLICS))
		{
			char reduceName[64];
			std::sprintf(reduceName, "!%s", rewardItem->name);
			char const* const reduceId = Config_ID(rewardGlobs.gameName, "Reward", "Flics", reduceName);
			flicName = Gods98::Config_GetStringValue(rewardGlobs.config, reduceId);
		}

		// If still no flic found, exit this function
		if (flicName == nullptr)
		{
			return;
		}
	}

	// Load found flic, if it exists
	char* stringParts[100] {};
	Gods98::Util_Tokenise(flicName, stringParts, "|");
	char valueBuffer[128] {};
	std::sprintf(valueBuffer, "%s", stringParts[0]);
	std::sprintf(rewardItem->FlicName, "%s", valueBuffer);
	rewardItem->FlicPosition.x = std::atoi(stringParts[1]);
	rewardItem->FlicPosition.y = std::atoi(stringParts[2]);
	rewardItem->FlicSize.width = std::atoi(stringParts[3]);
	rewardItem->FlicSize.height = std::atoi(stringParts[4]);
	rewardItem->flags |= RewardItemFlags::REWARDITEM_FLAG_FLIC;
	Gods98::Mem_Free(flicName);
}

// <LegoRR.exe @0045fa10>
void __cdecl LegoRR::Reward_Shutdown(void)
{
	for (auto& item : rewardGlobs.base->items)
	{
		if (item.flags & RewardItemFlags::REWARDITEM_FLAG_IMAGES)
		{
			Reward_FreeItemImage(&item);
		}

		if (item.flags & RewardItemFlags::REWARDITEM_FLAG_VALUETEXT)
		{
			Reward_FreeItemFont(&item);
		}
	}

	Gods98::Mem_Free(rewardGlobs.base);
}

// <LegoRR.exe @0045fa70>
void __cdecl LegoRR::Reward_FreeItemImage(RewardLevelItem* rewardItem)
{
	Gods98::Image_Remove(rewardItem->Image);
}

// <LegoRR.exe @0045fa90>
void __cdecl LegoRR::Reward_FreeItemFont(RewardLevelItem* rewardItem)
{
	Gods98::Font_Remove(rewardItem->Font);
}

// <LegoRR.exe @0045fab0>
bool32 __cdecl LegoRR::Reward_LoadGraphics(const Gods98::Config* config, const char* gameName)
{
	// Looks like a strange call, but this is what the disassembly shows
	Lego_GetLevel();

	// Setup globals
	rewardGlobs.config = config;
	rewardGlobs.gameName = gameName;

	// Load wallpaper
	char const* const wallpaperId = Config_ID(gameName, "Reward", "Wallpaper");
	char const* const wallpaperName = Gods98::Config_GetTempStringValue(config, wallpaperId);
	rewardGlobs.wallpaper = Gods98::Image_LoadBMPScaled(wallpaperName, 0, 0);

	// Load configurations
	char const* const displayId = Config_ID(gameName, "Reward", "Display");
	rewardGlobs.display = Gods98::Config_GetBoolValue(config, displayId);
	if (rewardGlobs.display == BOOL3_ERROR)
	{
		rewardGlobs.display = FALSE;
	}

	char const* const centerTextId = Config_ID(gameName, "Reward", "CentreText");
	rewardGlobs.centerText = Gods98::Config_GetBoolValue(config, centerTextId);
	if (rewardGlobs.centerText == BOOL3_ERROR)
	{
		rewardGlobs.centerText = FALSE;
	}

	char const* const scrollSpeedId = Config_ID(gameName, "Reward", "Scrollspeed");
	char const* const scrollSpeedValue = Gods98::Config_GetTempStringValue(config, scrollSpeedId);
	if (scrollSpeedValue != nullptr)
	{
		rewardGlobs.scrollSpeed = static_cast<float>(std::atof(scrollSpeedValue));
	}
	else
	{
		rewardGlobs.scrollSpeed = 0.0f;
	}

	char const* const vertSpacingId = Config_ID(gameName, "Reward", "VertSpacing");
	char const* const vertSpacingValue = Gods98::Config_GetTempStringValue(config, vertSpacingId);
	if (vertSpacingValue != nullptr)
	{
		rewardGlobs.scrollSpeed = static_cast<float>(std::atof(vertSpacingValue));
	}
	else
	{
		rewardGlobs.scrollSpeed = 0.0f;
	}

	// Load fonts
	char const* const fontId = Config_ID(gameName, "Reward", "Font");
	rewardGlobs.fontName = Gods98::Config_GetStringValue(config, fontId);

	char const* const titleFontId = Config_ID(gameName, "Reward", "TitleFont");
	rewardGlobs.titleFontName = Gods98::Config_GetStringValue(config, titleFontId);

	char const* const backFontId = Config_ID(gameName, "Reward", "BackFont");
	rewardGlobs.backFontName = Gods98::Config_GetStringValue(config, backFontId);

	rewardGlobs.font = Gods98::Font_Load(rewardGlobs.fontName);
	rewardGlobs.titleFont = Gods98::Font_Load(rewardGlobs.titleFontName);
	rewardGlobs.backFont = Gods98::Font_Load(rewardGlobs.backFontName);

	Reward_LoadButtons(config, gameName);

	// Load texts
	char const* const completeTextId = Config_ID(gameName, "Reward", "CompleteText");
	rewardGlobs.completeText = Gods98::Config_GetStringValue(config, completeTextId);

	char const* const failedTextId = Config_ID(gameName, "Reward", "FailedText");
	rewardGlobs.failedText = Gods98::Config_GetStringValue(config, failedTextId);

	char const* const quitTextId = Config_ID(gameName, "Reward", "QuitText");
	rewardGlobs.quitText = Gods98::Config_GetStringValue(config, quitTextId);

	char const* const textPositionId = Config_ID(gameName, "Reward", "TextPos");
	char* const textPositionValue = Gods98::Config_GetStringValue(config, textPositionId);
	if (textPositionValue != nullptr)
	{
		char* stringParts[100] {};
		Gods98::Util_Tokenise(textPositionValue, stringParts, "|");
		rewardGlobs.textPos.x = std::atoi(stringParts[0]);
		rewardGlobs.textPos.y = std::atoi(stringParts[1]);
		// textPositionValue leaks here
	}

	return TRUE;
}

// <LegoRR.exe @0045fdb0>
bool32 __cdecl LegoRR::Reward_LoadButtons(const Gods98::Config* config, const char* gameName)
{
	// Load SaveButton
	char const* const saveButtonId = Config_ID(gameName, "Reward", "SaveButton");
	char* const saveButtonName = Gods98::Config_GetStringValue(config, saveButtonId);
	if (saveButtonName != nullptr)
	{
		char* stringParts[100] {};
		Gods98::Util_Tokenise(saveButtonName, stringParts, "|");
		rewardGlobs.saveButton = Gods98::Image_LoadBMPScaled(stringParts[0], 0, 0);
		if (rewardGlobs.saveButton != nullptr)
		{
			rewardGlobs.saveButton_hi = Gods98::Image_LoadBMPScaled(stringParts[1], 0, 0);
			rewardGlobs.saveButton_in = Gods98::Image_LoadBMPScaled(stringParts[2], 0, 0);
			rewardGlobs.saveButton_dim = Gods98::Image_LoadBMPScaled(stringParts[3], 0, 0);

			rewardGlobs.saveButtonPosition.x = static_cast<float>(std::atoi(stringParts[4]));
			rewardGlobs.saveButtonPosition.y = static_cast<float>(std::atoi(stringParts[5]));

			Gods98::Image_SetupTrans(rewardGlobs.saveButton, 0, 0, 0, 0, 0, 0);
			if (rewardGlobs.saveButton_hi != nullptr)
			{
				Gods98::Image_SetupTrans(rewardGlobs.saveButton_hi, 0, 0, 0, 0, 0, 0);
			}
			if (rewardGlobs.saveButton_in != nullptr)
			{
				Gods98::Image_SetupTrans(rewardGlobs.saveButton_in, 0, 0, 0, 0, 0, 0);
			}
		}

		Gods98::Mem_Free(saveButtonName);
	}

	// Load AdvanceButton
	char const* const advanceButtonId = Config_ID(gameName, "Reward", "AdvanceButton");
	char* const advanceButtonName = Gods98::Config_GetStringValue(config, advanceButtonId);
	if (advanceButtonName != nullptr)
	{
		char* stringParts[100] {};
		Gods98::Util_Tokenise(advanceButtonName, stringParts, "|");
		rewardGlobs.advanceButton = Gods98::Image_LoadBMPScaled(stringParts[0], 0, 0);
		if (rewardGlobs.advanceButton != nullptr)
		{
			rewardGlobs.advanceButton_hi = Gods98::Image_LoadBMPScaled(stringParts[1], 0, 0);
			rewardGlobs.advanceButton_in = Gods98::Image_LoadBMPScaled(stringParts[2], 0, 0);
			rewardGlobs.advanceButton_dim = Gods98::Image_LoadBMPScaled(stringParts[3], 0, 0);

			rewardGlobs.advanceButtonPosition.x = static_cast<float>(std::atoi(stringParts[4]));
			rewardGlobs.advanceButtonPosition.y = static_cast<float>(std::atoi(stringParts[5]));

			Gods98::Image_SetupTrans(rewardGlobs.advanceButton, 0, 0, 0, 0, 0, 0);
			if (rewardGlobs.advanceButton_hi != nullptr)
			{
				Gods98::Image_SetupTrans(rewardGlobs.advanceButton_hi, 0, 0, 0, 0, 0, 0);
			}
			if (rewardGlobs.advanceButton_in != nullptr)
			{
				Gods98::Image_SetupTrans(rewardGlobs.advanceButton_in, 0, 0, 0, 0, 0, 0);
			}
		}

		Gods98::Mem_Free(advanceButtonName);
	}

	return rewardGlobs.saveButton != nullptr && rewardGlobs.advanceButton != nullptr;
}

// <LegoRR.exe @00460060>
bool32 __cdecl LegoRR::Reward_CreateLevel(void)
{
	// Setup globals
	Lego_Level const& level = *Lego_GetLevel();
	rewardGlobs.level = reinterpret_cast<RewardLevel*>(Gods98::Mem_Alloc(sizeof(RewardLevel)));
	std::memset(rewardGlobs.level, 0, sizeof(RewardLevel));

	// Load enable
	char const* const enableId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "Enable");
	rewardGlobs.level->Enabled = Gods98::Config_GetBoolValue(rewardGlobs.config, enableId);
	if (rewardGlobs.level->Enabled == BOOL3_ERROR)
	{
		rewardGlobs.level->Enabled = FALSE;
	}

	// Load modifier
	char const* const modifierId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "Modifier");
	char const* modifierValue = Gods98::Config_GetTempStringValue(rewardGlobs.config, modifierId);
	if (modifierValue == nullptr)
	{
		modifierValue = "";
	}
	rewardGlobs.level->Modifier = std::atoi(modifierValue);

	// Load timer
	char const* const timerId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "Timer");
	char const* timerValue = Gods98::Config_GetTempStringValue(rewardGlobs.config, timerId);
	if (timerValue == nullptr)
	{
		timerValue = "";
	}
	rewardGlobs.timer = static_cast<float>(std::atof(timerValue));

	// Load display text
	char const* const displayTextId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "DisplayText");
	rewardGlobs.displayText = Gods98::Config_GetBoolValue(rewardGlobs.config, displayTextId);
	if (rewardGlobs.displayText == BOOL3_ERROR)
	{
		rewardGlobs.displayText = TRUE;
	}

	// Load display images
	char const* const displayImagesId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "DisplayImages");
	rewardGlobs.displayImages = Gods98::Config_GetBoolValue(rewardGlobs.config, displayImagesId);
	if (rewardGlobs.displayImages == BOOL3_ERROR)
	{
		rewardGlobs.displayImages = TRUE;
	}

	// Load display flics
	char const* const displayFlicsId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "DisplayFlics");
	rewardGlobs.displayFlics = Gods98::Config_GetBoolValue(rewardGlobs.config, displayFlicsId);
	if (rewardGlobs.displayFlics == BOOL3_ERROR)
	{
		rewardGlobs.displayFlics = TRUE;
	}

	// Setup items
	rewardGlobs.level->itemPtr = rewardGlobs.level->items;
	rewardGlobs.level->itemCount = std::size(rewardGlobs.level->items);
	std::copy(std::begin(rewardGlobs.base->items), std::end(rewardGlobs.base->items), std::begin(rewardGlobs.level->items));
	for (auto& item : rewardGlobs.level->items)
	{
		Reward_LoadLevelItemImportance(&item);
		Reward_LoadLevelItemQuota(&item);
	}

	// Set hardcoded Caverns::Quota = 1, for Level03 ("Rubble Trouble!")
	if (_stricmp(level.name, "Levels::Level03") == 0)
	{
		rewardGlobs.level->items[4].Quota = 1;
	}

	return TRUE;
}

// <LegoRR.exe @00460360>
void __cdecl LegoRR::Reward_LoadLevelItemImportance(RewardLevelItem* rewardItem)
{
	Lego_Level const& level = *Lego_GetLevel();
	char const* const importanceId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "Importance", rewardItem->name);
	char const* importanceValue = Gods98::Config_GetTempStringValue(rewardGlobs.config, importanceId);
	if (importanceValue == nullptr)
	{
		importanceValue = "";
	}
	rewardItem->Importance = static_cast<float>(std::atof(importanceValue));
}

// <LegoRR.exe @00460400>
void __cdecl LegoRR::Reward_LoadLevelItemQuota(RewardLevelItem* rewardItem)
{
	Lego_Level const& level = *Lego_GetLevel();
	char const* const quotaId = Config_ID(rewardGlobs.gameName, level.name, "Reward", "Quota", rewardItem->name);
	char const* quotaValue = Gods98::Config_GetTempStringValue(rewardGlobs.config, quotaId);
	if (quotaValue == nullptr)
	{
		quotaValue = "";
	}
	rewardItem->Quota = std::atoi(quotaValue);
}

// <LegoRR.exe @004604a0>
void __cdecl LegoRR::Reward_FreeLevel(void)
{
	Gods98::Mem_Free(rewardGlobs.level);
}

// <LegoRR.exe @004604b0>
LegoRR::RewardLevel* __cdecl LegoRR::GetRewardLevel2(undefined4 unused_rewardID)
{
	return rewardGlobs.level;
}

// <LegoRR.exe @004604c0>
LegoRR::RewardLevel* __cdecl LegoRR::GetRewardLevel(void)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return nullptr;
	}

	return GetRewardLevel2(level->unused_rewardID);
}

// <LegoRR.exe @004604e0>
void __cdecl LegoRR::RewardQuota_CountUnits(void)
{
	// Get Level
	Lego_Level const* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	// Get Rewardlevel
	RewardLevel* rewardLevel = GetRewardLevel2(level->unused_rewardID);
	if (rewardLevel == nullptr)
	{
		return;
	}

	// Get building counts
	RewardBuildingCounts buildingCounts {};
	RewardQuota_CountBuildings(&buildingCounts);
	rewardGlobs.current.buildingCounts = buildingCounts;

	// Get reward type figures
	rewardGlobs.current.items[Reward_Type::Reward_Figures].numDestroyed = NERPsRuntime_GetPreviousLevelObjectsBuilt("Pilot", 0);
}

// <LegoRR.exe @00460550>
LegoRR::RewardBuildingCounts* __cdecl LegoRR::RewardQuota_CountBuildings(OUT RewardBuildingCounts* out_buildingCounts)
{
	RewardBuildingCounts buildingCounts {};
	LegoObject_RunThroughListsSkipUpgradeParts(RewardQuota_LiveObjectCallback_CountBuildings, &buildingCounts);

	for (sint32 objId = 0; objId < LegoObject_ID::LegoObject_ID_Count; objId += 1)
	{
		for (sint32 objLevel = 0; objLevel < 16; ++objLevel)
		{
			uint32 built = LegoObject_GetPreviousLevelObjectsBuilt(LegoObject_Type::LegoObject_Building, static_cast<LegoObject_ID>(objId), objLevel);
			buildingCounts.numPrevLevels_unk += built;
		}
	}

	*out_buildingCounts = buildingCounts;
	return out_buildingCounts;
}

// <LegoRR.exe @004605d0>
bool32 __cdecl LegoRR::RewardQuota_LiveObjectCallback_CountBuildings(LegoObject* liveObj, void* search)
{
	LegoObject_Type type {};
	LegoObject_ID id {};
	LegoObject_GetTypeAndID(liveObj, &type, &id);

	RewardBuildingCounts& buildingCounts = *reinterpret_cast<RewardBuildingCounts*>(search);
	if (type == LegoObject_Type::LegoObject_Building)
	{
		buildingCounts.nameTable[buildingCounts.count] = Object_GetTypeName(LegoObject_Type::LegoObject_Building, id);
		++buildingCounts.count;
	}

	return FALSE;
}

// <LegoRR.exe @00460620>
//bool32 __cdecl LegoRR::Reward_Prepare(void);

// <LegoRR.exe @00460bd0>
uint32 __cdecl LegoRR::Reward_GetLevelObjectsBuilt(const char* objName, sint32 objLevel, bool32 currentLevel)
{
	LegoObject_Type objType {};
	LegoObject_ID objId {};

	if (Lego_GetObjectByName(objName, &objType, &objId, nullptr))
	{
		return LegoObject_GetLevelObjectsBuilt(objType, objId, objLevel, currentLevel);
	}

	return 0;
}

inline void calculatePercent(LegoRR::RewardLevelItem& item)
{
	if (item.countdownRatio != 0.0f)
	{
		item.percentFloat = 100.0f;
	}
	else
	{
		item.percentFloat = std::min(100.0f, (100.0f / item.countdownRatio) * item.countdown);
	}
}

inline void addToScore(LegoRR::RewardLevelItem const& item, LegoRR::RewardLevelItem& score)
{
	for (int importance = 0; importance < item.Importance; ++importance)
	{
		score.countdownRatio += 100.0;
		score.countdown += item.percentFloat;
	}
}

// <LegoRR.exe @00460c10>
bool32 __cdecl LegoRR::Reward_PrepareCalculate(void)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return FALSE;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return FALSE;
	}

	// Reset score
	reward->items[Reward_Type::Reward_Score].countdown = 0.0f;
	reward->items[Reward_Type::Reward_Score].countdownRatio = 0.0f;
	reward->items[Reward_Type::Reward_Score].percentFloat = 0.0f;

	calculatePercent(reward->items[Reward_Type::Reward_Crystals]);
	calculatePercent(reward->items[Reward_Type::Reward_Ore]);
	calculatePercent(reward->items[Reward_Type::Reward_Diggable]);
	calculatePercent(reward->levelItem);
	calculatePercent(reward->items[Reward_Type::Reward_Oxygen]);
	calculatePercent(reward->items[Reward_Type::Reward_Constructions]);
	calculatePercent(reward->items[Reward_Type::Reward_Figures]);
	calculatePercent(reward->items[Reward_Type::Reward_RockMonsters]);

	RewardLevelItem& score = reward->items[Reward_Type::Reward_Score];
	addToScore(reward->items[Reward_Type::Reward_Figures], score);
	addToScore(reward->items[Reward_Type::Reward_Crystals], score);
	addToScore(reward->items[Reward_Type::Reward_Oxygen], score);
	addToScore(reward->items[Reward_Type::Reward_Constructions], score);
	addToScore(reward->items[Reward_Type::Reward_Caverns], score);
	addToScore(reward->items[Reward_Type::Reward_Timer], score);
	addToScore(reward->items[Reward_Type::Reward_RockMonsters], score);

	if (score.countdownRatio < score.countdown)
	{
		score.countdown = score.countdownRatio;
	}

	if (score.countdown < 0)
	{
		score.countdown = 0.0;
	}

	calculatePercent(score);
	score.percentFloat += reward->Modifier;

	if (score.percentFloat > 100.0f)
	{
		score.percentFloat = 100.0f;
	}

	return TRUE;
}

// <LegoRR.exe @004611c0>
bool32 __cdecl LegoRR::Reward_PrepareScroll(void)
{
	float const heightf = static_cast<float>(Gods98::mainGlobs.appHeight);
	float const widthf = static_cast<float>(Gods98::mainGlobs.appWidth);

	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return FALSE;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return FALSE;
	}

	Reward_PrepareValueText();
	RewardScroll_Create(&rewardGlobs.scroll, 0, heightf * 0.05f, widthf,
						heightf - (heightf / 3.5f), rewardGlobs.scrollSpeed);
	RewardScroll_SetDelay_Unk(rewardGlobs.scroll, 400.0f);
	// This has no effect (flags |= 0;)
	RewardScroll_AddFlags(rewardGlobs.scroll, RewardScrollFlags::REWARDSCROLL_NONE);
	// Add label to display level name at top of screen
	RewardScroll_AddLabel(rewardGlobs.scroll, level->FullName, rewardGlobs.titleFont, 0.0f, heightf * 0.04f,
						  RewardScrollLabelFlags::REWARDSCROLL_LABEL_CENTERED | RewardScrollLabelFlags::REWARDSCROLL_LABEL_NOSCROLL);
	char const* text;
	if ((text = rewardGlobs.completeText, level->status == LevelStatus::LEVELSTATUS_COMPLETE)
		|| (text = rewardGlobs.failedText, level->status == LevelStatus::LEVELSTATUS_FAILED)
		|| (text = rewardGlobs.quitText, level->status == LevelStatus::LEVELSTATUS_FAILED_CRYSTALS))
	{
		text = Gods98::Util_RemoveUnderscores(text);
		std::sprintf(rewardGlobs.statusMessage, "%s", text);
	}

	// Add label to display completion message just below level name
	RewardScroll_AddLabel(rewardGlobs.scroll, rewardGlobs.statusMessage, rewardGlobs.font, 0.0, rewardGlobs.vertSpacing,
						  RewardScrollLabelFlags::REWARDSCROLL_LABEL_CENTERED);
	return TRUE;
}

inline void Convert(LegoRR::RewardLevelItem& item)
{
	item.percentInt = static_cast<sint32>(item.percentFloat);
}

inline void Print(LegoRR::RewardLevelItem& item, char const* const format)
{
	std::sprintf(item.valueText, format, item.percentInt);
}

inline void ConvertAndPrintPercentage(LegoRR::RewardLevelItem& item)
{
	Convert(item);
	Print(item, "%d%%%%");
}

inline void ConvertAndPrint(LegoRR::RewardLevelItem& item)
{
	Convert(item);
	Print(item, "%d");
}

// <LegoRR.exe @00461330>
bool32 __cdecl LegoRR::Reward_PrepareValueText(void)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return FALSE;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return FALSE;
	}

	// Convert and print most reward types
	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Crystals]);
	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Ore]);
	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Diggable]);
	ConvertAndPrint(reward->levelItem);

	reward->items[Reward_Type::Reward_Constructions].percentInt = reward->uintConstructCount_1cc;
	Print(reward->items[Reward_Type::Reward_Constructions], "%d");

	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Caverns]);
	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Figures]);

	auto& rockMonsters = reward->items[Reward_Type::Reward_RockMonsters];
	rockMonsters.percentInt = static_cast<sint32>(std::min(rockMonsters.damageTaken, 100.0f));
	Print(rockMonsters, "%d%%%%");

	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Oxygen]);

	// Convert and create timer text
	auto& timer = reward->items[Reward_Type::Reward_Timer];
	sint32 const total_seconds = static_cast<sint32>(timer.countdown * timer.countdownRatio * 0.04f);
	timer.percentInt = total_seconds;
	sint32 seconds = total_seconds % 359999;
	sint32 const hours = (seconds - 1) / 3600;
	seconds -= hours * 3600;
	sint32 const minutes = (seconds - 1) / 60;
	seconds -= minutes * 60;
	std::sprintf(timer.valueText, "%02d:%02d.%02d", hours, minutes, seconds);

	// Finally convert the 
	ConvertAndPrintPercentage(reward->items[Reward_Type::Reward_Score]);

	return TRUE;
}

// <LegoRR.exe @004616d0>
bool32 __cdecl LegoRR::Reward_Show(void)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return FALSE;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return FALSE;
	}

	if (!Reward_LoopBegin())
	{
		return FALSE;
	}

	RewardUIState currentState = RewardUIState::REWARDSTATE_0;
	float lastTime = static_cast<float>(Gods98::Main_GetTime());
	float timer = rewardGlobs.timer;
	Reward_Type rewardType = Reward_Type::Reward_Crystals;
	bool32 finished = false;
	bool32 saved = false;
	while (currentState < RewardUIState::REWARDSTATE__COUNT || Gods98::mslb() || Gods98::Input_LClicked())
	{
		// Do time calculations
		float const currentTime = static_cast<float>(Gods98::Main_GetTime());
		float const timeDelta = (currentTime - lastTime) * 0.001f;
		lastTime = currentTime;
		timer += timeDelta;

		Reward_UpdateState(&rewardType, &currentState, &timer);

		if (Input_IsKeyReleased(DIK_SPACE) || Gods98::Input_LClicked() && currentState < RewardUIState::REWARDSTATE_3)
		{
			currentState = static_cast<RewardUIState>(currentState + 1);
			if (currentState == RewardUIState::REWARDSTATE_3)
			{
				currentState = RewardUIState::REWARDSTATE_2;
			}

			if (currentState < RewardUIState::REWARDSTATE_2 || level->status != LevelStatus::LEVELSTATUS_COMPLETE)
			{
				rewardType = Reward_Type::Reward_Score;
			}
			else
			{
				rewardType = Reward_Type::Reward_Type_Count;
			}
		}

		if (rewardGlobs.wallpaper != nullptr)
		{
			Gods98::Image_DisplayScaled(rewardGlobs.wallpaper, nullptr, nullptr, nullptr);
		}

		for (int idx = 0; idx < rewardType; ++idx)
		{
			Reward_DrawItem(&reward->items[idx], RewardItemFlags::REWARDITEM_FLAG_IMAGES, rewardType);
		}

		for (int idx = 0; idx < std::min(rewardType, Reward_Type::Reward_Score); ++idx)
		{
			Reward_DrawItem(&reward->items[idx], RewardItemFlags::REWARDITEM_FLAG_BOXIMAGES, rewardType);
		}

		for (int idx = 0; idx < rewardType; ++idx)
		{
			Reward_DrawItem(&reward->items[idx], RewardItemFlags::REWARDITEM_FLAG_FLIC, rewardType);
		}

		Reward_DrawAllValues(&rewardType, &finished);

		if (finished)
		{
			Reward_DrawScore(rewardType);

			if (currentState < RewardUIState::REWARDSTATE_3)
			{
				Reward_HandleButtons(&currentState, &saved);
			}
		}

		RewardScroll_DrawLabels(rewardGlobs.scroll);

		if (currentState == RewardUIState::REWARDSTATE_3)
		{
			currentState = RewardUIState::REWARDSTATE__COUNT;
		}

		Reward_HandleDebugKeys(&rewardType, &currentState, &finished, &timer);
		Reward_LoopUpdate(timeDelta);
	}

	RewardScroll_Free(&rewardGlobs.scroll);

	for (int idx = 0; idx < Reward_Type::Reward_Type_Count; ++idx)
	{
		if (reward->items[idx].flags & RewardItemFlags::REWARDITEM_FLAG_FLIC)
		{
			Gods98::Flic_Close(reward->items[idx].Flic);
			reward->items[idx].Flic = nullptr;
		}
	}

	Gods98::Sound3D_Stream_Stop(TRUE);

	if (level->status == LevelStatus::LEVELSTATUS_COMPLETE && legoGlobs.EndGameAVI1 != nullptr)
	{
		char const* endGameMovie = legoGlobs.EndGameAVI1;
		if (Front_Save_IsGame100Percented() && legoGlobs.EndGameAVI2 != nullptr)
		{
			endGameMovie = legoGlobs.EndGameAVI2;
		}

		if (Gods98::File_Exists(endGameMovie))
		{
			Gods98::Movie_t* movie = Gods98::Movie_Load(endGameMovie);
			Front_PlayMovie(movie, FALSE);
			Gods98::Movie_Free(movie);
		}

		SFX_AddToQueue(SFX_ID::SFX_AmbientMusicLoop, Gods98::SoundMode::Loop);
		Front_Callback_TriggerPlayCredits();
		Gods98::Sound3D_Stream_Stop(TRUE);
	}

	return TRUE;
}

// <LegoRR.exe @00461a50>
void __cdecl LegoRR::Reward_DrawItem(RewardLevelItem* rewardItem, RewardItemFlags flags, Reward_Type rewardType)
{
#if false
	// Test NAMETEXT boxes:
	// The chief animation includes sections of the BoxImages due to drawing under them,
	//  So it's not a bug that they're partially visible at the very end.
	flags &= ~REWARDITEM_FLAG_BOXIMAGES;

	if (flags & REWARDITEM_FLAG_VALUETEXT) {
		/// POINTER MATH: rewardItems don't store their type, so grab that from the memory address.
		const Reward_Type itemType = static_cast<Reward_Type>(((ptrdiff_t)rewardItem - (ptrdiff_t)rewardGlobs.level->itemPtr) / sizeof(RewardLevelItem));
		if (itemType != Reward_Score && itemType < Reward_Type_Count) {
			flags &= ~REWARDITEM_FLAG_VALUETEXT;
			flags |= REWARDITEM_FLAG_NAMETEXT_HALFTRANS;
		}
	}
#endif

	/// NOTE: Throughout this function, we check if both rewardItem->flags and parameter flags have the same values.
	///       So we're using a variable that's a bitwise AND of both flags to simplify the comparisons.

	const RewardItemFlags bothFlags = (rewardItem->flags & flags);


	// 1-INDEXED!!!????
	uint32 rewardIdx;
	if (rewardType == 0)
		rewardIdx = 0;
	else
		rewardIdx = rewardType - 1;

	if (rewardGlobs.displayImages) {
		if (bothFlags & REWARDITEM_FLAG_IMAGES) {
			const Point2F destPos = {
				static_cast<real32>(rewardItem->ImagePosition.x),
				static_cast<real32>(rewardItem->ImagePosition.y),
			};
			Gods98::Image_Display(rewardItem->Image, &destPos);
		}

		if (bothFlags & REWARDITEM_FLAG_BOXIMAGES) {
			const Point2F destPos = {
				static_cast<real32>(rewardItem->BoxImagePosition.x),
				static_cast<real32>(rewardItem->BoxImagePosition.y),
			};
			Gods98::Image_Display(rewardItem->BoxImage, &destPos);

			/// CUSTOM: Show tooltips for what reward value the mouse is hovering over.
			const Area2F destArea = {
				destPos.x,
				destPos.y,
				static_cast<real32>(Gods98::Image_GetWidth(rewardItem->BoxImage)),
				static_cast<real32>(Gods98::Image_GetHeight(rewardItem->BoxImage)),
			};
			Reward_HandleItemToolTip(&destArea, rewardItem);
		}
	}

	if (rewardGlobs.displayFlics && (bothFlags & REWARDITEM_FLAG_FLIC)) {
		// Lazy loading for Flics.
		if (rewardItem->Flic == nullptr) {
			Gods98::Flic_Setup(rewardItem->FlicName, &rewardItem->Flic, Gods98::FlicUserFlags::FLICDISK);
		}

		const Area2F destArea = {
			static_cast<real32>(rewardItem->FlicPosition.x),
			static_cast<real32>(rewardItem->FlicPosition.y),
			static_cast<real32>(rewardItem->FlicSize.width),
			static_cast<real32>(rewardItem->FlicSize.height),
		};

		/// ALTERATION: Draw animation at 1.5x speed, because it looks terribly slow in modern day.
		const bool advance = (rewardItem->Flic->currentframe < 35);
		Gods98::Flic_AnimateDeltaTime(rewardItem->Flic, &destArea, advance, false,
									  Gods98::Main_GetDeltaTime() * (37.5f / STANDARD_FRAMERATE));
	}

	if (rewardGlobs.displayText) {
		if (bothFlags & REWARDITEM_FLAG_VALUETEXT) {

			Gods98::Font_PrintFCenter(rewardItem->Font, rewardItem->TextPosition.x, rewardItem->TextPosition.y, rewardItem->valueText);
			if (rewardType < Reward_Type_Count) {
				const char* text = rewardGlobs.level->itemPtr[rewardIdx].Text;

				Gods98::Font_PrintFCenter(rewardGlobs.titleFont, rewardGlobs.textPos.x, rewardGlobs.textPos.y, text);
			}
		}

		if ((bothFlags & REWARDITEM_FLAG_NAMETEXT) ||
			(bothFlags & REWARDITEM_FLAG_NAMETEXT_HALFTRANS))
		{
			const ColourRGBF rc = {
				(30.0f / 255.0f),
				(30.0f / 255.0f),
				(30.0f / 255.0f),
			};
			const ColourRGBF ln1 = {
				(60.0f / 255.0f),
				(60.0f / 255.0f),
				(60.0f / 255.0f),
			};
			const ColourRGBF ln2 = {
				0.0f,
				0.0f,
				0.0f,
			};

			{
				const uint32 width = Gods98::Font_GetStringWidth(rewardItem->Font, rewardItem->valueText);
				const uint32 boxWidth = width + 20;

				const Area2F destArea = {
					static_cast<real32>(rewardItem->TextPosition.x - (boxWidth/2)),
					static_cast<real32>(rewardItem->TextPosition.y),
					static_cast<real32>(boxWidth),
					static_cast<real32>(Gods98::Font_GetHeight(rewardItem->Font)),
				};

				const bool halfTrans = (flags & REWARDITEM_FLAG_NAMETEXT_HALFTRANS);

				ToolTip_DrawBox(destArea, rc.r, rc.g, rc.b, ln1.r, ln1.g, ln1.b,
								ln2.r, ln2.g, ln2.b, halfTrans);

				Gods98::Font_PrintF(rewardItem->Font, rewardItem->TextPosition.x - (width/2), rewardItem->TextPosition.y, rewardItem->valueText);

				/// CUSTOM: Show tooltips for what reward value the mouse is hovering over.
				Reward_HandleItemToolTip(&destArea, rewardItem);
			}

			if (rewardType < Reward_Type_Count) {
				const char* text = rewardGlobs.level->itemPtr[rewardIdx].Text;

				const uint32 width = Gods98::Font_GetStringWidth(rewardGlobs.titleFont, text);
				const uint32 boxWidth = width + 20;

				const Area2F destArea = {
					static_cast<real32>(rewardGlobs.textPos.x - (boxWidth/2)),
					static_cast<real32>(rewardGlobs.textPos.y),
					static_cast<real32>(boxWidth),
					static_cast<real32>(Gods98::Font_GetHeight(rewardGlobs.titleFont)),
				};

				ToolTip_DrawBox(destArea, rc.r, rc.g, rc.b, ln1.r, ln1.g, ln1.b,
								ln2.r, ln2.g, ln2.b, false);

				Gods98::Font_PrintF(rewardGlobs.titleFont, rewardGlobs.textPos.x - (width/2), rewardGlobs.textPos.y, text);
			}
		}
	}
}

// <LegoRR.exe @00461f50>
void __cdecl LegoRR::Reward_DrawScore(Reward_Type rewardType)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return;
	}

	if (level->status != LevelStatus::LEVELSTATUS_COMPLETE)
	{
		return;
	}

	auto& score = reward->items[Reward_Type::Reward_Score];
	Reward_DrawItem(&score, RewardItemFlags::REWARDITEM_FLAG_VALUETEXT, rewardType);

	char buffer1[32] {};
	char buffer2[32] {};
	std::sprintf(buffer2, "%s", score.Text);
	std::size_t length = std::strlen(score.Text);
	for (std::size_t idx = 0; idx < length; ++idx)
	{
		if (score.Text[idx] == ' ')
		{
			buffer2[idx] = '\0';
			std::sprintf(buffer1, "%s", &buffer2[idx + 1]);
		}
	}

	uint32 const width = Gods98::Font_GetStringWidth(rewardGlobs.backFont, buffer2);
	Gods98::Font_PrintF(rewardGlobs.backFont, score.TextPosition.x - (width / 2), score.TextPosition.y - 40, buffer2);
}

// <LegoRR.exe @00462090>
void __cdecl LegoRR::Reward_HandleButtons(OUT RewardUIState* state, OUT bool32* saved)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return;
	}

	bool buttonPressed = false;

	if (level->status == LevelStatus::LEVELSTATUS_COMPLETE)
	{
		if (rewardGlobs.saveButtonPosition.x < Gods98::msx()
			&& Gods98::msx() < (rewardGlobs.saveButton->width + rewardGlobs.saveButtonPosition.x)
			&& rewardGlobs.saveButtonPosition.y < Gods98::msy()
			&&Gods98::msy() < (rewardGlobs.saveButton->height + rewardGlobs.saveButtonPosition.y)
			)
		{
			ToolTip_Activate(ToolTip_Type::ToolTip_Reward_Save);

			if (Gods98::mslb() && rewardGlobs.saveButton_in != nullptr)
			{
				Gods98::Image_DisplayScaled(rewardGlobs.saveButton_in, nullptr, &rewardGlobs.saveButtonPosition, nullptr);
			}
			else if (rewardGlobs.saveButton_hi != nullptr)
			{
				Gods98::Image_DisplayScaled(rewardGlobs.saveButton_hi, nullptr, &rewardGlobs.saveButtonPosition, nullptr);
			}

			if (Lego_UpdateGameCtrlLeftButtonLast())
			{
				buttonPressed = true;
				SFX_Random_PlaySoundNormal(SFX_ID::SFX_Okay, FALSE);
				Reward_GotoSaveMenu();
				*saved = TRUE;
				SFX_Sound3D_StopSound(SFX_ID::SFX_MusicLoop);
				SFX_AddToQueue(SFX_ID::SFX_MusicLoop, Gods98::SoundMode::Loop);
			}
		}
		else
		{
			Gods98::Image_DisplayScaled(rewardGlobs.saveButton, nullptr, &rewardGlobs.saveButtonPosition, nullptr);
		}
	}
	else
	{
		if (rewardGlobs.saveButton_dim != nullptr)
		{
			Gods98::Image_DisplayScaled(rewardGlobs.saveButton_dim, nullptr, &rewardGlobs.saveButtonPosition, nullptr);
		}
	}

	if (Gods98::msx() <= rewardGlobs.advanceButtonPosition.x
		|| (rewardGlobs.advanceButton->width + rewardGlobs.advanceButtonPosition.x) <= Gods98::msx()
		|| Gods98::msy() <= rewardGlobs.advanceButtonPosition.y
		|| (rewardGlobs.advanceButton->height + rewardGlobs.advanceButtonPosition.y) <= Gods98::msy())
	{
		Gods98::Image_DisplayScaled(rewardGlobs.advanceButton, nullptr, &rewardGlobs.advanceButtonPosition, nullptr);
	}
	else
	{
		ToolTip_Activate(ToolTip_Type::ToolTip_Reward_Advance);

		if (Gods98::mslb() != 0 && rewardGlobs.advanceButton_in != nullptr)
		{
			Gods98::Image_DisplayScaled(rewardGlobs.advanceButton_in, nullptr, &rewardGlobs.advanceButtonPosition, nullptr);
		}
		else if (rewardGlobs.advanceButton_hi != nullptr)
		{
			Gods98::Image_DisplayScaled(rewardGlobs.advanceButton_hi, nullptr, &rewardGlobs.advanceButtonPosition, nullptr);
		}

		if (Lego_UpdateGameCtrlLeftButtonLast())
		{
			buttonPressed = true;
			*state = RewardUIState::REWARDSTATE_3;
			SFX_Random_PlaySoundNormal(SFX_ID::SFX_Okay, FALSE);
			Reward_GotoAdvance();
		}
	}

	if (!buttonPressed && Lego_UpdateGameCtrlLeftButtonLast())
	{
		SFX_Random_PlaySoundNormal(SFX_ID::SFX_Okay, FALSE);
	}
}

// <LegoRR.exe @004622f0>
//#define Reward_GotoSaveMenu ((void (__cdecl* )(void))0x004622f0)
void __cdecl LegoRR::Reward_GotoSaveMenu(void)
{
	/// REMOVE: Function call with no effect.
	//Front_IsMissionSelected();

	Front_SaveOptionParameters();

	if (Front_IsFrontEndEnabled() && Objective_IsObjectiveAchieved() && !Front_IsTutorialSelected()) {
		const sint32 levelIndex = Front_LevelSet_IndexOf(Front_Levels_GetTutoOrMissions(), Lego_GetLevel()->name);
		/// REMOVE: Function call with no effect.
		//Front_Save_GetRewardLevel(levelIndex);

		const sint32 lastSaveNumber = Front_Save_GetSaveNumber();
		SaveData* lastSave = Front_Save_GetCurrentSaveData();
		SaveStruct_18 saveStruct18;
		SaveData copySave;
		// This allocates missionsTable, so we need to either call Front_Save_SetSaveData or Front_Save_FreeSaveData (but not both).
		Front_Save_CopySaveData(&copySave);
		RewardLevel* rewardLevel = GetRewardLevel();

		do {
			Front_RunScreenMenuType(Menu_Screen_Save);

		} while (frontGlobs.saveMenuKeepOpen);

		// True when leaving the save menu without using the back button.
		if (frontGlobs.saveMenuHasSaved == TRUE) {
			// This boolean is only ever true when running Menu_Screen_Save, and bool_85c was true.
			// Solve bool_85c to solve bool_540.
			if (Front_Save_GetHasNoSaveData()) {
				Front_Save_WriteEmptySaveFiles();
				Front_Save_SetHasNoSaveData(false);
			}

			SaveData* currSave = Front_Save_GetCurrentSaveData();
			if (currSave != nullptr) {
				if (currSave != lastSave && lastSave != nullptr) {
					Front_Save_SetSaveData(&copySave);
				}
				else {
					/// FIX APPLY: Free SaveData when Front_Save_SetSaveData is not called.
					Front_Save_FreeSaveData(&copySave);
				}

				Front_Save_SetLevelCompleted(levelIndex);
				Front_Save_SetRewardLevel(levelIndex, rewardLevel);

				Object_Save_CopyStruct18(&saveStruct18);
				Front_Save_SetSaveStruct18(&saveStruct18);

				Front_Save_WriteCurrentSaveFiles();
			}
			else {
				/// FIX APPLY: Free SaveData when Front_Save_SetSaveData is not called.
				Front_Save_FreeSaveData(&copySave);

				/// REMOVE: Level index lookup with no usage.
			}
		}
		else {
			/// FIX APPLY: Free SaveData when Front_Save_SetSaveData is not called.
			Front_Save_FreeSaveData(&copySave);

			Front_Save_SetSaveNumber(lastSaveNumber);

			// This should be the same as lastSave.
			SaveData* currSave = Front_Save_GetCurrentSaveData();
			if (currSave != nullptr) {
				Front_Save_SetLevelCompleted(levelIndex);
				Front_Save_SetRewardLevel(levelIndex, rewardLevel);

				Object_Save_CopyStruct18(&saveStruct18);
				Front_Save_SetSaveStruct18(&saveStruct18);
			}
		}
	}
}

// <LegoRR.exe @00462530>
//#define Reward_GotoAdvance ((void (__cdecl* )(void))0x00462530)
void __cdecl LegoRR::Reward_GotoAdvance(void)
{
	const sint32 levelIndex = Front_LevelSet_IndexOf(Front_Levels_GetTutoOrMissions(), Lego_GetLevel()->name);

	const sint32 lastSaveNumber = Front_Save_GetSaveNumber();
	if (lastSaveNumber == -1) {
		// Change to the local save if no save is active.
		Front_Save_SetSaveNumber(5);
	}

	if (Lego_GetLevel()->status == LEVELSTATUS_COMPLETE) {
		Front_Save_SetLevelCompleted((uint32)levelIndex);
	}
	Front_Save_SetRewardLevel(levelIndex, GetRewardLevel());

	if (lastSaveNumber == -1) {
		// This is the first time the local save has been used (or after clearing). So clear all missions first.
		SaveData* localSave = Front_Save_GetSaveDataAt(5);
		if (localSave != nullptr) {
			/// TODO: Why are we starting at one??? Does this have something to do
			///        with that > count bounds check originally in Front_Save_SetLevelCompleted?
			///       Or is this because the first tutorial mission is always unlocked?
			std::memset(localSave->missionsTable + 1, 0, ((localSave->missionsCount - 1) * sizeof(SaveReward)));
			//for (uint32 i = 1; i < localSave->missionsCount; i++) {
			//
			//}

			// Apply the changes again because we just overwrote them with memset.
			if (Lego_GetLevel()->status == LEVELSTATUS_COMPLETE) {
				Front_Save_SetLevelCompleted((uint32)levelIndex);
			}
			Front_Save_SetRewardLevel(levelIndex, GetRewardLevel());
		}

		// Write the local save file.
		Front_Save_WriteCurrentSaveFiles();

		/// FIX APPLY: We need to free missionsTable before reading over it!
		Front_Save_FreeSaveData(&frontGlobs.saveData[5]);

		// Then reload the local save file for whatever reason (to confirm the save file is accurate maybe?)
		Front_Save_ReadSaveFile(5, &frontGlobs.saveData[5], false);
	}

	MenuItem_SelectData* select = frontGlobs.mainMenuSet->menus[1]->items[1]->itemData.select;
	SaveData* currSave = Front_Save_GetCurrentSaveData();
	Front_Levels_UpdateAvailable(frontGlobs.startMissionLink, currSave->missionsTable, &frontGlobs.missionLevels, select, false);

	Front_Save_SetShouldClearUnlockedLevels(false);
}

// <LegoRR.exe @00462650>
void __cdecl LegoRR::Reward_HandleDebugKeys(IN OUT Reward_Type* rewardType, IN OUT RewardUIState* state, IN OUT bool32* finished, IN OUT real32* timer)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return;
	}

	if (!(legoGlobs.flags2 & GameFlags2::GAME2_ALLOWDEBUGKEYS))
	{
		return;
	}

	// If Spacebar is down and escape is pressed, terminate the program
	if (Input_IsKeyDown(DIK_SPACE) && Input_IsKeyPressed(DIK_ESCAPE))
	{
		Lego_Exit();
	}

	// If key R is pressed, restart the reward showcase from the beginning
	if (Input_IsKeyPressed(DIK_R))
	{
		*rewardType = Reward_Type::Reward_Crystals;
		*state = RewardUIState::REWARDSTATE_0;
		*timer = rewardGlobs.timer;
		*finished = FALSE;
		Gods98::Sound3D_StopSound(SFX_ID::SFX_MusicLoop);
		SFX_AddToQueue(SFX_ID::SFX_MusicLoop, Gods98::SoundMode::Loop);
		for (RewardLevelItem& item : reward->items)
		{
			Reward_PlayFlic(&item);
		}
	}
}

// <LegoRR.exe @00462720>
void __cdecl LegoRR::Reward_PlayFlic(RewardLevelItem* rewardItem)
{
	if (rewardItem->flags & RewardItemFlags::REWARDITEM_FLAG_FLIC)
	{
		Gods98::Flic_Close(rewardItem->Flic);
		Gods98::Flic_Setup(rewardItem->FlicName, &rewardItem->Flic, Gods98::FlicUserFlags::FLICDISK);
	}
}

// <LegoRR.exe @00462760>
void __cdecl LegoRR::Reward_UpdateState(IN OUT Reward_Type* rewardType, IN OUT RewardUIState* state, IN OUT real32* timer)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return;
	}

	bool progressState = false;

	if (rewardGlobs.timer <= *timer && *state == REWARDSTATE_0) {
		progressState = true;
	}

	if ((*state != REWARDSTATE_0)
		&& ((rewardGlobs.timer * 2) <= *timer)
		&& (Lego_GetLevel()->status == LevelStatus::LEVELSTATUS_COMPLETE)
		&& (*rewardType < Reward_Type::Reward_Type_Count))
	{
		progressState = true;
	}

	if (!progressState)
	{
		return;
	}

	*timer -= rewardGlobs.timer;

	sint32 rewardTypeInt = *rewardType;
	if (rewardTypeInt < Reward_Type::Reward_Type_Count)
	{
		++rewardTypeInt;
	}
	*rewardType = static_cast<Reward_Type>(rewardTypeInt);

	char const* const sfxName = reward->items[rewardTypeInt - 1].SoundName;
	SFX_ID sfxType;
	if (sfxName != nullptr && SFX_GetType(sfxName, &sfxType))
	{
		SFX_AddToQueue(sfxType, Gods98::SoundMode::Once);
	}

	if (*rewardType >= Reward_Type::Reward_Type_Count)
	{
		if (*state == RewardUIState::REWARDSTATE_0)
		{
			*rewardType = Reward_Type::Reward_Score;
			sint32 stateInt = static_cast<sint32>(*state);
			*state = static_cast<RewardUIState>(stateInt + 1);
			return;
		}

		if ((*rewardType >= Reward_Type::Reward_Type_Count) && (*state >= RewardUIState::REWARDSTATE_1))
		{
			if (Reward_Type::Reward_Type_Count < *rewardType)
			{
				*rewardType = Reward_Type::Reward_Type_Count;
				sint32 stateInt = static_cast<sint32>(*state);
				*state = static_cast<RewardUIState>(stateInt + 1);
			}

			for (auto& item : reward->items)
			{
				Reward_PlayFlic(&item);
			}
		}
	}
}

// <LegoRR.exe @004628c0>
void __cdecl LegoRR::Reward_DrawAllValues(IN Reward_Type* rewardType, OUT bool32* finished)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return;
	}

	if (((*rewardType == Reward_Type::Reward_Type_Count)
		 || (level->status != LevelStatus::LEVELSTATUS_COMPLETE
			 && (*rewardType == Reward_Type::Reward_Score)))
		&& (reward->items[Reward_Type::Reward_Score].Flic == nullptr
			|| rewardGlobs.displayFlics == 0
			|| reward->items[Reward_Type::Reward_Score].Flic->currentframe >= 33
			|| level->status != LevelStatus::LEVELSTATUS_COMPLETE))
	{
		*finished = TRUE;
	}

	for (sint32 idx = 0; idx < Reward_Type::Reward_Type_Count; ++idx)
	{
		sint32 rewardTypeInt = static_cast<sint32>(*rewardType);
		if (rewardTypeInt >= Reward_Type::Reward_Type_Count)
		{
			rewardTypeInt = Reward_Type::Reward_Score;
		}

		if (rewardTypeInt <= idx)
		{
			break;
		}

		Reward_DrawItem(&reward->items[idx], RewardItemFlags::REWARDITEM_FLAG_VALUETEXT, *rewardType);
	}

	// These calls server no purpose
	Gods98::Font_GetStringWidth(rewardGlobs.titleFont, level->FullName);
	Gods98::Font_GetStringWidth(rewardGlobs.titleFont, rewardGlobs.statusMessage);
}

// <LegoRR.exe @004629c0>
void __cdecl LegoRR::Reward_LoopUpdate(real32 elapsedSeconds)
{
	/// FIX APPLY: Seconds to standard framerate.
	const real32 elapsed = (elapsedSeconds * STANDARD_FRAMERATE);

	/// FIX APPLY: Properly pass elapsed time to Pointer_Update
	Pointer_Update(elapsed);
	Pointer_SetCurrent_IfTimerFinished(Pointer_Standard);
	Pointer_DrawPointer(Gods98::msx(), Gods98::msy());

	Lego_UpdateGameCtrlLeftButtonLast();
	Gods98::INPUT.lClicked = false;

	SFX_Update(elapsed);

	// Why is it that ToolTips are the biggest thing NOT using standard framerate internally??
	ToolTip_Update(Gods98::msx(), Gods98::msy(), elapsed);

	Gods98::Main_LoopUpdate(true);
}

// <LegoRR.exe @00462a40>
bool32 __cdecl LegoRR::Reward_LoopBegin(void)
{
	Lego_Level* level = Lego_GetLevel();
	if (level == nullptr)
	{
		return FALSE;
	}

	RewardLevel* reward = GetRewardLevel2(level->unused_rewardID);
	if (reward == nullptr)
	{
		return FALSE;
	}

	if (rewardGlobs.display && reward->Enabled)
	{
		while (Input_IsKeyUp(DIK_SPACE) && !Input_IsKeyReleased(DIK_SPACE))
		{
			Gods98::Main_LoopUpdate(TRUE);
		}
		Gods98::INPUT.lClicked = FALSE;
		SFX_AddToQueue(SFX_ID::SFX_MusicLoop, Gods98::SoundMode::Loop);
		return TRUE;
	}

	Reward_GotoAdvance();
	return FALSE;
}

// <LegoRR.exe @00462ac0>
void __cdecl LegoRR::RewardQuota_UpdateTimers(real32 elapsedGame)
{
	RewardLevelItem& timer = rewardGlobs.current.items[Reward_Type::Reward_Timer];
	timer.countdown += 1.0f;
	timer.countdownRatio += elapsedGame;
}

// <LegoRR.exe @00462af0>
void __cdecl LegoRR::RewardQuota_WallDestroyed(void)
{
	rewardGlobs.current.items[Reward_Type::Reward_Timer].countdown += 1.0f;
}

// <LegoRR.exe @00462b10>
void __cdecl LegoRR::RewardQuota_CavernDiscovered(void)
{
	rewardGlobs.current.items[Reward_Type::Reward_Caverns].countdown += 1.0f;
}

// <LegoRR.exe @00462b30>
void __cdecl LegoRR::RewardQuota_RockMonsterGenerated(void)
{
	rewardGlobs.current.items[Reward_Type::Reward_RockMonsters].numGenerated += 1;
}

// <LegoRR.exe @00462b40>
void __cdecl LegoRR::RewardQuota_RockMonsterDestroyed(void)
{
	rewardGlobs.current.items[Reward_Type::Reward_RockMonsters].numDestroyed += 1;
}

// <LegoRR.exe @00462b50>
void __cdecl LegoRR::RewardQuota_RockMonsterAttacked(void)
{
	rewardGlobs.current.items[Reward_Type::Reward_RockMonsters].numAttacked += 1;
}

// <LegoRR.exe @00462b60>
void __cdecl LegoRR::RewardQuota_RockMonsterDamageDealt(real32 damage)
{
	rewardGlobs.current.items[Reward_Type::Reward_RockMonsters].damageTaken += damage;
}

// <LegoRR.exe @00462b80>
void __cdecl LegoRR::RewardQuota_MiniFigureDamageTaken(real32 damage)
{
	rewardGlobs.current.items[Reward_Type::Reward_Figures].damageTaken += damage;
}

// <LegoRR.exe @00462ba0>
LegoRR::RewardScroll* __cdecl LegoRR::RewardScroll_Create(OUT RewardScroll** scroll, real32 zero, real32 heightDiv20, real32 width, real32 bottomSubDiv3pt5, real32 scrollSpeed)
{
	RewardScroll* front = reinterpret_cast<RewardScroll*>(Gods98::Mem_Alloc(sizeof(RewardScroll)));

	if (front != nullptr)
	{
		std::memset(front, 0, sizeof(RewardScroll));
		front->labels = reinterpret_cast<RewardScrollLabel**>(Gods98::Mem_Alloc(sizeof(RewardScrollLabel*)));
		*front->labels = reinterpret_cast<RewardScrollLabel*>(Gods98::Mem_Alloc(sizeof(RewardScrollLabel)));
		std::memset(front->labels[0], 0, sizeof(RewardScrollLabel));

		front->scrollSpeed = scrollSpeed;
		front->xPos = zero;
		front->yPos = heightDiv20;
		front->width = width;
		front->height = bottomSubDiv3pt5;
		front->yInitial = heightDiv20;
	}

	*scroll = front;
	return front;
}

// <LegoRR.exe @00462c20>
bool32 __cdecl LegoRR::RewardScroll_Free(IN OUT RewardScroll** scroll)
{
	RewardScroll* scroll_ptr = *scroll;
	if (scroll_ptr->labelCount != 0)
	{
		for (uint32 idx = 0; idx < scroll_ptr->labelCount; ++idx)
		{
			RewardScrollLabel* label = scroll_ptr->labels[idx];
			if (label->text != nullptr)
			{
				Gods98::Mem_Free(label->text);
			}
			Gods98::Mem_Free(label);
		}
	}

	if (scroll_ptr->labels != nullptr)
	{
		Gods98::Mem_Free(scroll_ptr->labels);
	}

	Gods98::Mem_Free(scroll_ptr);
	return TRUE;
}

// <LegoRR.exe @00462c90>
LegoRR::RewardScrollLabel* __cdecl LegoRR::RewardScroll_AddLabel(RewardScroll* scroll, const char* text, Gods98::Font* font, real32 xPos, real32 yPos, RewardScrollLabelFlags labelFlags)
{
	if (font == nullptr)
	{
		return nullptr;
	}

	int const idx = scroll->labelCount;
	scroll->labelCount = idx + 1;

	scroll->labels = reinterpret_cast<RewardScrollLabel**>(
		Gods98::Mem_ReAlloc(scroll->labels, scroll->labelCount * sizeof(RewardScrollLabel*)));
	RewardScrollLabel* label = reinterpret_cast<RewardScrollLabel*>(Gods98::Mem_Alloc(sizeof(RewardScrollLabel)));
	scroll->labels[idx] = label;
	std::memset(label, 0, sizeof(RewardScrollLabel));

	label->xPos = xPos;
	label->yPos = yPos;

	std::size_t const text_length = std::strlen(text);
	label->text = reinterpret_cast<char*>(Gods98::Mem_Alloc(text_length));
	std::sprintf(label->text, "%s", text);

	label->mode = RewardScrollLabelMode::REWARDSCROLL_MODE_TEXT;
	label->font = font;
	label->flags |= labelFlags;

	return label;
}

// <LegoRR.exe @00462d70>
void __cdecl LegoRR::RewardScroll_SetDelay_Unk(RewardScroll* scroll, real32 curScrollY)
{
	scroll->curScrollY = curScrollY;
}

// <LegoRR.exe @00462d80>
void __cdecl LegoRR::RewardScroll_AddFlags(RewardScroll* scroll, RewardScrollFlags flags)
{
	scroll->flags |= flags;
}

// <LegoRR.exe @00462d90>
bool32 __cdecl LegoRR::RewardScroll_DrawLabels(RewardScroll* scroll)
{
	for (uint32 idx = 0; idx < scroll->labelCount; ++idx)
	{
		RewardScrollLabel* label = scroll->labels[idx];

		int textXPos = static_cast<int>(label->xPos + scroll->xPos);
		int textYPos = static_cast<int>(label->yPos + scroll->yPos);
		if (scroll->flags & RewardScrollFlags::REWARDSCROLL_UNK_1)
		{
			while (textYPos < scroll->yInitial)
			{
				int const yOffset = static_cast<int>(scroll->curScrollY + scroll->height);
				textYPos += yOffset;
			}

			float const scrollHeight = scroll->curScrollY + scroll->yInitial + scroll->height;
			while (scrollHeight < textYPos)
			{
				textYPos -= static_cast<int>(scroll->height);
			}
		}

		switch (label->mode)
		{
		case RewardScrollLabelMode::REWARDSCROLL_MODE_WINDOW:
			Gods98::TextWindow_ChangePosition(label->textWnd, textXPos, textYPos);
			Gods98::TextWindow_Update(label->textWnd, 0, Gods98::mainGlobs.fixedFrameTiming, nullptr);
			// Fallthrough

		case RewardScrollLabelMode::REWARDSCROLL_MODE_IMAGE:
			Point2F destPos, destSize;
			destPos.x = static_cast<float>(textXPos);
			destPos.y = static_cast<float>(textYPos);
			destSize.x = static_cast<float>(label->image->width);
			destSize.y = static_cast<float>(label->image->height);
			Gods98::Image_DisplayScaled(label->image, nullptr, &destPos, &destSize);
			// Fallthrough

		case RewardScrollLabelMode::REWARDSCROLL_MODE_TEXT:
			if (label->flags & RewardScrollLabelFlags::REWARDSCROLL_LABEL_CENTERED)
			{
				textXPos += (static_cast<int>((scroll->width - scroll->xPos) / 2.0f)
							 - (Gods98::Font_GetStringWidth(label->font, label->text) / 2));
			}

			if (label->flags & RewardScrollLabelFlags::REWARDSCROLL_LABEL_NOSCROLL)
			{
				textYPos = static_cast<int>(label->yPos);
			}

			if (label->flags & RewardScrollLabelFlags::REWARDSCROLL_LABEL_NOSCROLL
				|| (textYPos < (scroll->height + scroll->yInitial) && scroll->yInitial < textYPos))
			{
				Gods98::Font_PrintF(label->font, textXPos, textYPos, label->text);
			}
		}
	}

	scroll->yPos -= scroll->scrollSpeed;

	if (legoGlobs.flags2 & GameFlags2::GAME2_ALLOWDEBUGKEYS)
	{
		if (Input_IsKeyDown(DIK_S))
		{
			scroll->scrollSpeed = 0.0f;
		}

		if (Input_IsKeyDown(DIK_R))
		{
			scroll->yPos = scroll->yInitial;
		}

		if (Input_IsKeyDown(DIK_EQUALS))
		{
			scroll->scrollSpeed += 0.01f;
		}

		if (Input_IsKeyDown(DIK_MINUS))
		{
			scroll->scrollSpeed -= 0.01f;
		}

		for (int key = 0; key < 10; ++key)
		{
			if (Input_IsKeyDown(DIK_1 + key))
			{
				scroll->scrollSpeed = (key + 1) * 0.1f;
			}
		}
	}

	return true;
}

#pragma endregion
