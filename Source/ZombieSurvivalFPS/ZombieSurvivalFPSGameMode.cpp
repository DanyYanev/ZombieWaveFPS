// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieSurvivalFPSHUD.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "ZombieCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"

AZombieSurvivalFPSGameMode::AZombieSurvivalFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombieSurvivalFPSHUD::StaticClass();

	Score = 0;
	Wave = 0;

	AliveZombies = 0;
	InitialZombies = 0;

}

void AZombieSurvivalFPSGameMode::BeginPlay()
{
	Super::InitGameState();

	TArray<AActor*> ActorResult;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), ActorResult);

	TArray<APlayerStart *> Result;

	for (int i = 0; i < ActorResult.Num(); i++) {

		APlayerStart * PlayerStart = Cast<APlayerStart>(ActorResult[i]);

		if (PlayerStart) {
			Result.Add(PlayerStart);
		}

	}

	for (int i = 0; i < Result.Num(); ++i) {
		if (Result[i]->PlayerStartTag == TEXT("ZombieSpawnEast")) {
			ZombieSpawnEast.Add(Result[i]->GetTransform());
		}
		else if (Result[i]->PlayerStartTag == TEXT("ZombieSpawnWest")) {
			ZombieSpawnWest.Add(Result[i]->GetTransform());
		}
	}

	NextWave();
}

void AZombieSurvivalFPSGameMode::NextWave()
{
	Wave++;
	UE_LOG(LogTemp, Warning, TEXT("Wave now: %d"), Wave);

	if (Wave <= 5) {
		InitialZombies = Wave * 5;

		TArray<FTransform> SpawnArray = ZombieSpawnEast;

		if (Wave > 2) {
			SpawnArray += ZombieSpawnEast += ZombieSpawnWest;
		} else if (Wave % 2 == 0) {
			SpawnArray = ZombieSpawnWest;
		}

		for (int i = 0; AliveZombies < InitialZombies && i < SpawnArray.Num(); i++) {
			FTransform SpawnTransform = SpawnArray[i];
			//UE_LOG(LogTemp, Error, TEXT("Zombie Spawn Location: %s"), *SpawnTransform.GetLocation().ToString());
			UWorld* World = GetWorld();
			AActor* Zombie = World->SpawnActor<AActor>(ZombieClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
			if (IsValid(Zombie)) {
				AliveZombies++;
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Zombie Spawn Failed"));
			}
		}
	}
	else {
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Type::Quit);
	}
}

void AZombieSurvivalFPSGameMode::ZombieDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Zombie death counted"));

	UpdateCurrentScoreBy(100);

	AliveZombies--;
	if (AliveZombies <= 0) {
		NextWave();
	}
}

void AZombieSurvivalFPSGameMode::UpdateCurrentScoreBy(int Value) {
	Score += Value;

	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}