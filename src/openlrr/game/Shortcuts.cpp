// Shortcuts.cpp : 
//

#include "../engine/core/Files.h"

#include "Game.h"
#include "Shortcuts.hpp"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

Shortcuts::ShortcutManager Shortcuts::shortcutManager = Shortcuts::ShortcutManager();

static size_t _shortcutsMaxNameLength = 0;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Shortcut_Register(name, defaultTextorFallback) shortcutInfos[static_cast<size_t>(name)] = ShortcutInfo(nameof(name), name, defaultTextorFallback)


// Pad shortcut name to the left for the max name length.
#define Shortcut_FormatName(name)			std::string((name)).append(std::max(_shortcutsMaxNameLength, std::string(name).length()) - std::string((name)).length(), ' ').c_str()

// General shortcuts error logging.
#define Shortcuts_DebugF(s, ...)			Error_DebugF2("Shortcuts: %s",   Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcuts_InfoF(s, ...)				Error_InfoF2( "Shortcuts: %s\n", Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcuts_WarnF(b, s, ...)			Error_WarnF2( (b), "Shortcuts: Warning: %s\n", Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcuts_FatalF(b, s, ...)			Error_FatalF2((b), "Shortcuts: Fatal: %s\n",   Gods98::Error_Format((s), __VA_ARGS__))

// Error logging for a specific shortcut. Name parameter can be c-string or std::string.
#define Shortcut_DebugF(name, s, ...)		Error_DebugF2("Shortcut %s: %s",   Shortcut_FormatName((name)), Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcut_InfoF(name, s, ...)		Error_InfoF2( "Shortcut %s: %s\n", Shortcut_FormatName((name)), Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcut_WarnF(b, name, s, ...)		Error_WarnF2( (b), "Shortcut %s: Warning: %s\n", Shortcut_FormatName((name)), Gods98::Error_Format((s), __VA_ARGS__))
#define Shortcut_FatalF(b, name, s, ...)	Error_FatalF2((b), "Shortcut %s: Fatal: %s\n",   Shortcut_FormatName((name)), Gods98::Error_Format((s), __VA_ARGS__))

#pragma endregion

/**********************************************************************************
 ******** Class Functions
 **********************************************************************************/

#pragma region ShortcutManager class definition

bool Shortcuts::ShortcutManager::Initialise()
{
	if (initialised)
		return false; // Initialise already called.

	shortcutInfos.clear();
	shortcutInfos.resize(static_cast<size_t>(ShortcutID::Count));

	Shortcut_Register(InterfaceActionModifier, "KEY_F2");
	Shortcut_Register(AddSelectionModifier, "KEY_LEFTSHIFT|KEY_RIGHTSHIFT");

	Shortcut_Register(ToggleObjectInfo, "KEY_SPACE");
	Shortcut_Register(EscapePause, "KEY_ESCAPE");
	Shortcut_Register(TogglePause, "KEY_P");

	Shortcut_Register(RenameUnit, "KEY_RETURN");
	// NOT REBINDABLE:
	//Shortcut_Register(AcceptRenameUnit, "KEY_RETURN|KEY_ESCAPE");

	Shortcut_Register(NERPsEndMessageWait, "KEY_RETURN");

	Shortcut_Register(FPMoveForward, "KEY_CURSORUP");
	Shortcut_Register(FPMoveBackward, "KEY_CURSORDOWN");
	Shortcut_Register(FPTurnLeft, "KEY_CURSORLEFT");
	Shortcut_Register(FPTurnRight, "KEY_CURSORRIGHT");
	Shortcut_Register(FPStrafeLeft, "KEY_Z");
	Shortcut_Register(FPStrafeRight, "KEY_X");

	Shortcut_Register(CameraTiltUp, "KEY_CURSORUP");
	Shortcut_Register(CameraTiltDown, "KEY_CURSORDOWN");
	Shortcut_Register(CameraTurnLeft, "KEY_CURSORLEFT");
	Shortcut_Register(CameraTurnRight, "KEY_CURSORRIGHT");
	Shortcut_Register(CameraZoomIn, "KEY_EQUALS");
	Shortcut_Register(CameraZoomOut, "KEY_MINUS");

	Shortcut_Register(Debug_HardExit, "KEY_ESCAPE+KEY_SPACE|KEY_SPACE+KEY_ESCAPE");
	Shortcut_Register(Debug_SoftExit, "KEY_ESCAPE+KEY_RETURN|KEY_RETURN+KEY_ESCAPE");

	Shortcut_Register(SwitchRadarMode, "KEY_TAB");
	Shortcut_Register(Debug_ToggleNoNERPs, "KEY_F12");
	Shortcut_Register(Debug_ToggleBuildDependencies, "KEY_F11");
	Shortcut_Register(Debug_ToggleInvertLighting, "KEY_F10");
	Shortcut_Register(Debug_ToggleSpotlightEffects, "KEY_F9");
	Shortcut_Register(Debug_SwitchDebugOverlay, "KEY_F8");

	Shortcut_Register(Debug_ToggleFallins, "KEY_F6");
	Shortcut_Register(Debug_ToggleFPNoClip, "KEY_LEFTCTRL+KEY_RETURN");

	Shortcut_Register(Debug_DestroyWall, "KEYPAD_DELETE");
	Shortcut_Register(Debug_DestroyWallConnection, "KEYPAD_3");

	Shortcut_Register(MaxGameSpeed, "KEYPAD_7");
	Shortcut_Register(DefaultGameSpeed, nullptr);
	Shortcut_Register(MinGameSpeed, nullptr);
	Shortcut_Register(Debug_FreezeGameSpeed, nullptr);
	Shortcut_Register(DecreaseGameSpeed, "KEYPAD_8");
	Shortcut_Register(IncreaseGameSpeed, "KEYPAD_9");

	Shortcut_Register(Debug_CommandDropDynamite, "KEY_C");
	// Avoid conflicts with Reload keybinds command.
	Shortcut_Register(Debug_CommandRegisterVehicle, "!KEY_LEFTSHIFT+!KEY_RIGHTSHIFT+KEY_K");
	Shortcut_Register(Debug_CommandGetInRegisteredVehicle, "!KEY_LEFTSHIFT+!KEY_RIGHTSHIFT+KEY_K");

	Shortcut_Register(Debug_Unknown_Water, "KEY_W");
	Shortcut_Register(Debug_Unknown_Gather, "KEY_BACKSPACE");
	Shortcut_Register(Debug_Unknown_ElectricFence, "KEY_H");

	Shortcut_Register(Debug_StopRouting, "KEY_N");

	Shortcut_Register(Debug_ToggleSelfPowered, "KEY_END");

	Shortcut_Register(Debug_EndAdvisorAnim, "!KEY_LEFTSHIFT+KEY_U");
	Shortcut_Register(Debug_BeginAdvisorAnim, "KEY_LEFTSHIFT+KEY_U");

	Shortcut_Register(Edit_PlaceElectricFence, "KEY_J");
	Shortcut_Register(Edit_PlaceSpiderWeb, "KEY_H");

	Shortcut_Register(Edit_PlacePath, nullptr);
	Shortcut_Register(Edit_PlaceCrystal, nullptr);
	Shortcut_Register(Edit_PlaceOre, nullptr);
	Shortcut_Register(Cheat_IncreaseCrystalsStored, nullptr);
	Shortcut_Register(Cheat_IncreaseOreStored, nullptr);
	
	Shortcut_Register(ToggleMusic, "KEY_M");
	Shortcut_Register(ToggleSound, "KEY_S");

	Shortcut_Register(Debug_DecreaseOxygen, "KEY_O");
	Shortcut_Register(Debug_IncreaseOxygen, nullptr);
	Shortcut_Register(Debug_CreateLandslide, "KEY_A");

	Shortcut_Register(Debug_TripUnit, "KEY_A");
	Shortcut_Register(Debug_BumpUnit, "KEY_B");
	Shortcut_Register(Debug_DamageUnit, "KEY_F");
	Shortcut_Register(Cheat_HealUnit, nullptr);

	Shortcut_Register(Debug_CommandPlaceSonicBlaster, "KEY_LEFTSHIFT+KEY_A");
	Shortcut_Register(Debug_CommandEat, "KEY_Z");

	Shortcut_Register(Debug_ShakeScreen, "KEY_Z");

	Shortcut_Register(Debug_InterfaceCrystalFoundMessage, "KEY_Y");

	Shortcut_Register(Debug_EmergeMonster, "KEY_E");
	Shortcut_Register(Debug_CommandGatherBoulder, "KEY_W");

	Shortcut_Register(ChangeViewFP1, "KEY_ONE");
	Shortcut_Register(ChangeViewFP2, "KEY_TWO");
	Shortcut_Register(ChangeViewTop, "KEY_THREE");
	Shortcut_Register(TrackUnit, "KEY_FOUR");
	Shortcut_Register(FollowUnit, nullptr);
	Shortcut_Register(Cheat_FasterUnit, nullptr);
	Shortcut_Register(LaserTrackerUnit, nullptr);
	Shortcut_Register(Edit_DestroyUnits, nullptr);
	Shortcut_Register(Cheat_MaxOutUnit, nullptr);

	Shortcut_Register(Debug_ToggleUpgradeCarry, "KEY_FIVE");
	Shortcut_Register(Debug_ToggleUpgradeScan, "KEY_SIX");
	Shortcut_Register(Debug_ToggleUpgradeSpeed, "KEY_SEVEN");
	Shortcut_Register(Debug_ToggleUpgradeDrill, "KEY_EIGHT");

	Shortcut_Register(Debug_DecreaseSpotlightPenumbra, "!KEY_LEFTSHIFT+KEY_FIVE");
	Shortcut_Register(Debug_IncreaseSpotlightPenumbra, "KEY_LEFTSHIFT+KEY_FIVE");
	Shortcut_Register(Debug_DecreaseSpotlightUmbra, "!KEY_LEFTSHIFT+KEY_SIX");
	Shortcut_Register(Debug_IncreaseSpotlightUmbra, "KEY_LEFTSHIFT+KEY_SIX");
	Shortcut_Register(Debug_DecreaseSound3DRollOffFactor, "KEY_NINE");
	Shortcut_Register(Debug_IncreaseSound3DRollOffFactor, "KEY_ZERO");

	Shortcut_Register(Debug_ToggleFPSMonitor, "KEY_LEFTCTRL+KEY_F");
	Shortcut_Register(Debug_ToggleMemoryMonitor, "KEY_LEFTCTRL+KEY_G");

	Shortcut_Register(Debug_WinLevelInstant, "KEY_L");
	Shortcut_Register(Debug_LoseLevel, "KEY_LEFTCTRL+KEY_D");
	Shortcut_Register(Debug_WinLevel, "KEY_LEFTCTRL+KEY_S");
	Shortcut_Register(Debug_LoseLevelCrystals, "KEY_RIGHTCTRL+KEY_S");
	Shortcut_Register(Debug_ToggleFreeCameraMovement, "KEYPAD_0");

	Shortcut_Register(Cheat_TopdownFPMoveForward, FPMoveForward);
	Shortcut_Register(Cheat_TopdownFPMoveBackward, FPMoveBackward);
	Shortcut_Register(Cheat_TopdownFPTurnLeft, FPTurnLeft);
	Shortcut_Register(Cheat_TopdownFPTurnRight, FPTurnRight);
	Shortcut_Register(Cheat_TopdownFPStrafeLeft, FPStrafeLeft);
	Shortcut_Register(Cheat_TopdownFPStrafeRight, FPStrafeRight);

	Shortcut_Register(Edit_SelectMonstersModifier, "KEY_T");
	Shortcut_Register(Edit_SelectResourcesModifier, "KEY_R");

	Shortcut_Register(Edit_TogglePower, nullptr);
	Shortcut_Register(Cheat_FreezeUnit, nullptr);
	Shortcut_Register(Cheat_PlaceDynamite, nullptr);
	Shortcut_Register(Cheat_PlaceDynamiteInstant, nullptr);
	Shortcut_Register(Cheat_PlaceSonicBlaster, nullptr);
	Shortcut_Register(Cheat_PlaceSonicBlasterInstant, nullptr);
	Shortcut_Register(Cheat_KamikazeUnit, nullptr);

	Shortcut_Register(CameraCycleNextUnit, nullptr);


	Shortcut_Register(ReloadKeyBinds, "KEY_LEFTCTRL+KEY_LEFTSHIFT+KEY_K|KEY_RIGHTCTRL+KEY_RIGHTSHIFT+KEY_K");


	// Make sure we've registered all of our KeyBindIDs.
	std::vector<size_t> missingList;
	
	// Start at 1 to skip ShortcutID::None.
	for (size_t i = 1; i < static_cast<size_t>(ShortcutID::Count); i++) {
		const auto& shortcutInfo = shortcutInfos[i];
		if (shortcutInfo.name.empty()) {
			missingList.push_back(i);
		}
	}
	if (!missingList.empty()) {
		std::string s = "Missing ShortcutID registrations:";
		for (size_t i = 0; i < missingList.size(); i++) {
			//s += "\n- ";
			if (i > 0) s += ", ";
			s += std::to_string(missingList[i]);
		}
		Shortcuts_FatalF(true, "%s", s.c_str());
	}

	// Get padding size used for formatting messages.
	for (size_t i = 0; i < static_cast<size_t>(ShortcutID::Count); i++) {
		_shortcutsMaxNameLength = std::max(_shortcutsMaxNameLength, shortcutInfos[i].name.length());
	}

	// Load default keybinds once to confirm that they're defined correctly.
	this->LoadDefaults();

	initialised = false;
	return true;
}

void Shortcuts::ShortcutManager::Shutdown()
{
	shortcutInfos.clear();
}

bool Shortcuts::ShortcutManager::Update(real32 elapsed, bool checkForReload)
{
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Update(elapsed);
	}

	if (checkForReload) {
		if (Shortcut_IsPressed(ShortcutID::ReloadKeyBinds)) {
			Load();
			return true;
		}
	}
	return false;
}


void Shortcuts::ShortcutManager::Reset(bool release)
{
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Reset(release);
	}
}

void Shortcuts::ShortcutManager::Enable()
{
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Enable();
	}
}

void Shortcuts::ShortcutManager::Disable(bool untilRelease)
{
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Disable(untilRelease);
	}
}

void Shortcuts::ShortcutManager::ClearAssignments()
{
	for (auto& shortcutInfo : shortcutInfos) {
		if (shortcutInfo.shortcutID != ShortcutID::None) {
			shortcutInfo.button = nullptr;
		}
	}
}

void Shortcuts::ShortcutManager::LoadDefaults()
{
	// Nullify all keybinds to reassign defaults.
	ClearAssignments();

	AssignDefaults();

	// First update all keybinds once to gather their current state.
	// Then disable them until release so that nothing is instantly triggered during reload.
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Update(0.0f);
	}
	Disable(true);
}

void Shortcuts::ShortcutManager::AssignDefaults()
{
	// The number of keybinds that will fallback to existing keybinds to parse.
	size_t fallbackCount = 0;
	for (auto& shortcutInfo : shortcutInfos) {
		if (shortcutInfo.shortcutID != ShortcutID::None && shortcutInfo.button == nullptr) {
			if (shortcutInfo.fallbackID == ShortcutID::None) {
				if (shortcutInfo.defaultText.empty()) {
					shortcutInfo.button = std::make_shared<Gods98::InputButton>(); // No binding.
				}
				else if (!Parse(shortcutInfo, shortcutInfo.defaultText)) {
					Shortcut_FatalF(true, shortcutInfo.name, "%s", "Failed to parse defaultText.");
				}
			}
			else {
				fallbackCount++;
			}
		}
	}

	size_t attemptsLeft = fallbackCount; // Count loops max are needed to resolve references.
	while (fallbackCount > 0) {
		/*if (attemptsLeft-- == 0) {
			Shortcuts_WarnF(true, "Found %i default fallback cyclic references.", fallbackCount);
			break;
		}*/

		fallbackCount = 0;
		for (auto& shortcutInfo : shortcutInfos) {

			if (shortcutInfo.shortcutID != ShortcutID::None && shortcutInfo.button == nullptr) {
				if (shortcutInfo.fallbackID != ShortcutID::None) {
					Shortcut_FatalF(!initialised && (shortcutInfo.shortcutID == shortcutInfo.fallbackID),
									shortcutInfo.name, "%s", "Same shortcutID as fallbackID.");
					
					if (attemptsLeft == 0) {
						Shortcut_FatalF(!initialised, shortcutInfo.name, "%s", "Default fallbackID is a cyclic reference.");

						shortcutInfo.button = std::make_shared<Gods98::InputButton>(); // No binding.
					}
					else {
						auto& fallback = shortcutInfos[static_cast<size_t>(shortcutInfo.fallbackID)];
						if (fallback.button != nullptr) {
							shortcutInfo.button = shortcutInfos[static_cast<size_t>(shortcutInfo.fallbackID)].button;
						}
						else {
							fallbackCount++;
						}
					}
				}
			}
		}

		if (attemptsLeft-- == 0) {
			Shortcuts_WarnF(true, "Found %i default fallback cyclic references.", fallbackCount);
			break;
		}
	}

}

bool Shortcuts::ShortcutManager::Load()
{
	// Nullify all keybinds to reassign.
	ClearAssignments();

	bool ok = true;

	Gods98::Config* config = Gods98::Config_Load2(SHORTCUTS_FILENAME, Gods98::FileFlags::FILE_FLAG_EXEDIR|Gods98::FileFlags::FILE_FLAG_NOCD); // Load using exeDir.
	if (config != nullptr) {

		const bool fallbackToDefaults = Config_GetBoolOrTrue(config, Lego_ID("FallbackToDefaultKeyBinds"));

		const Gods98::Config* arrayFirst = Gods98::Config_FindArray(config, Lego_ID("KeyBinds"));
		if (!arrayFirst) {
			Shortcuts_WarnF(true, "%s", "Could not find KeyBinds array.");
			ok = false;
		}

		for (const Gods98::Config* prop = arrayFirst; prop != nullptr; prop = Gods98::Config_GetNextItem(prop)) {

			const char* key = Gods98::Config_GetItemName(prop);
			const char* origValue = Gods98::Config_GetDataString(prop);

			bool found = false;
			for (auto& shortcutInfo : shortcutInfos) {
				if (::_stricmp(shortcutInfo.name.c_str(), key) == 0) {
					const char* value = origValue;
					found = true;

					// If the keybind contains "::", then its a reference to an
					//  existing fallback keybind in the configuration file.
					// Find the value of the referenced keybind and parse that instead.
					size_t attemptsLeft = static_cast<size_t>(ShortcutID::Count);
					while (value != nullptr && std::strstr(value, CONFIG_SEPARATOR) != nullptr) {
						if (attemptsLeft-- == 0) {
							Shortcut_WarnF(true, key, "Cyclic reference detected \"%s\".", origValue);
							value = nullptr;
							break;
						}

						const char* linkValue = Gods98::Config_GetTempStringValue(config, Lego_ID(value));

						Shortcut_WarnF(linkValue == nullptr, key, "Reference not found \"%s\".", origValue);

						value = linkValue;
					}

					if (value == nullptr || !Parse(shortcutInfo, value)) {
						Shortcut_WarnF(value != nullptr, key, "Failed to parse \"%s\".", value);
						//if (value) Error_InfoF("ERROR: KeyBind [%s] failed to parse \"%s\".", key, value);
						ok = false;

						if (!fallbackToDefaults) {
							shortcutInfo.button = std::make_shared<Gods98::InputButton>(); // No binding.
						}
					}
					else {
						Shortcut_DebugF(shortcutInfo.name, "Loaded: %s.\n", shortcutInfo.button->ToString().c_str());
					}
					break;
				}
			}
			if (!found) {
				Shortcut_WarnF(true, key, "%s", "Unknown shortcut ID.");
				ok = false;
			}
		}
	}
	else {
		Shortcuts_WarnF(true, "%s file not found, reverting to default keybinds.", SHORTCUTS_FILENAME);
		ok = false;
	}

	// Assign defaults for remaining keybinds left out of the config.
	AssignDefaults();

	// First update all keybinds once to gather their current state.
	// Then disable them until release so that nothing is instantly triggered during reload.
	for (auto& shortcutInfo : shortcutInfos) {
		shortcutInfo.button->Update(0.0f);
	}
	Disable(true);
	// Update and then disable until release to prevent this keybind from endlessly re-triggering itself.
	//shortcutInfos[ShortcutID::ReloadKeyBinds].button->Update(0.0f);
	//shortcutInfos[ShortcutID::ReloadKeyBinds].button->Disable(true);

	if (!loadedOnce) {
		loadedOnce = true;
	}
	else {
		Shortcuts_InfoF("%s", "Reloaded Shortcuts!");
	}
	return ok;
}


bool Shortcuts::ShortcutManager::Parse(ShortcutInfo& shortcutInfo, const std::string& text)
{
	// We need a manipulatable string for Util_Tokenise.
	char* str = Gods98::Util_StrCpy(text.c_str());
	char* s = str;


	#pragma region Prefix Modifiers
	bool asteriskModifier = false;
	bool atsignModifier = false;
	sint32 numericModifier = 0;

	if (*s == '*') { // asterisk modifier
		s++;
		asteriskModifier = true;
	}
	if (*s == '#') { // numeric modifier: #-vol#
		s++; // skip opening '#'
		std::string volStr = "";
		while (*s != '#') {
			volStr.append(1, *s++); // Copy number between #...#
		}
		s++; // skip closing '#'

		numericModifier = std::atoi(volStr.c_str());
	}
	if (*s == '@') { // atsign modifier
		s++;
		atsignModifier = true;
	}
	#pragma endregion


	std::vector<std::shared_ptr<Gods98::InputButtonBase>> multiButtonBuilder;

	bool ok = true;

	char* bind_argv[1024];// = { nullptr };
	const uint32 bind_argc = Gods98::Util_TokeniseSafe(s, bind_argv, "|", _countof(bind_argv));
	for (uint32 i = 0; i < bind_argc && ok; i++) {

		Gods98::InputComboOrder comboOrder = Gods98::InputComboOrder::Final;
		if (asteriskModifier)
			comboOrder = Gods98::InputComboOrder::Any;
		bool preserveCombo = false;

		std::vector<std::shared_ptr<Gods98::InputButtonBase>> comboButtonBuilder;

		// Check for always-on/always-off keybind: "ON" / "OFF"
		BoolTri boolOnOff = Gods98::Util_GetBoolFromString(bind_argv[i]);
		// LEGACY: Support for "NULL" ("OFF").
		if (boolOnOff == BoolTri::BOOL3_ERROR && ::_stricmp(bind_argv[i], "NULL") == 0) {
			boolOnOff = BoolTri::BOOL3_FALSE;
		}
		if (boolOnOff != BoolTri::BOOL3_ERROR) {
			// This is an always-on or always-off keybind.
			const bool alwaysDown = (boolOnOff == BoolTri::BOOL3_TRUE);
			multiButtonBuilder.push_back(std::make_shared<Gods98::InputButton>(alwaysDown));
			continue;
		}

		char* combo_argv[1024];// = { nullptr };
		const uint32 combo_argc = Gods98::Util_TokeniseSafe(bind_argv[i], combo_argv, "+", _countof(combo_argv));
		for (uint32 j = 0; j < combo_argc && ok; j++) {
			const char* part = combo_argv[j];
			const char* p = part;

			bool invertToggle = false;
			if (*p == '!') {
				p++;
				invertToggle = true;
			}

			const char* KEYCODE_PREFIX = "KEYCODE_";
			const char* MOUSECODE_PREFIX = "MOUSECODE_";

			Gods98::Keys keyCode = Gods98::Keys::KEY_NONE;
			Gods98::MouseButtons mouseCode = Gods98::MouseButtons::MOUSE_NONE;
			if (::_strnicmp(p, KEYCODE_PREFIX, std::strlen(KEYCODE_PREFIX)) == 0) {
				p += std::strlen(KEYCODE_PREFIX);
				const sint32 code = std::atoi(p);
				if (code < 0 || code > 255 || !Gods98::Util_IsNumber(p)) {
					Shortcut_WarnF(ok, shortcutInfo.name, "Key code \"%s\" is not a valid number between 0 and 255.", part);
					ok = false;
				}
				keyCode = static_cast<Gods98::Keys>(code);
				comboButtonBuilder.push_back(std::make_shared<Gods98::InputButton>(keyCode, invertToggle));
			}
			else if (::_strnicmp(p, MOUSECODE_PREFIX, std::strlen(MOUSECODE_PREFIX)) == 0) {
				p += std::strlen(MOUSECODE_PREFIX);
				const sint32 code = std::atoi(p);
				if (code < 0 || code > 5 || !Gods98::Util_IsNumber(p)) {
					Shortcut_WarnF(ok, shortcutInfo.name, "Mouse code \"%s\" is not a valid number between 0 and 5.", part);
					ok = false;
				}
				mouseCode = static_cast<Gods98::MouseButtons>(code);
				comboButtonBuilder.push_back(std::make_shared<Gods98::InputButton>(mouseCode, invertToggle));
			}
			else if (Gods98::Key_Find(p, &keyCode)) {
				comboButtonBuilder.push_back(std::make_shared<Gods98::InputButton>(keyCode, invertToggle));
			}
			else if (Gods98::MouseButton_Find(p, &mouseCode)) {
				comboButtonBuilder.push_back(std::make_shared<Gods98::InputButton>(mouseCode, invertToggle));
			}
			/*else if (::_stricmp(p, "NULL") == 0) {
				/// LEGACY: NULL for always-off keybind.
				comboButtonBuilder.push_back(std::make_shared<Gods98::InputButton>());
			}*/
			else {
				Shortcut_WarnF(ok, shortcutInfo.name, "Unknown key/mouse code \"%s\".", part);
				ok = false;
				comboButtonBuilder.clear();
				break;
			}
		}

		if (ok) {
			if (comboButtonBuilder.empty()) {
				Shortcut_WarnF(ok, shortcutInfo.name, "Empty keybind combo for \"%s\".", str);
				ok = false;
				multiButtonBuilder.clear();
				break;
			}
			else if (comboButtonBuilder.size() == 1) {
				multiButtonBuilder.push_back(comboButtonBuilder[0]); // Avoid pointless nesting.
			}
			else {
				multiButtonBuilder.push_back(std::make_shared<Gods98::ComboInputButton>(comboButtonBuilder, comboOrder, preserveCombo));
			}
		}
	}

	if (ok) {
		if (multiButtonBuilder.empty()) {
			Shortcut_WarnF(ok, shortcutInfo.name, "Empty keybinds for \"%s\".", str);
			ok = false;
		}
		else if (multiButtonBuilder.size() == 1) {
			shortcutInfo.button = multiButtonBuilder[0]; // Avoid pointless nesting.
		}
		else {
			shortcutInfo.button = std::make_shared<Gods98::MultiInputButton>(multiButtonBuilder);
		}
	}

	Gods98::Mem_Free(str);
	return true;
}

#pragma endregion
