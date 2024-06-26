// NERPsFunctions.h : 
//

#pragma once

#include "../GameCommon.h"

#include "NERPsFile.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define NERPs_MakeFunction(name, args) NERPsFunctionSignature { nameof(name), NERPFunc__## name, (args) }

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Also used outside of NERPs scripting.
// <LegoRR.exe @00453b60>
//#define NERPFunc__SetGameSpeed ((NERPsFunction)0x00453b60)
sint32 __cdecl NERPFunc__SetGameSpeed(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00453cb0>
//#define NERPFunc__GetMessagesAreUpToDate ((NERPsFunction)0x00453cb0)
sint32 __cdecl NERPFunc__GetMessagesAreUpToDate(sint32* stack);

// <LegoRR.exe @00453cd0>
//#define NERPFunc__SupressArrow ((NERPsFunction)0x00453cd0)
sint32 __cdecl NERPFunc__SupressArrow(sint32* stack);

// <LegoRR.exe @00453d10>
//#define NERPFunc__AdvanceMessage ((NERPsFunction)0x00453d10)
sint32 __cdecl NERPFunc__AdvanceMessage(sint32* stack);

// <LegoRR.exe @00453d30>
//#define NERPFunc__AllowCameraMovement ((NERPsFunction)0x00453d30)
sint32 __cdecl NERPFunc__AllowCameraMovement(sint32* stack);

// <LegoRR.exe @00453d50>
//#define NERPFunc__ClickOnlyObjects ((NERPsFunction)0x00453d50)
sint32 __cdecl NERPFunc__ClickOnlyObjects(sint32* stack);

// <LegoRR.exe @00453d80>
//#define NERPFunc__ClickOnlyMap ((NERPsFunction)0x00453d80)
sint32 __cdecl NERPFunc__ClickOnlyMap(sint32* stack);

// <LegoRR.exe @00453db0>
//#define NERPFunc__ClickOnlyIcon ((NERPsFunction)0x00453db0)
sint32 __cdecl NERPFunc__ClickOnlyIcon(sint32* stack);

// <LegoRR.exe @00453de0>
//#define NERPFunc__ClickOnlyCalltoarms ((NERPsFunction)0x00453de0)
sint32 __cdecl NERPFunc__ClickOnlyCalltoarms(sint32* stack);

// <LegoRR.exe @00453e10>
//#define NERPFunc__DisallowAll ((NERPsFunction)0x00453e10)
sint32 __cdecl NERPFunc__DisallowAll(sint32* stack);

// <LegoRR.exe @00453e40>
//#define NERPFunc__FlashCallToArmsIcon ((NERPsFunction)0x00453e40)
sint32 __cdecl NERPFunc__FlashCallToArmsIcon(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00453f60>
//#define NERPFunc__GetTimer0 ((NERPsFunction)0x00453f60)
sint32 __cdecl NERPFunc__GetTimer0(sint32* stack);

// <LegoRR.exe @00453f70>
//#define NERPFunc__GetTimer1 ((NERPsFunction)0x00453f70)
sint32 __cdecl NERPFunc__GetTimer1(sint32* stack);

// <LegoRR.exe @00453f80>
//#define NERPFunc__GetTimer2 ((NERPsFunction)0x00453f80)
sint32 __cdecl NERPFunc__GetTimer2(sint32* stack);

// <LegoRR.exe @00453f90>
//#define NERPFunc__GetTimer3 ((NERPsFunction)0x00453f90)
sint32 __cdecl NERPFunc__GetTimer3(sint32* stack);

// <LegoRR.exe @00453fa0>
//#define NERPFunc__SetTimer0 ((NERPsFunction)0x00453fa0)
sint32 __cdecl NERPFunc__SetTimer0(sint32* stack);

// <LegoRR.exe @00453fd0>
//#define NERPFunc__SetTimer1 ((NERPsFunction)0x00453fd0)
sint32 __cdecl NERPFunc__SetTimer1(sint32* stack);

// <LegoRR.exe @00454000>
//#define NERPFunc__SetTimer2 ((NERPsFunction)0x00454000)
sint32 __cdecl NERPFunc__SetTimer2(sint32* stack);

// <LegoRR.exe @00454030>
//#define NERPFunc__SetTimer3 ((NERPsFunction)0x00454030)
sint32 __cdecl NERPFunc__SetTimer3(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004542a0>
//#define NERPFunc__CameraLockOnMonster ((NERPsFunction)0x004542a0)
sint32 __cdecl NERPFunc__CameraLockOnMonster(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454310>
//#define NERPFunc__CameraLockOnObject ((NERPsFunction)0x00454310)
sint32 __cdecl NERPFunc__CameraLockOnObject(sint32* stack);

// <LegoRR.exe @00454330>
//#define NERPFunc__CameraUnlock ((NERPsFunction)0x00454330)
sint32 __cdecl NERPFunc__CameraUnlock(sint32* stack);

// <LegoRR.exe @00454350>
//#define NERPFunc__CameraZoomIn ((NERPsFunction)0x00454350)
sint32 __cdecl NERPFunc__CameraZoomIn(sint32* stack);

// <LegoRR.exe @004543d0>
//#define NERPFunc__CameraZoomOut ((NERPsFunction)0x004543d0)
sint32 __cdecl NERPFunc__CameraZoomOut(sint32* stack);

// <LegoRR.exe @00454440>
//#define NERPFunc__CameraRotate ((NERPsFunction)0x00454440)
sint32 __cdecl NERPFunc__CameraRotate(sint32* stack);

// <LegoRR.exe @004544e0>
//#define NERPFunc__GetSelectedRecordObject ((NERPsFunction)0x004544e0)
sint32 __cdecl NERPFunc__GetSelectedRecordObject(sint32* stack);

// <LegoRR.exe @00454530>
//#define NERPFunc__SetCrystalPriority ((NERPsFunction)0x00454530)
sint32 __cdecl NERPFunc__SetCrystalPriority(sint32* stack);

// <LegoRR.exe @00454550>
//#define NERPFunc__SetMonsterAttackPowerstation ((NERPsFunction)0x00454550)
sint32 __cdecl NERPFunc__SetMonsterAttackPowerstation(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454660>
//#define NERPFunc__SetMonsterAttackNowt ((NERPsFunction)0x00454660)
sint32 __cdecl NERPFunc__SetMonsterAttackNowt(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004547f0>
//#define NERPFunc__GetRecordObjectAtTutorial ((NERPsFunction)0x004547f0)
sint32 __cdecl NERPFunc__GetRecordObjectAtTutorial(sint32* stack);

// <LegoRR.exe @00454860>
//#define NERPFunc__GetRecordObjectAmountAtTutorial ((NERPsFunction)0x00454860)
sint32 __cdecl NERPFunc__GetRecordObjectAmountAtTutorial(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004549c0>
//#define NERPFunc__SetRecordObjectPointer ((NERPsFunction)0x004549c0)
sint32 __cdecl NERPFunc__SetRecordObjectPointer(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004549e0>
//#define NERPFunc__GetOxygenLevel ((NERPsFunction)0x004549e0)
sint32 __cdecl NERPFunc__GetOxygenLevel(sint32* stack);

// <LegoRR.exe @00454a60>
//#define NERPFunc__GenerateSlug ((NERPsFunction)0x00454a60)
sint32 __cdecl NERPFunc__GenerateSlug(sint32* stack);

// <LegoRR.exe @00454aa0>
//#define NERPFunc__SetAttackDefer ((NERPsFunction)0x00454aa0)
sint32 __cdecl NERPFunc__SetAttackDefer(sint32* stack);

// <LegoRR.exe @00454ac0>
//#define NERPFunc__SetCallToArms ((NERPsFunction)0x00454ac0)
sint32 __cdecl NERPFunc__SetCallToArms(sint32* stack);

// <LegoRR.exe @00454ae0>
//#define NERPFunc__GetCallToArmsButtonClicked ((NERPsFunction)0x00454ae0)
sint32 __cdecl NERPFunc__GetCallToArmsButtonClicked(sint32* stack);

// <LegoRR.exe @00454af0>
//#define NERPFunc__SetRockMonster ((NERPsFunction)0x00454af0)
sint32 __cdecl NERPFunc__SetRockMonster(sint32* stack);

// <LegoRR.exe @00454b30>
//#define NERPFunc__GetRockMonstersDestroyed ((NERPsFunction)0x00454b30)
sint32 __cdecl NERPFunc__GetRockMonstersDestroyed(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454b60>
//#define NERPFunc__GetHiddenObjectsFound ((NERPsFunction)0x00454b60)
sint32 __cdecl NERPFunc__GetHiddenObjectsFound(sint32* stack);

// <LegoRR.exe @00454b70>
//#define NERPFunc__SetHiddenObjectsFound ((NERPsFunction)0x00454b70)
sint32 __cdecl NERPFunc__SetHiddenObjectsFound(sint32* stack);

// <LegoRR.exe @00454b80>
//#define NERPFunc__SetUpgradeBuildingIconClicked ((NERPsFunction)0x00454b80)
sint32 __cdecl NERPFunc__SetUpgradeBuildingIconClicked(sint32* stack);

// <LegoRR.exe @00454ba0>
//#define NERPFunc__GetUpgradeBuildingIconClicked ((NERPsFunction)0x00454ba0)
sint32 __cdecl NERPFunc__GetUpgradeBuildingIconClicked(sint32* stack);

// <LegoRR.exe @00454bc0>
//#define NERPFunc__FlashUpgradeBuildingIcon ((NERPsFunction)0x00454bc0)
sint32 __cdecl NERPFunc__FlashUpgradeBuildingIcon(sint32* stack);

// <LegoRR.exe @00454be0>
//#define NERPFunc__SetGoBackIconClicked ((NERPsFunction)0x00454be0)
sint32 __cdecl NERPFunc__SetGoBackIconClicked(sint32* stack);

// <LegoRR.exe @00454c00>
//#define NERPFunc__GetGoBackIconClicked ((NERPsFunction)0x00454c00)
sint32 __cdecl NERPFunc__GetGoBackIconClicked(sint32* stack);

// <LegoRR.exe @00454c20>
//#define NERPFunc__FlashGoBackIcon ((NERPsFunction)0x00454c20)
sint32 __cdecl NERPFunc__FlashGoBackIcon(sint32* stack);

// <LegoRR.exe @00454c40>
//#define NERPFunc__GetRockMonsterRunningAway ((NERPsFunction)0x00454c40)
sint32 __cdecl NERPFunc__GetRockMonsterRunningAway(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454d20>
//#define NERPFunc__SetRockMonsterPainThreshold ((NERPsFunction)0x00454d20)
sint32 __cdecl NERPFunc__SetRockMonsterPainThreshold(sint32* stack);

// <LegoRR.exe @00454d60>
//#define NERPFunc__SetRockMonsterHealth ((NERPsFunction)0x00454d60)
sint32 __cdecl NERPFunc__SetRockMonsterHealth(sint32* stack);

// <LegoRR.exe @00454da0>
//#define NERPFunc__SetPauseGame ((NERPsFunction)0x00454da0)
sint32 __cdecl NERPFunc__SetPauseGame(sint32* stack);

// <LegoRR.exe @00454dc0>
//#define NERPFunc__GetAnyKeyPressed ((NERPsFunction)0x00454dc0)
sint32 __cdecl NERPFunc__GetAnyKeyPressed(sint32* stack);

// <LegoRR.exe @00454dd0>
//#define NERPFunc__SetIconPos ((NERPsFunction)0x00454dd0)
sint32 __cdecl NERPFunc__SetIconPos(sint32* stack);

// <LegoRR.exe @00454e10>
//#define NERPFunc__SetIconSpace ((NERPsFunction)0x00454e10)
sint32 __cdecl NERPFunc__SetIconSpace(sint32* stack);

// <LegoRR.exe @00454e20>
//#define NERPFunc__SetIconWidth ((NERPsFunction)0x00454e20)
sint32 __cdecl NERPFunc__SetIconWidth(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @00454e30>
//#define NERPFunc__SetLevelCompleted ((NERPsFunction)0x00454e30)
sint32 __cdecl NERPFunc__SetLevelCompleted(sint32* stack);

// ALIAS: NERPFunc__SetLevelCompleted
// <LegoRR.exe @00454e30>
//#define NERPFunc__SetGameCompleted NERPFunc__SetLevelCompleted
sint32 __cdecl NERPFunc__SetGameCompleted(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @00454e40>
//#define NERPFunc__SetLevelFail ((NERPsFunction)0x00454e40)
sint32 __cdecl NERPFunc__SetLevelFail(sint32* stack);

// <LegoRR.exe @00454e60>
//#define NERPFunc__SetGameFail ((NERPsFunction)0x00454e60)
sint32 __cdecl NERPFunc__SetGameFail(sint32* stack);

// <LegoRR.exe @00454e70>
//#define NERPFunc__SetTutorialPointer ((NERPsFunction)0x00454e70)
sint32 __cdecl NERPFunc__SetTutorialPointer(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454f40>
//#define NERPFunc__SetTutorialBlockClicks ((NERPsFunction)0x00454f40)
sint32 __cdecl NERPFunc__SetTutorialBlockClicks(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00454fd0>
//#define NERPFunc__SetTutorialCrystals ((NERPsFunction)0x00454fd0)
sint32 __cdecl NERPFunc__SetTutorialCrystals(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00455050>
//#define NERPFunc__SetOreAtIconPositions ((NERPsFunction)0x00455050)
sint32 __cdecl NERPFunc__SetOreAtIconPositions(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004550d0>
//#define NERPFunc__GetMiniFigureSelected ((NERPsFunction)0x004550d0)
sint32 __cdecl NERPFunc__GetMiniFigureSelected(sint32* stack);

// <LegoRR.exe @004550e0>
//#define NERPFunc__GetSmallTruckSelected ((NERPsFunction)0x004550e0)
sint32 __cdecl NERPFunc__GetSmallTruckSelected(sint32* stack);

// <LegoRR.exe @00455140>
//#define NERPFunc__GetSmallDiggerSelected ((NERPsFunction)0x00455140)
sint32 __cdecl NERPFunc__GetSmallDiggerSelected(sint32* stack);

// <LegoRR.exe @004551a0>
//#define NERPFunc__GetRapidRiderSelected ((NERPsFunction)0x004551a0)
sint32 __cdecl NERPFunc__GetRapidRiderSelected(sint32* stack);

// <LegoRR.exe @00455200>
//#define NERPFunc__GetSmallHelicopterSelected ((NERPsFunction)0x00455200)
sint32 __cdecl NERPFunc__GetSmallHelicopterSelected(sint32* stack);

// <LegoRR.exe @00455260>
//#define NERPFunc__GetGraniteGrinderSelected ((NERPsFunction)0x00455260)
sint32 __cdecl NERPFunc__GetGraniteGrinderSelected(sint32* stack);

// <LegoRR.exe @004552c0>
//#define NERPFunc__GetChromeCrusherSelected ((NERPsFunction)0x004552c0)
sint32 __cdecl NERPFunc__GetChromeCrusherSelected(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00455390>
//#define NERPFunc__AddPoweredCrystals ((NERPsFunction)0x00455390)
sint32 __cdecl NERPFunc__AddPoweredCrystals(sint32* stack);

// <LegoRR.exe @004553c0>
//#define NERPFunc__AddStoredOre ((NERPsFunction)0x004553c0)
sint32 __cdecl NERPFunc__AddStoredOre(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00455420>
//#define NERPFunc__GetTutorialCrystals ((NERPsFunction)0x00455420)
sint32 __cdecl NERPFunc__GetTutorialCrystals(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00455480>
//#define NERPFunc__GetTutorialBlockClicks ((NERPsFunction)0x00455480)
sint32 __cdecl NERPFunc__GetTutorialBlockClicks(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00455680>
//#define NERPFunc__GetMiniFigureinGraniteGrinder ((NERPsFunction)0x00455680)
sint32 __cdecl NERPFunc__GetMiniFigureinGraniteGrinder(sint32* stack);

// <LegoRR.exe @00455690>
//#define NERPFunc__GetMiniFigureinChromeCrusher ((NERPsFunction)0x00455690)
sint32 __cdecl NERPFunc__GetMiniFigureinChromeCrusher(sint32* stack);

// <LegoRR.exe @004556a0>
//#define NERPFunc__GetMiniFigureinSmallDigger ((NERPsFunction)0x004556a0)
sint32 __cdecl NERPFunc__GetMiniFigureinSmallDigger(sint32* stack);

// <LegoRR.exe @004556b0>
//#define NERPFunc__GetMiniFigureinRapidRider ((NERPsFunction)0x004556b0)
sint32 __cdecl NERPFunc__GetMiniFigureinRapidRider(sint32* stack);

// <LegoRR.exe @004556c0>
//#define NERPFunc__GetMiniFigureinSmallTruck ((NERPsFunction)0x004556c0)
sint32 __cdecl NERPFunc__GetMiniFigureinSmallTruck(sint32* stack);

// <LegoRR.exe @004556d0>
//#define NERPFunc__GetMiniFigureinSmallHelicopter ((NERPsFunction)0x004556d0)
sint32 __cdecl NERPFunc__GetMiniFigureinSmallHelicopter(sint32* stack);

// <LegoRR.exe @004556e0>
//#define NERPFunc__SetBarracksLevel ((NERPsFunction)0x004556e0)
sint32 __cdecl NERPFunc__SetBarracksLevel(sint32* stack);

// <LegoRR.exe @00455700>
//#define NERPFunc__SetDocksLevel ((NERPsFunction)0x00455700)
sint32 __cdecl NERPFunc__SetDocksLevel(sint32* stack);

// <LegoRR.exe @00455720>
//#define NERPFunc__SetGeoDomeLevel ((NERPsFunction)0x00455720)
sint32 __cdecl NERPFunc__SetGeoDomeLevel(sint32* stack);

// <LegoRR.exe @00455740>
//#define NERPFunc__SetPowerstationLevel ((NERPsFunction)0x00455740)
sint32 __cdecl NERPFunc__SetPowerstationLevel(sint32* stack);

// <LegoRR.exe @00455760>
//#define NERPFunc__SetToolStoreLevel ((NERPsFunction)0x00455760)
sint32 __cdecl NERPFunc__SetToolStoreLevel(sint32* stack);

// <LegoRR.exe @00455780>
//#define NERPFunc__SetGunstationLevel ((NERPsFunction)0x00455780)
sint32 __cdecl NERPFunc__SetGunstationLevel(sint32* stack);

// <LegoRR.exe @004557a0>
//#define NERPFunc__SetTeleportPadLevel ((NERPsFunction)0x004557a0)
sint32 __cdecl NERPFunc__SetTeleportPadLevel(sint32* stack);

// <LegoRR.exe @004557c0>
//#define NERPFunc__SetSuperTeleportLevel ((NERPsFunction)0x004557c0)
sint32 __cdecl NERPFunc__SetSuperTeleportLevel(sint32* stack);

// <LegoRR.exe @004557e0>
//#define NERPFunc__SetUpgradeStationLevel ((NERPsFunction)0x004557e0)
sint32 __cdecl NERPFunc__SetUpgradeStationLevel(sint32* stack);

// <LegoRR.exe @00455800>
//#define NERPFunc__GetBarracksSelected ((NERPsFunction)0x00455800)
sint32 __cdecl NERPFunc__GetBarracksSelected(sint32* stack);

// <LegoRR.exe @00455810>
//#define NERPFunc__GetDocksSelected ((NERPsFunction)0x00455810)
sint32 __cdecl NERPFunc__GetDocksSelected(sint32* stack);

// <LegoRR.exe @00455820>
//#define NERPFunc__GetGeoDomeSelected ((NERPsFunction)0x00455820)
sint32 __cdecl NERPFunc__GetGeoDomeSelected(sint32* stack);

// <LegoRR.exe @00455830>
//#define NERPFunc__GetPowerstationSelected ((NERPsFunction)0x00455830)
sint32 __cdecl NERPFunc__GetPowerstationSelected(sint32* stack);

// <LegoRR.exe @00455840>
//#define NERPFunc__GetToolStoreSelected ((NERPsFunction)0x00455840)
sint32 __cdecl NERPFunc__GetToolStoreSelected(sint32* stack);

// <LegoRR.exe @00455850>
//#define NERPFunc__GetGunstationSelected ((NERPsFunction)0x00455850)
sint32 __cdecl NERPFunc__GetGunstationSelected(sint32* stack);

// <LegoRR.exe @00455860>
//#define NERPFunc__GetTeleportPadSelected ((NERPsFunction)0x00455860)
sint32 __cdecl NERPFunc__GetTeleportPadSelected(sint32* stack);

// <LegoRR.exe @00455870>
//#define NERPFunc__GetSuperTeleportSelected ((NERPsFunction)0x00455870)
sint32 __cdecl NERPFunc__GetSuperTeleportSelected(sint32* stack);

// <LegoRR.exe @00455880>
//#define NERPFunc__GetUpgradeStationSelected ((NERPsFunction)0x00455880)
sint32 __cdecl NERPFunc__GetUpgradeStationSelected(sint32* stack);

// <LegoRR.exe @00455890>
//#define NERPFunc__GetPoweredBarracksBuilt ((NERPsFunction)0x00455890)
sint32 __cdecl NERPFunc__GetPoweredBarracksBuilt(sint32* stack);

// <LegoRR.exe @004558a0>
//#define NERPFunc__GetPoweredDocksBuilt ((NERPsFunction)0x004558a0)
sint32 __cdecl NERPFunc__GetPoweredDocksBuilt(sint32* stack);

// <LegoRR.exe @004558b0>
//#define NERPFunc__GetPoweredGeodomeBuilt ((NERPsFunction)0x004558b0)
sint32 __cdecl NERPFunc__GetPoweredGeodomeBuilt(sint32* stack);

// <LegoRR.exe @004558c0>
//#define NERPFunc__GetPoweredPowerstationsBuilt ((NERPsFunction)0x004558c0)
sint32 __cdecl NERPFunc__GetPoweredPowerstationsBuilt(sint32* stack);

// <LegoRR.exe @004558d0>
//#define NERPFunc__GetPoweredToolStoresBuilt ((NERPsFunction)0x004558d0)
sint32 __cdecl NERPFunc__GetPoweredToolStoresBuilt(sint32* stack);

// <LegoRR.exe @004558e0>
//#define NERPFunc__GetPoweredGunstationsBuilt ((NERPsFunction)0x004558e0)
sint32 __cdecl NERPFunc__GetPoweredGunstationsBuilt(sint32* stack);

// <LegoRR.exe @004558f0>
//#define NERPFunc__GetPoweredTeleportsBuilt ((NERPsFunction)0x004558f0)
sint32 __cdecl NERPFunc__GetPoweredTeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455900>
//#define NERPFunc__GetPoweredVehicleTeleportsBuilt ((NERPsFunction)0x00455900)
sint32 __cdecl NERPFunc__GetPoweredVehicleTeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455910>
//#define NERPFunc__GetPoweredUpgradeStationsBuilt ((NERPsFunction)0x00455910)
sint32 __cdecl NERPFunc__GetPoweredUpgradeStationsBuilt(sint32* stack);

// <LegoRR.exe @00455920>
//#define NERPFunc__GetBarracksBuilt ((NERPsFunction)0x00455920)
sint32 __cdecl NERPFunc__GetBarracksBuilt(sint32* stack);

// <LegoRR.exe @00455930>
//#define NERPFunc__GetDocksBuilt ((NERPsFunction)0x00455930)
sint32 __cdecl NERPFunc__GetDocksBuilt(sint32* stack);

// <LegoRR.exe @00455940>
//#define NERPFunc__GetGeodomeBuilt ((NERPsFunction)0x00455940)
sint32 __cdecl NERPFunc__GetGeodomeBuilt(sint32* stack);

// <LegoRR.exe @00455950>
//#define NERPFunc__GetPowerstationsBuilt ((NERPsFunction)0x00455950)
sint32 __cdecl NERPFunc__GetPowerstationsBuilt(sint32* stack);

// <LegoRR.exe @00455960>
//#define NERPFunc__GetToolStoresBuilt ((NERPsFunction)0x00455960)
sint32 __cdecl NERPFunc__GetToolStoresBuilt(sint32* stack);

// <LegoRR.exe @00455970>
//#define NERPFunc__GetGunstationsBuilt ((NERPsFunction)0x00455970)
sint32 __cdecl NERPFunc__GetGunstationsBuilt(sint32* stack);

// <LegoRR.exe @00455980>
//#define NERPFunc__GetTeleportsBuilt ((NERPsFunction)0x00455980)
sint32 __cdecl NERPFunc__GetTeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455990>
//#define NERPFunc__GetVehicleTeleportsBuilt ((NERPsFunction)0x00455990)
sint32 __cdecl NERPFunc__GetVehicleTeleportsBuilt(sint32* stack);

// <LegoRR.exe @004559a0>
//#define NERPFunc__GetUpgradeStationsBuilt ((NERPsFunction)0x004559a0)
sint32 __cdecl NERPFunc__GetUpgradeStationsBuilt(sint32* stack);

// <LegoRR.exe @004559b0>
//#define NERPFunc__GetLevel1BarracksBuilt ((NERPsFunction)0x004559b0)
sint32 __cdecl NERPFunc__GetLevel1BarracksBuilt(sint32* stack);

// <LegoRR.exe @004559c0>
//#define NERPFunc__GetLevel1DocksBuilt ((NERPsFunction)0x004559c0)
sint32 __cdecl NERPFunc__GetLevel1DocksBuilt(sint32* stack);

// <LegoRR.exe @004559d0>
//#define NERPFunc__GetLevel1GeodomeBuilt ((NERPsFunction)0x004559d0)
sint32 __cdecl NERPFunc__GetLevel1GeodomeBuilt(sint32* stack);

// <LegoRR.exe @004559e0>
//#define NERPFunc__GetLevel1PowerstationsBuilt ((NERPsFunction)0x004559e0)
sint32 __cdecl NERPFunc__GetLevel1PowerstationsBuilt(sint32* stack);

// <LegoRR.exe @004559f0>
//#define NERPFunc__GetLevel1ToolStoresBuilt ((NERPsFunction)0x004559f0)
sint32 __cdecl NERPFunc__GetLevel1ToolStoresBuilt(sint32* stack);

// <LegoRR.exe @00455a00>
//#define NERPFunc__GetLevel1GunstationsBuilt ((NERPsFunction)0x00455a00)
sint32 __cdecl NERPFunc__GetLevel1GunstationsBuilt(sint32* stack);

// <LegoRR.exe @00455a10>
//#define NERPFunc__GetLevel1TeleportsBuilt ((NERPsFunction)0x00455a10)
sint32 __cdecl NERPFunc__GetLevel1TeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455a20>
//#define NERPFunc__GetLevel1VehicleTeleportsBuilt ((NERPsFunction)0x00455a20)
sint32 __cdecl NERPFunc__GetLevel1VehicleTeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455a30>
//#define NERPFunc__GetLevel1UpgradeStationsBuilt ((NERPsFunction)0x00455a30)
sint32 __cdecl NERPFunc__GetLevel1UpgradeStationsBuilt(sint32* stack);

// <LegoRR.exe @00455a40>
//#define NERPFunc__GetLevel2BarracksBuilt ((NERPsFunction)0x00455a40)
sint32 __cdecl NERPFunc__GetLevel2BarracksBuilt(sint32* stack);

// <LegoRR.exe @00455a50>
//#define NERPFunc__GetLevel2DocksBuilt ((NERPsFunction)0x00455a50)
sint32 __cdecl NERPFunc__GetLevel2DocksBuilt(sint32* stack);

// <LegoRR.exe @00455a60>
//#define NERPFunc__GetLevel2GeodomeBuilt ((NERPsFunction)0x00455a60)
sint32 __cdecl NERPFunc__GetLevel2GeodomeBuilt(sint32* stack);

// <LegoRR.exe @00455a70>
//#define NERPFunc__GetLevel2PowerstationsBuilt ((NERPsFunction)0x00455a70)
sint32 __cdecl NERPFunc__GetLevel2PowerstationsBuilt(sint32* stack);

// <LegoRR.exe @00455a80>
//#define NERPFunc__GetLevel2ToolStoresBuilt ((NERPsFunction)0x00455a80)
sint32 __cdecl NERPFunc__GetLevel2ToolStoresBuilt(sint32* stack);

// <LegoRR.exe @00455a90>
//#define NERPFunc__GetLevel2GunstationsBuilt ((NERPsFunction)0x00455a90)
sint32 __cdecl NERPFunc__GetLevel2GunstationsBuilt(sint32* stack);

// <LegoRR.exe @00455aa0>
//#define NERPFunc__GetLevel2TeleportsBuilt ((NERPsFunction)0x00455aa0)
sint32 __cdecl NERPFunc__GetLevel2TeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455ab0>
//#define NERPFunc__GetLevel2VehicleTeleportsBuilt ((NERPsFunction)0x00455ab0)
sint32 __cdecl NERPFunc__GetLevel2VehicleTeleportsBuilt(sint32* stack);

// <LegoRR.exe @00455ac0>
//#define NERPFunc__GetLevel2UpgradeStationsBuilt ((NERPsFunction)0x00455ac0)
sint32 __cdecl NERPFunc__GetLevel2UpgradeStationsBuilt(sint32* stack);

// <LegoRR.exe @00455ad0>
//#define NERPFunc__GetBarracksIconClicked ((NERPsFunction)0x00455ad0)
sint32 __cdecl NERPFunc__GetBarracksIconClicked(sint32* stack);

// <LegoRR.exe @00455ae0>
//#define NERPFunc__GetGeodomeIconClicked ((NERPsFunction)0x00455ae0)
sint32 __cdecl NERPFunc__GetGeodomeIconClicked(sint32* stack);

// <LegoRR.exe @00455af0>
//#define NERPFunc__GetPowerstationIconClicked ((NERPsFunction)0x00455af0)
sint32 __cdecl NERPFunc__GetPowerstationIconClicked(sint32* stack);

// <LegoRR.exe @00455b00>
//#define NERPFunc__GetToolStoreIconClicked ((NERPsFunction)0x00455b00)
sint32 __cdecl NERPFunc__GetToolStoreIconClicked(sint32* stack);

// <LegoRR.exe @00455b10>
//#define NERPFunc__GetGunstationIconClicked ((NERPsFunction)0x00455b10)
sint32 __cdecl NERPFunc__GetGunstationIconClicked(sint32* stack);

// <LegoRR.exe @00455b20>
//#define NERPFunc__GetTeleportPadIconClicked ((NERPsFunction)0x00455b20)
sint32 __cdecl NERPFunc__GetTeleportPadIconClicked(sint32* stack);

// <LegoRR.exe @00455b30>
//#define NERPFunc__GetVehicleTransportIconClicked ((NERPsFunction)0x00455b30)
sint32 __cdecl NERPFunc__GetVehicleTransportIconClicked(sint32* stack);

// <LegoRR.exe @00455b40>
//#define NERPFunc__GetUpgradeStationIconClicked ((NERPsFunction)0x00455b40)
sint32 __cdecl NERPFunc__GetUpgradeStationIconClicked(sint32* stack);

// <LegoRR.exe @00455b50>
//#define NERPFunc__SetBarracksIconClicked ((NERPsFunction)0x00455b50)
sint32 __cdecl NERPFunc__SetBarracksIconClicked(sint32* stack);

// <LegoRR.exe @00455b70>
//#define NERPFunc__SetGeodomeIconClicked ((NERPsFunction)0x00455b70)
sint32 __cdecl NERPFunc__SetGeodomeIconClicked(sint32* stack);

// <LegoRR.exe @00455b90>
//#define NERPFunc__SetPowerstationIconClicked ((NERPsFunction)0x00455b90)
sint32 __cdecl NERPFunc__SetPowerstationIconClicked(sint32* stack);

// <LegoRR.exe @00455bb0>
//#define NERPFunc__SetToolStoreIconClicked ((NERPsFunction)0x00455bb0)
sint32 __cdecl NERPFunc__SetToolStoreIconClicked(sint32* stack);

// <LegoRR.exe @00455bd0>
//#define NERPFunc__SetGunstationIconClicked ((NERPsFunction)0x00455bd0)
sint32 __cdecl NERPFunc__SetGunstationIconClicked(sint32* stack);

// <LegoRR.exe @00455bf0>
//#define NERPFunc__SetTeleportPadIconClicked ((NERPsFunction)0x00455bf0)
sint32 __cdecl NERPFunc__SetTeleportPadIconClicked(sint32* stack);

// <LegoRR.exe @00455c10>
//#define NERPFunc__SetVehicleTransportIconClicked ((NERPsFunction)0x00455c10)
sint32 __cdecl NERPFunc__SetVehicleTransportIconClicked(sint32* stack);

// <LegoRR.exe @00455c30>
//#define NERPFunc__SetUpgradeStationIconClicked ((NERPsFunction)0x00455c30)
sint32 __cdecl NERPFunc__SetUpgradeStationIconClicked(sint32* stack);

// <LegoRR.exe @00455c50>
//#define NERPFunc__FlashBarracksIcon ((NERPsFunction)0x00455c50)
sint32 __cdecl NERPFunc__FlashBarracksIcon(sint32* stack);

// <LegoRR.exe @00455c70>
//#define NERPFunc__FlashGeodomeIcon ((NERPsFunction)0x00455c70)
sint32 __cdecl NERPFunc__FlashGeodomeIcon(sint32* stack);

// <LegoRR.exe @00455c90>
//#define NERPFunc__FlashPowerStationIcon ((NERPsFunction)0x00455c90)
sint32 __cdecl NERPFunc__FlashPowerStationIcon(sint32* stack);

// <LegoRR.exe @00455cb0>
//#define NERPFunc__FlashToolStoreIcon ((NERPsFunction)0x00455cb0)
sint32 __cdecl NERPFunc__FlashToolStoreIcon(sint32* stack);

// <LegoRR.exe @00455cd0>
//#define NERPFunc__FlashGunstationIcon ((NERPsFunction)0x00455cd0)
sint32 __cdecl NERPFunc__FlashGunstationIcon(sint32* stack);

// <LegoRR.exe @00455cf0>
//#define NERPFunc__FlashTeleportPadIcon ((NERPsFunction)0x00455cf0)
sint32 __cdecl NERPFunc__FlashTeleportPadIcon(sint32* stack);

// <LegoRR.exe @00455d10>
//#define NERPFunc__FlashVehicleTransportIcon ((NERPsFunction)0x00455d10)
sint32 __cdecl NERPFunc__FlashVehicleTransportIcon(sint32* stack);

// <LegoRR.exe @00455d30>
//#define NERPFunc__FlashUpgradeStationIcon ((NERPsFunction)0x00455d30)
sint32 __cdecl NERPFunc__FlashUpgradeStationIcon(sint32* stack);

// <LegoRR.exe @00455d50>
//#define NERPFunc__GetPathsBuilt ((NERPsFunction)0x00455d50)
sint32 __cdecl NERPFunc__GetPathsBuilt(sint32* stack);

// <LegoRR.exe @00455d60>
//#define NERPFunc__GetStudCount ((NERPsFunction)0x00455d60)
sint32 __cdecl NERPFunc__GetStudCount(sint32* stack);

// <LegoRR.exe @00455d70>
//#define NERPFunc__GetSmallHelicoptersOnLevel ((NERPsFunction)0x00455d70)
sint32 __cdecl NERPFunc__GetSmallHelicoptersOnLevel(sint32* stack);

// <LegoRR.exe @00455d80>
//#define NERPFunc__GetGraniteGrindersOnLevel ((NERPsFunction)0x00455d80)
sint32 __cdecl NERPFunc__GetGraniteGrindersOnLevel(sint32* stack);

// <LegoRR.exe @00455d90>
//#define NERPFunc__GetRapidRidersOnLevel ((NERPsFunction)0x00455d90)
sint32 __cdecl NERPFunc__GetRapidRidersOnLevel(sint32* stack);

// <LegoRR.exe @00455da0>
//#define NERPFunc__GetSmallDiggersOnLevel ((NERPsFunction)0x00455da0)
sint32 __cdecl NERPFunc__GetSmallDiggersOnLevel(sint32* stack);

// <LegoRR.exe @00455db0>
//#define NERPFunc__GetSlugsOnLevel ((NERPsFunction)0x00455db0)
sint32 __cdecl NERPFunc__GetSlugsOnLevel(sint32* stack);

// <LegoRR.exe @00455dc0>
//#define NERPFunc__GetMiniFiguresOnLevel ((NERPsFunction)0x00455dc0)
sint32 __cdecl NERPFunc__GetMiniFiguresOnLevel(sint32* stack);

// <LegoRR.exe @00455dd0>
//#define NERPFunc__GetOreRefineriesBuilt ((NERPsFunction)0x00455dd0)
sint32 __cdecl NERPFunc__GetOreRefineriesBuilt(sint32* stack);

// <LegoRR.exe @00455de0>
//#define NERPFunc__GetCrystalRefineriesBuilt ((NERPsFunction)0x00455de0)
sint32 __cdecl NERPFunc__GetCrystalRefineriesBuilt(sint32* stack);

// <LegoRR.exe @00455df0>
//#define NERPFunc__GetTeleportIconClicked ((NERPsFunction)0x00455df0)
sint32 __cdecl NERPFunc__GetTeleportIconClicked(sint32* stack);

// <LegoRR.exe @00455e10>
//#define NERPFunc__GetDynamiteClicked ((NERPsFunction)0x00455e10)
sint32 __cdecl NERPFunc__GetDynamiteClicked(sint32* stack);

// <LegoRR.exe @00455e30>
//#define NERPFunc__GetMountIconClicked ((NERPsFunction)0x00455e30)
sint32 __cdecl NERPFunc__GetMountIconClicked(sint32* stack);

// <LegoRR.exe @00455e50>
//#define NERPFunc__GetTrainIconClicked ((NERPsFunction)0x00455e50)
sint32 __cdecl NERPFunc__GetTrainIconClicked(sint32* stack);

// <LegoRR.exe @00455e70>
//#define NERPFunc__GetDropSonicBlasterIconClicked ((NERPsFunction)0x00455e70)
sint32 __cdecl NERPFunc__GetDropSonicBlasterIconClicked(sint32* stack);

// <LegoRR.exe @00455e90>
//#define NERPFunc__GetGetToolIconClicked ((NERPsFunction)0x00455e90)
sint32 __cdecl NERPFunc__GetGetToolIconClicked(sint32* stack);

// <LegoRR.exe @00455eb0>
//#define NERPFunc__GetGetPusherIconClicked ((NERPsFunction)0x00455eb0)
sint32 __cdecl NERPFunc__GetGetPusherIconClicked(sint32* stack);

// <LegoRR.exe @00455ed0>
//#define NERPFunc__GetGetSonicBlasterIconClicked ((NERPsFunction)0x00455ed0)
sint32 __cdecl NERPFunc__GetGetSonicBlasterIconClicked(sint32* stack);

// <LegoRR.exe @00455ef0>
//#define NERPFunc__GetTrainSailorIconClicked ((NERPsFunction)0x00455ef0)
sint32 __cdecl NERPFunc__GetTrainSailorIconClicked(sint32* stack);

// <LegoRR.exe @00455f10>
//#define NERPFunc__GetTrainPilotIconClicked ((NERPsFunction)0x00455f10)
sint32 __cdecl NERPFunc__GetTrainPilotIconClicked(sint32* stack);

// <LegoRR.exe @00455f30>
//#define NERPFunc__GetTrainDriverIconClicked ((NERPsFunction)0x00455f30)
sint32 __cdecl NERPFunc__GetTrainDriverIconClicked(sint32* stack);

// <LegoRR.exe @00455f50>
//#define NERPFunc__GetGetLaserIconClicked ((NERPsFunction)0x00455f50)
sint32 __cdecl NERPFunc__GetGetLaserIconClicked(sint32* stack);

// <LegoRR.exe @00455f70>
//#define NERPFunc__GetDismountIconClicked ((NERPsFunction)0x00455f70)
sint32 __cdecl NERPFunc__GetDismountIconClicked(sint32* stack);

// <LegoRR.exe @00455f90>
//#define NERPFunc__GetDigIconClicked ((NERPsFunction)0x00455f90)
sint32 __cdecl NERPFunc__GetDigIconClicked(sint32* stack);

// <LegoRR.exe @00455fb0>
//#define NERPFunc__GetBuildIconClicked ((NERPsFunction)0x00455fb0)
sint32 __cdecl NERPFunc__GetBuildIconClicked(sint32* stack);

// <LegoRR.exe @00455fd0>
//#define NERPFunc__GetLayPathIconClicked ((NERPsFunction)0x00455fd0)
sint32 __cdecl NERPFunc__GetLayPathIconClicked(sint32* stack);

// <LegoRR.exe @00455ff0>
//#define NERPFunc__GetPlaceFenceIconClicked ((NERPsFunction)0x00455ff0)
sint32 __cdecl NERPFunc__GetPlaceFenceIconClicked(sint32* stack);

// <LegoRR.exe @00456010>
//#define NERPFunc__SetTeleportIconClicked ((NERPsFunction)0x00456010)
sint32 __cdecl NERPFunc__SetTeleportIconClicked(sint32* stack);

// <LegoRR.exe @00456030>
//#define NERPFunc__SetDynamiteClicked ((NERPsFunction)0x00456030)
sint32 __cdecl NERPFunc__SetDynamiteClicked(sint32* stack);

// <LegoRR.exe @00456050>
//#define NERPFunc__SetTrainIconClicked ((NERPsFunction)0x00456050)
sint32 __cdecl NERPFunc__SetTrainIconClicked(sint32* stack);

// <LegoRR.exe @00456070>
//#define NERPFunc__SetTrainDriverIconClicked ((NERPsFunction)0x00456070)
sint32 __cdecl NERPFunc__SetTrainDriverIconClicked(sint32* stack);

// <LegoRR.exe @00456090>
//#define NERPFunc__SetTrainSailorIconClicked ((NERPsFunction)0x00456090)
sint32 __cdecl NERPFunc__SetTrainSailorIconClicked(sint32* stack);

// <LegoRR.exe @004560b0>
//#define NERPFunc__SetGetToolIconClicked ((NERPsFunction)0x004560b0)
sint32 __cdecl NERPFunc__SetGetToolIconClicked(sint32* stack);

// <LegoRR.exe @004560d0>
//#define NERPFunc__SetDropSonicBlasterIconClicked ((NERPsFunction)0x004560d0)
sint32 __cdecl NERPFunc__SetDropSonicBlasterIconClicked(sint32* stack);

// <LegoRR.exe @004560f0>
//#define NERPFunc__SetGetLaserIconClicked ((NERPsFunction)0x004560f0)
sint32 __cdecl NERPFunc__SetGetLaserIconClicked(sint32* stack);

// <LegoRR.exe @00456110>
//#define NERPFunc__SetGetPusherIconClicked ((NERPsFunction)0x00456110)
sint32 __cdecl NERPFunc__SetGetPusherIconClicked(sint32* stack);

// <LegoRR.exe @00456130>
//#define NERPFunc__SetGetSonicBlasterIconClicked ((NERPsFunction)0x00456130)
sint32 __cdecl NERPFunc__SetGetSonicBlasterIconClicked(sint32* stack);

// <LegoRR.exe @00456150>
//#define NERPFunc__SetDismountIconClicked ((NERPsFunction)0x00456150)
sint32 __cdecl NERPFunc__SetDismountIconClicked(sint32* stack);

// <LegoRR.exe @00456170>
//#define NERPFunc__SetTrainPilotIconClicked ((NERPsFunction)0x00456170)
sint32 __cdecl NERPFunc__SetTrainPilotIconClicked(sint32* stack);

// <LegoRR.exe @00456190>
//#define NERPFunc__SetMountIconClicked ((NERPsFunction)0x00456190)
sint32 __cdecl NERPFunc__SetMountIconClicked(sint32* stack);

// <LegoRR.exe @004561b0>
//#define NERPFunc__SetDigIconClicked ((NERPsFunction)0x004561b0)
sint32 __cdecl NERPFunc__SetDigIconClicked(sint32* stack);

// <LegoRR.exe @004561d0>
//#define NERPFunc__SetBuildIconClicked ((NERPsFunction)0x004561d0)
sint32 __cdecl NERPFunc__SetBuildIconClicked(sint32* stack);

// <LegoRR.exe @004561f0>
//#define NERPFunc__SetLayPathIconClicked ((NERPsFunction)0x004561f0)
sint32 __cdecl NERPFunc__SetLayPathIconClicked(sint32* stack);

// <LegoRR.exe @00456210>
//#define NERPFunc__SetPlaceFenceIconClicked ((NERPsFunction)0x00456210)
sint32 __cdecl NERPFunc__SetPlaceFenceIconClicked(sint32* stack);

// <LegoRR.exe @00456230>
//#define NERPFunc__FlashTeleportIcon ((NERPsFunction)0x00456230)
sint32 __cdecl NERPFunc__FlashTeleportIcon(sint32* stack);

// <LegoRR.exe @00456250>
//#define NERPFunc__FlashDynamiteIcon ((NERPsFunction)0x00456250)
sint32 __cdecl NERPFunc__FlashDynamiteIcon(sint32* stack);

// <LegoRR.exe @00456270>
//#define NERPFunc__FlashMountIcon ((NERPsFunction)0x00456270)
sint32 __cdecl NERPFunc__FlashMountIcon(sint32* stack);

// <LegoRR.exe @00456290>
//#define NERPFunc__FlashTrainIcon ((NERPsFunction)0x00456290)
sint32 __cdecl NERPFunc__FlashTrainIcon(sint32* stack);

// <LegoRR.exe @004562b0>
//#define NERPFunc__FlashTrainDriverIcon ((NERPsFunction)0x004562b0)
sint32 __cdecl NERPFunc__FlashTrainDriverIcon(sint32* stack);

// <LegoRR.exe @004562d0>
//#define NERPFunc__FlashTrainPilotIcon ((NERPsFunction)0x004562d0)
sint32 __cdecl NERPFunc__FlashTrainPilotIcon(sint32* stack);

// <LegoRR.exe @004562f0>
//#define NERPFunc__FlashTrainSailorIcon ((NERPsFunction)0x004562f0)
sint32 __cdecl NERPFunc__FlashTrainSailorIcon(sint32* stack);

// <LegoRR.exe @00456310>
//#define NERPFunc__FlashDismountIcon ((NERPsFunction)0x00456310)
sint32 __cdecl NERPFunc__FlashDismountIcon(sint32* stack);

// <LegoRR.exe @00456330>
//#define NERPFunc__FlashGetToolIcon ((NERPsFunction)0x00456330)
sint32 __cdecl NERPFunc__FlashGetToolIcon(sint32* stack);

// <LegoRR.exe @00456350>
//#define NERPFunc__FlashDropSonicBlasterIcon ((NERPsFunction)0x00456350)
sint32 __cdecl NERPFunc__FlashDropSonicBlasterIcon(sint32* stack);

// <LegoRR.exe @00456370>
//#define NERPFunc__FlashGetLaserIcon ((NERPsFunction)0x00456370)
sint32 __cdecl NERPFunc__FlashGetLaserIcon(sint32* stack);

// <LegoRR.exe @00456390>
//#define NERPFunc__FlashGetPusherIcon ((NERPsFunction)0x00456390)
sint32 __cdecl NERPFunc__FlashGetPusherIcon(sint32* stack);

// <LegoRR.exe @004563b0>
//#define NERPFunc__FlashGetSonicBlasterIcon ((NERPsFunction)0x004563b0)
sint32 __cdecl NERPFunc__FlashGetSonicBlasterIcon(sint32* stack);

// <LegoRR.exe @004563d0>
//#define NERPFunc__FlashDigIcon ((NERPsFunction)0x004563d0)
sint32 __cdecl NERPFunc__FlashDigIcon(sint32* stack);

// <LegoRR.exe @004563f0>
//#define NERPFunc__FlashBuildIcon ((NERPsFunction)0x004563f0)
sint32 __cdecl NERPFunc__FlashBuildIcon(sint32* stack);

// <LegoRR.exe @00456410>
//#define NERPFunc__FlashLayPathIcon ((NERPsFunction)0x00456410)
sint32 __cdecl NERPFunc__FlashLayPathIcon(sint32* stack);

// <LegoRR.exe @00456430>
//#define NERPFunc__FlashPlaceFenceIcon ((NERPsFunction)0x00456430)
sint32 __cdecl NERPFunc__FlashPlaceFenceIcon(sint32* stack);

// <LegoRR.exe @00456450>
//#define NERPFunc__GetRandom ((NERPsFunction)0x00456450)
sint32 __cdecl NERPFunc__GetRandom(sint32* stack);

// <LegoRR.exe @00456460>
//#define NERPFunc__GetRandomTrueFalse ((NERPsFunction)0x00456460)
sint32 __cdecl NERPFunc__GetRandomTrueFalse(sint32* stack);

// <LegoRR.exe @00456470>
//#define NERPFunc__GetRandom10 ((NERPsFunction)0x00456470)
sint32 __cdecl NERPFunc__GetRandom10(sint32* stack);

// <LegoRR.exe @00456490>
//#define NERPFunc__GetRandom100 ((NERPsFunction)0x00456490)
sint32 __cdecl NERPFunc__GetRandom100(sint32* stack);

// <LegoRR.exe @004564b0>
//#define NERPFunc__GetCrystalsPickedUp ((NERPsFunction)0x004564b0)
sint32 __cdecl NERPFunc__GetCrystalsPickedUp(sint32* stack);

// <LegoRR.exe @004564c0>
//#define NERPFunc__GetCrystalsCurrentlyStored ((NERPsFunction)0x004564c0)
sint32 __cdecl NERPFunc__GetCrystalsCurrentlyStored(sint32* stack);


// MERGED FUNCTION
// <LegoRR.exe @004564d0>
//#define NERPFunc__False ((NERPsFunction)0x004564d0)
sint32 __cdecl NERPFunc__False(sint32* stack);

// ALIAS: NERPFunc__False
// <LegoRR.exe @004564d0>
//#define NERPFunc__Null NERPFunc__False
sint32 __cdecl NERPFunc__Null(sint32* stack);

// ALIAS: NERPFunc__False (function behaviour is hardcoded, and is never actually called)
// <LegoRR.exe @004564d0>
//#define NERPFunc__Stop NERPFunc__False
sint32 __cdecl NERPFunc__Stop(sint32* stack);

// ALIAS: NERPFunc__False (dummy function used at the end-of-list entry for NERPs functions)
// <LegoRR.exe @004564d0>
//#define NERPFunc__End_Of_List NERPFunc__False
sint32 __cdecl NERPFunc__End_Of_List(sint32* stack);

// ALIAS: NERPFunc__False (function is unimplemented)
// <LegoRR.exe @004564d0>
//#define NERPFunc__GetCrystalsUsed NERPFunc__False
sint32 __cdecl NERPFunc__GetCrystalsUsed(sint32* stack);

// ALIAS: NERPFunc__False (function is unimplemented)
// <LegoRR.exe @004564d0>
//#define NERPFunc__GetCrystalsStolen NERPFunc__False
sint32 __cdecl NERPFunc__GetCrystalsStolen(sint32* stack);

// ALIAS: NERPFunc__False (function is unimplemented)
// <LegoRR.exe @004564d0>
//#define NERPFunc__GetOreUsed NERPFunc__False
sint32 __cdecl NERPFunc__GetOreUsed(sint32* stack);

// ALIAS: NERPFunc__False (function is unimplemented)
// <LegoRR.exe @004564d0>
//#define NERPFunc__GetOreStolen NERPFunc__False
sint32 __cdecl NERPFunc__GetOreStolen(sint32* stack);


// <LegoRR.exe @004564e0>
//#define NERPFunc__GetOrePickedUp ((NERPsFunction)0x004564e0)
sint32 __cdecl NERPFunc__GetOrePickedUp(sint32* stack);

// <LegoRR.exe @004564f0>
//#define NERPFunc__GetOreCurrentlyStored ((NERPsFunction)0x004564f0)
sint32 __cdecl NERPFunc__GetOreCurrentlyStored(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @00456500>
//#define NERPFunc__GetTutorialFlags ((TutorialFlags (__cdecl* )(sint32* stack))0x00456500)
sint32 __cdecl NERPFunc__GetTutorialFlags(sint32* stack);

// <LegoRR.exe @00456510>
//#define NERPFunc__GetR0 ((NERPsFunction)0x00456510)
sint32 __cdecl NERPFunc__GetR0(sint32* stack);

// <LegoRR.exe @00456520>
//#define NERPFunc__GetR1 ((NERPsFunction)0x00456520)
sint32 __cdecl NERPFunc__GetR1(sint32* stack);

// <LegoRR.exe @00456530>
//#define NERPFunc__GetR2 ((NERPsFunction)0x00456530)
sint32 __cdecl NERPFunc__GetR2(sint32* stack);

// <LegoRR.exe @00456540>
//#define NERPFunc__GetR3 ((NERPsFunction)0x00456540)
sint32 __cdecl NERPFunc__GetR3(sint32* stack);

// <LegoRR.exe @00456550>
//#define NERPFunc__GetR4 ((NERPsFunction)0x00456550)
sint32 __cdecl NERPFunc__GetR4(sint32* stack);

// <LegoRR.exe @00456560>
//#define NERPFunc__GetR5 ((NERPsFunction)0x00456560)
sint32 __cdecl NERPFunc__GetR5(sint32* stack);

// <LegoRR.exe @00456570>
//#define NERPFunc__GetR6 ((NERPsFunction)0x00456570)
sint32 __cdecl NERPFunc__GetR6(sint32* stack);

// <LegoRR.exe @00456580>
//#define NERPFunc__GetR7 ((NERPsFunction)0x00456580)
sint32 __cdecl NERPFunc__GetR7(sint32* stack);

// <LegoRR.exe @00456590>
//#define NERPFunc__AddR0 ((NERPsFunction)0x00456590)
sint32 __cdecl NERPFunc__AddR0(sint32* stack);

// <LegoRR.exe @004565b0>
//#define NERPFunc__AddR1 ((NERPsFunction)0x004565b0)
sint32 __cdecl NERPFunc__AddR1(sint32* stack);

// <LegoRR.exe @004565d0>
//#define NERPFunc__AddR2 ((NERPsFunction)0x004565d0)
sint32 __cdecl NERPFunc__AddR2(sint32* stack);

// <LegoRR.exe @004565f0>
//#define NERPFunc__AddR3 ((NERPsFunction)0x004565f0)
sint32 __cdecl NERPFunc__AddR3(sint32* stack);

// <LegoRR.exe @00456610>
//#define NERPFunc__AddR4 ((NERPsFunction)0x00456610)
sint32 __cdecl NERPFunc__AddR4(sint32* stack);

// <LegoRR.exe @00456630>
//#define NERPFunc__AddR5 ((NERPsFunction)0x00456630)
sint32 __cdecl NERPFunc__AddR5(sint32* stack);

// <LegoRR.exe @00456650>
//#define NERPFunc__AddR6 ((NERPsFunction)0x00456650)
sint32 __cdecl NERPFunc__AddR6(sint32* stack);

// <LegoRR.exe @00456670>
//#define NERPFunc__AddR7 ((NERPsFunction)0x00456670)
sint32 __cdecl NERPFunc__AddR7(sint32* stack);

// <LegoRR.exe @00456690>
//#define NERPFunc__SubR0 ((NERPsFunction)0x00456690)
sint32 __cdecl NERPFunc__SubR0(sint32* stack);

// <LegoRR.exe @004566b0>
//#define NERPFunc__SubR1 ((NERPsFunction)0x004566b0)
sint32 __cdecl NERPFunc__SubR1(sint32* stack);

// <LegoRR.exe @004566d0>
//#define NERPFunc__SubR2 ((NERPsFunction)0x004566d0)
sint32 __cdecl NERPFunc__SubR2(sint32* stack);

// <LegoRR.exe @004566f0>
//#define NERPFunc__SubR3 ((NERPsFunction)0x004566f0)
sint32 __cdecl NERPFunc__SubR3(sint32* stack);

// <LegoRR.exe @00456710>
//#define NERPFunc__SubR4 ((NERPsFunction)0x00456710)
sint32 __cdecl NERPFunc__SubR4(sint32* stack);

// <LegoRR.exe @00456730>
//#define NERPFunc__SubR5 ((NERPsFunction)0x00456730)
sint32 __cdecl NERPFunc__SubR5(sint32* stack);

// <LegoRR.exe @00456750>
//#define NERPFunc__SubR6 ((NERPsFunction)0x00456750)
sint32 __cdecl NERPFunc__SubR6(sint32* stack);

// <LegoRR.exe @00456770>
//#define NERPFunc__SubR7 ((NERPsFunction)0x00456770)
sint32 __cdecl NERPFunc__SubR7(sint32* stack);

// <LegoRR.exe @00456790>
//#define NERPFunc__SetR0 ((NERPsFunction)0x00456790)
sint32 __cdecl NERPFunc__SetR0(sint32* stack);

// <LegoRR.exe @004567a0>
//#define NERPFunc__SetR1 ((NERPsFunction)0x004567a0)
sint32 __cdecl NERPFunc__SetR1(sint32* stack);

// <LegoRR.exe @004567b0>
//#define NERPFunc__SetR2 ((NERPsFunction)0x004567b0)
sint32 __cdecl NERPFunc__SetR2(sint32* stack);

// <LegoRR.exe @004567c0>
//#define NERPFunc__SetR3 ((NERPsFunction)0x004567c0)
sint32 __cdecl NERPFunc__SetR3(sint32* stack);

// <LegoRR.exe @004567d0>
//#define NERPFunc__SetR4 ((NERPsFunction)0x004567d0)
sint32 __cdecl NERPFunc__SetR4(sint32* stack);

// <LegoRR.exe @004567e0>
//#define NERPFunc__SetR5 ((NERPsFunction)0x004567e0)
sint32 __cdecl NERPFunc__SetR5(sint32* stack);

// <LegoRR.exe @004567f0>
//#define NERPFunc__SetR6 ((NERPsFunction)0x004567f0)
sint32 __cdecl NERPFunc__SetR6(sint32* stack);

// <LegoRR.exe @00456800>
//#define NERPFunc__SetR7 ((NERPsFunction)0x00456800)
sint32 __cdecl NERPFunc__SetR7(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @00456810>
//#define NERPFunc__SetTutorialFlags ((NERPsFunction)0x00456810)
sint32 __cdecl NERPFunc__SetTutorialFlags(sint32* stack);

// <LegoRR.exe @00456820>
//#define NERPFunc__GetTrainFlags ((NERPsFunction)0x00456820)
sint32 __cdecl NERPFunc__GetTrainFlags(sint32* stack);

// <LegoRR.exe @00456830>
//#define NERPFunc__SetTrainFlags ((NERPsFunction)0x00456830)
sint32 __cdecl NERPFunc__SetTrainFlags(sint32* stack);

// <LegoRR.exe @00456840>
//#define NERPFunc__GetMonstersOnLevel ((NERPsFunction)0x00456840)
sint32 __cdecl NERPFunc__GetMonstersOnLevel(sint32* stack);

// <LegoRR.exe @00456880>
//#define NERPFunc__GetBuildingsTeleported ((NERPsFunction)0x00456880)
sint32 __cdecl NERPFunc__GetBuildingsTeleported(sint32* stack);

// <LegoRR.exe @00456890>
//#define NERPFunc__SetBuildingsTeleported ((NERPsFunction)0x00456890)
sint32 __cdecl NERPFunc__SetBuildingsTeleported(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @004568b0>
//#define NERPFunc__SetMessagePermit ((NERPsFunction)0x004568b0)
sint32 __cdecl NERPFunc__SetMessagePermit(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00456920>
//#define NERPFunc__SetMessageWait ((NERPsFunction)0x00456920)
sint32 __cdecl NERPFunc__SetMessageWait(sint32* stack);

// <LegoRR.exe @00456930>
//#define NERPFunc__SetMessageTimerValues ((NERPsFunction)0x00456930)
sint32 __cdecl NERPFunc__SetMessageTimerValues(sint32* stack);

// <LegoRR.exe @00456980>
//#define NERPFunc__GetMessageTimer ((NERPsFunction)0x00456980)
sint32 __cdecl NERPFunc__GetMessageTimer(sint32* stack);

// <LegoRR.exe @00456990>
//#define NERPFunc__SetMessage ((NERPsFunction)0x00456990)
sint32 __cdecl NERPFunc__SetMessage(sint32* stack);

// Also used outside of NERPs scripting.
// <LegoRR.exe @00456a80>
//#define NERPFunc__SetObjectiveSwitch ((NERPsFunction)0x00456a80)
sint32 __cdecl NERPFunc__SetObjectiveSwitch(sint32* stack);

// <LegoRR.exe @00456a90>
//#define NERPFunc__GetObjectiveSwitch ((NERPsFunction)0x00456a90)
sint32 __cdecl NERPFunc__GetObjectiveSwitch(sint32* stack);

// <LegoRR.exe @00456ab0>
//#define NERPFunc__GetObjectiveShowing ((NERPsFunction)0x00456ab0)
sint32 __cdecl NERPFunc__GetObjectiveShowing(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @004573f0>
//#define NERPFunc__MakeSomeoneOnThisBlockPickUpSomethingOnThisBlock ((NERPsFunction)0x004573f0)
sint32 __cdecl NERPFunc__MakeSomeoneOnThisBlockPickUpSomethingOnThisBlock(sint32* stack);


// [NERPsRuntime...]


// <LegoRR.exe @00457520>
//#define NERPFunc__SetCongregationAtTutorial ((NERPsFunction)0x00457520)
sint32 __cdecl NERPFunc__SetCongregationAtTutorial(sint32* stack);

// <LegoRR.exe @00457560>
//#define NERPFunc__SetRockMonsterAtTutorial ((NERPsFunction)0x00457560)
sint32 __cdecl NERPFunc__SetRockMonsterAtTutorial(sint32* stack);

// <LegoRR.exe @004575a0>
//#define NERPFunc__SetCameraGotoTutorial ((NERPsFunction)0x004575a0)
sint32 __cdecl NERPFunc__SetCameraGotoTutorial(sint32* stack);

// <LegoRR.exe @004575e0>
//#define NERPFunc__GetCameraAtTutorial ((NERPsFunction)0x004575e0)
sint32 __cdecl NERPFunc__GetCameraAtTutorial(sint32* stack);

// <LegoRR.exe @00457620>
//#define NERPFunc__GetTutorialBlockIsGround ((NERPsFunction)0x00457620)
sint32 __cdecl NERPFunc__GetTutorialBlockIsGround(sint32* stack);

// <LegoRR.exe @00457660>
//#define NERPFunc__GetTutorialBlockIsPath ((NERPsFunction)0x00457660)
sint32 __cdecl NERPFunc__GetTutorialBlockIsPath(sint32* stack);

// <LegoRR.exe @004576a0>
//#define NERPFunc__SetTutorialBlockIsGround ((NERPsFunction)0x004576a0)
sint32 __cdecl NERPFunc__SetTutorialBlockIsGround(sint32* stack);

// <LegoRR.exe @004576e0>
//#define NERPFunc__SetTutorialBlockIsPath ((NERPsFunction)0x004576e0)
sint32 __cdecl NERPFunc__SetTutorialBlockIsPath(sint32* stack);

// <LegoRR.exe @00457720>
//#define NERPFunc__GetUnitAtBlock ((NERPsFunction)0x00457720)
sint32 __cdecl NERPFunc__GetUnitAtBlock(sint32* stack);

// <LegoRR.exe @00457760>
//#define NERPFunc__GetMonsterAtTutorial ((NERPsFunction)0x00457760)
sint32 __cdecl NERPFunc__GetMonsterAtTutorial(sint32* stack);

// MERGED FUNCTION
// <LegoRR.exe @00484e50>
//#define NERPFunc__True ((NERPsFunction)0x00484e50)
sint32 __cdecl NERPFunc__True(sint32* stack);


#pragma endregion

}
