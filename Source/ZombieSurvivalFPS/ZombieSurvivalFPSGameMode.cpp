// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieSurvivalFPSHUD.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombieSurvivalFPSGameMode::AZombieSurvivalFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombieSurvivalFPSHUD::StaticClass();
}
