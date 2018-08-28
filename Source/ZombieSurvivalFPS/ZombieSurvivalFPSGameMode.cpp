// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieSurvivalFPSHUD.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "ZombieBarrier.h"
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
	Countdown = TimeBetweenWaves;

	AliveZombies = 0;
	InitialZombies = 0;

	TimeBetweenWaves = 10;

}

void AZombieSurvivalFPSGameMode::BeginPlay()
{
	Super::InitGameState();

	TArray<AActor*> SpawnResult;

	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), SpawnResult);
	UGameplayStatics::GetAllActorsOfClass(this, AZombieBarrier::StaticClass(), Targets);


	TArray<APlayerStart *> Result;

	for (int i = 0; i < SpawnResult.Num(); i++) {

		APlayerStart * PlayerStart = Cast<APlayerStart>(SpawnResult[i]);

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

	if (Scoreboard) {
		Scoreboard->UpdateWave(Wave);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Scoreboard not found!"));
	}


	if (Wave <= 5) {
		GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AZombieSurvivalFPSGameMode::TickTimer, 1, true);
	}
	else {
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Type::Quit);
	}
}

void AZombieSurvivalFPSGameMode::SpawnZombies()
{
	InitialZombies = Wave * 3;

	TArray<FTransform> SpawnArray = ZombieSpawnEast;

	if (Wave > 2) {
		SpawnArray += ZombieSpawnEast += ZombieSpawnWest;
	}
	else if (Wave % 2 == 0) {
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

void AZombieSurvivalFPSGameMode::TickTimer()
{
	if (Countdown <= 0) {
		SpawnZombies();
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		Countdown = TimeBetweenWaves;
		if (Scoreboard) {
			Scoreboard->ClearCountdown();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Scoreboard not found."));
		}
	}
	else {
		Countdown--;
		if (Scoreboard) {
			Scoreboard->UpdateCountdown(Countdown);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Scoreboard not found."));
		}
		
	}
}

void AZombieSurvivalFPSGameMode::AttachScoreboard(AScoreboard * ScoreboardInstance)
{
	if (IsValid(ScoreboardInstance)) {
		Scoreboard = ScoreboardInstance;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Unable to attach scoreboard - Pointer invalid"));
	}
		
}

void AZombieSurvivalFPSGameMode::AttachLevelUpShop(ALevelUpShop * LevelUpShopInstance)
{
	if (IsValid(LevelUpShopInstance)) {
		LevelUpShop = LevelUpShopInstance;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Unable to attach LevelUpShop - Pointer invalid"));
	}
}

void AZombieSurvivalFPSGameMode::ZombieDeath()
{
	//UE_LOG(LogTemp, Error, TEXT("Zombie death counted"));

	UpdateCurrentScoreBy(100);
	UpdateCurrentMoneyBy(100);

	AliveZombies--;
	if (AliveZombies <= 0) {
		NextWave();
	}
}

void AZombieSurvivalFPSGameMode::TargetDestroyed(AActor * Target)
{
	Targets.Remove(Target);

	if (Target->ActorHasTag(TEXT("Final"))) { //Endgame
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Type::Quit);
	}
}

void AZombieSurvivalFPSGameMode::UpdateCurrentScoreBy(int Value) {
	Score += Value;

	if (IsValid(Scoreboard)) {
		Scoreboard->UpdateScore(Score);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Scoreboard not found!"));
	}
}

void AZombieSurvivalFPSGameMode::UpdateCurrentMoneyBy(int Value)
{
	Money += Value;

	if (IsValid(Scoreboard)) {
		Scoreboard->UpdateMoney(Money);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Scoreboard not found!"));
	}

	if (IsValid(LevelUpShop)) {
		LevelUpShop->UpdateMoney(Money);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("LevelUpShop not found!"));
	}
}

