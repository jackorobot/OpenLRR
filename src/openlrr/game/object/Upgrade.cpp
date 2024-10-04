// Upgrade.cpp : 
//

#include "Upgrade.h"
#include "../Game.h"
#include "../../engine/core/Utils.h"

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // <LegoRR.exe @00406bc0>
void __cdecl LegoRR::Upgrade_Part_Hide(Upgrade_PartModel* upgradePart, bool32 hide)
{
	Gods98::Container_Hide(upgradePart->cont, hide);
}

// <LegoRR.exe @00406d60>
Gods98::Container* __cdecl LegoRR::Upgrade_Part_GetActivityContainer(Upgrade_PartModel* upgradePart)
{
	return upgradePart->cont;
}

// <LegoRR.exe @00406e80>
Gods98::Container* __cdecl LegoRR::Upgrade_Part_FindNull(Upgrade_PartModel* upgradePart, const char* name, uint32 frameNo)
{
	const char* partName = Gods98::Container_FormatPartName(upgradePart->cont, name, &frameNo);
	return Gods98::Container_SearchTree(upgradePart->cont, partName, Gods98::Container_SearchMode::FirstMatch, nullptr);
}

// <LegoRR.exe @004082b0>
void __cdecl LegoRR::Upgrade_Part_SetOwnerObject(Upgrade_PartModel* upgradePart, LegoObject* liveObj)
{
	Gods98::Container_SetUserData(upgradePart->cont, liveObj);
}

// <LegoRR.exe @004085d0>
bool32 __cdecl LegoRR::Upgrade_Part_IsHidden(Upgrade_PartModel* upgradePart)
{
	return Gods98::Container_IsHidden(upgradePart->cont);
}

// <LegoRR.exe @0046c2f0>
void __cdecl LegoRR::Upgrade_Part_Load(OUT Upgrade_PartModel* upgradePart, LegoObject_ID objID, Gods98::Container* root, const char* filename)
{
	std::memset(upgradePart, 0, sizeof(Upgrade_PartModel));

	upgradePart->cont = Gods98::Container_Load(root, filename, "LWO", TRUE);

	if (upgradePart->cont == nullptr)
	{
		upgradePart->cont = Gods98::Container_Load(root, filename, "ACT", TRUE);
	}

	if (upgradePart->cont == nullptr)
	{
		upgradePart->cont = Gods98::Container_Load(root, filename, "MESH", TRUE);
	}

	upgradePart->nextObject = nullptr;
	upgradePart->objID = objID;
}

// <LegoRR.exe @0046c370>
void __cdecl LegoRR::Upgrade_Part_Clone(IN Upgrade_PartModel* srcUpgradePart, OUT Upgrade_PartModel* destUpgradePart)
{
	*destUpgradePart = *srcUpgradePart;
	destUpgradePart->cont = Gods98::Container_Clone(srcUpgradePart->cont);
}

// <LegoRR.exe @0046c3b0>
void __cdecl LegoRR::Upgrade_Part_Remove(Upgrade_PartModel* upgradePart)
{
	Container_Remove(upgradePart->cont);
}

// <LegoRR.exe @0046c3d0>
real32 __cdecl LegoRR::Upgrade_Part_MoveAnimation(Upgrade_PartModel* upgradePart, real32 elapsed, uint32 unused_unkFrameNo)
{
	return Gods98::Container_MoveAnimation(upgradePart->cont, elapsed);
}

// <LegoRR.exe @0046c3f0>
bool32 __cdecl LegoRR::Upgrade_Part_SetActivity(Upgrade_PartModel* upgradePart, const char* activityName)
{
	if (Gods98::Container_SetActivity(upgradePart->cont, activityName) != FALSE)
	{
		return TRUE;
	}

	return Gods98::Container_SetActivity(upgradePart->cont, objectGlobs.activityName[0]);
}

// <LegoRR.exe @0046c420>
void __cdecl LegoRR::Upgrade_Load(OUT UpgradesModel* upgrades, const Gods98::Config* act, const char* gameName)
{
	upgrades->partCount = 0;
	upgrades->parts = reinterpret_cast<Upgrade_PartInfo*>(Gods98::Mem_Alloc(UPGRADE_NUM_PARTS * sizeof(Upgrade_PartInfo)));
	std::memset(upgrades->parts, 0, UPGRADE_NUM_PARTS * sizeof(Upgrade_PartInfo));

	for (sint32 level = 0; level <= LegoObject_UpgradeFlags::UPGRADE_FLAGS_ALL; ++level)
	{
		char buff[256];
		std::sprintf(buff, "Level%i%i%i%i", level >> 3 & 1, level >> 2 & 1, level >> 1 & 1, level & 1);
		char const* keyPath = Gods98::Config_BuildStringID(gameName, "Upgrades", buff);
		Gods98::Config const* prop = Gods98::Config_FindArray(act, keyPath);
		if (prop == nullptr)
		{
			continue;
		}

		upgrades->levelFlags |= static_cast<LegoObject_UpgradeFlags>(level);

		for (; prop != nullptr; prop = Gods98::Config_GetNextItem(prop))
		{
			Upgrade_PartInfo& part = upgrades->parts[upgrades->partCount];
			std::strcpy(buff, prop->dataString);
			char* stringParts[3] {};
			uint32 const tokens = Gods98::Util_Tokenise(buff, stringParts, ",");
			if (tokens == 3)
			{
				part.weaponName = Gods98::Util_StrCpy(stringParts[2]);
				part.nullObjectName = Gods98::Util_StrCpy(stringParts[0]);
				part.level = level;
				part.nullFrameNo = std::atoi(stringParts[1]) - 1;
				part.upgradeData = Lego_GetUpgradePartModel(prop->itemName);
				upgrades->partCount += 1;
			}
		}
	}

	if (upgrades->partCount == 0)
	{
		Gods98::Mem_Free(upgrades->parts);
		upgrades->parts = nullptr;
	}
}

// <LegoRR.exe @0046c600>
void __cdecl LegoRR::Upgrade_SetUpgradeLevel(UpgradesModel* upgrades, uint32 objLevel)
{
	LegoObject* object = upgrades->firstObject;
	LegoObject* nextObject;
	while (object != nullptr)
	{
		nextObject = object->upgradePart->nextObject;
		LegoObject_Remove(object);
		object = nextObject;
	}

	upgrades->currentLevel = objLevel;
	upgrades->firstObject = nullptr;

	LegoObject* previousObject {};
	for (uint32 id = 0; id < upgrades->partCount; ++id)
	{
		if (upgrades->parts[id].level == objLevel)
		{
			LegoObject* upgradeObject = LegoObject_Create(upgrades->parts[id].upgradeData, LegoObject_Type::LegoObject_UpgradePart, static_cast<LegoObject_ID>(id));
			if (previousObject == nullptr)
			{
				upgrades->firstObject = upgradeObject;
			}
			else
			{
				previousObject->upgradePart->nextObject = upgradeObject;
			}
			upgradeObject->upgradePart->partInfo = &upgrades->parts[id];
			previousObject = upgradeObject;
		}
	}
}

#pragma endregion
