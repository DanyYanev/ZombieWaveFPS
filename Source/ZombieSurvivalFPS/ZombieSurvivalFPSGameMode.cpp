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
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AZombieSurvivalFPSHUD::StaticClass();

	Countdown = TimeBetweenWaves;
}

void AZombieSurvivalFPSGameMode::BeginPlay()
{
	Super::InitGameState();

	if (IsValid(EndGameWidgetClass))
	{
		EndGameWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), EndGameWidgetClass, TEXT("EndGameWidget"));

		if (!IsValid(EndGameWidgetInstance)) {
			UE_LOG(LogTemp, Error, TEXT("EndGameWidgetInstance instance failed to create"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("EndGameWidgetClass class not set"));
	}

	if (IsValid(PausedWidgetClass))
	{
		PausedWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PausedWidgetClass, TEXT("PausedWidgetClass"));

		if (!IsValid(PausedWidgetInstance)) {
			UE_LOG(LogTemp, Error, TEXT("PausedWidgetInstance instance failed to create"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PausedWidgetClass class not set"));
	}


	TArray<AActor*> SpawnResult;

	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), SpawnResult);
	UGameplayStatics::GetAllActorsOfClass(this, AZombieBarrier::StaticClass(), Targets);


	TArray<APlayerStart *> Result;

	for (int i = 0; i < SpawnResult.Num(); i++) {

		APlayerStart * PlayerStart = Cast<APlayerStart>(SpawnResult[i]);

		if (IsValid(PlayerStart)) {
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

	//After Money and Score are setup with default values, call this to force update on all relevant actors
	UpdateCurrentMoneyBy(0);
	UpdateCurrentScoreBy(0);
	NextWave();
}

void AZombieSurvivalFPSGameMode::SetGamePause()
{
	if (!bIsPaused) {

		if (!UGameplayStatics::SetGamePaused(this, true)) {
			UE_LOG(LogTemp, Warning, TEXT("Pause failed!"));
		}
		else {
			bIsPaused = true;

			AHUD * HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();

			if (IsValid(HUD)) {
				HUD->ShowHUD();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Hud cast failed"));
			}

			if (IsValid(PausedWidgetInstance)) {
				PausedWidgetInstance->AddToViewport();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("PauseGameWidgetInstance not valid."));
			}

			APlayerController* PlayerControler = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (IsValid(PlayerControler)){
				PlayerControler->bShowMouseCursor = true;
				PlayerControler->bEnableClickEvents = true;
				PlayerControler->bEnableMouseOverEvents = true;
			}
			else{
				UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid PlayerControler."));
			}
		}

	}
	else {
		if (!UGameplayStatics::SetGamePaused(this, false)) {
			UE_LOG(LogTemp, Error, TEXT("UnPause failed!"));
		}
		else {
			bIsPaused = false;

			AHUD * HUD = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();

			if (IsValid(HUD)) {
				HUD->ShowHUD();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Hud cast failed"));
			}

			if (IsValid(PausedWidgetInstance)) {
				PausedWidgetInstance->RemoveFromViewport();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("PauseGameWidgetInstance not craeted"));
			}

			APlayerController* PlayerControler = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			if (IsValid(PlayerControler)){
				PlayerControler->bShowMouseCursor = false;
				PlayerControler->bEnableClickEvents = false;
				PlayerControler->bEnableMouseOverEvents = false;
			}
		}
	}
}

void AZombieSurvivalFPSGameMode::QuickStartWave()
{
	if (Countdown > 0) {
		Countdown = 0;
	}
}

void AZombieSurvivalFPSGameMode::NextWave()
{

	Wave++;

	if (IsValid(Scoreboard)) {
		Scoreboard->UpdateWave(Wave);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Scoreboard not found!"));
	}


	if (Wave <= 5) {
		GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AZombieSurvivalFPSGameMode::TickTimer, 1, true);
	}
	else {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (IsValid(PlayerController)) {
			AHUD * HUD = PlayerController->GetHUD();

			if (IsValid(HUD)) {
				HUD->ShowHUD();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Hud cast failed"));
			}

			EndGameWidgetInstance->AddToViewport();

			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("PlayerController is invalid"));
		}
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
		UWorld* World = GetWorld();
		if (IsValid(World)) {
			AZombieCharacter* Zombie = World->SpawnActor<AZombieCharacter>(BasicZombieClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
			if (IsValid(Zombie)) {
				AliveZombies++;
				Zombies.Add(Zombie);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Zombie Spawn Failed"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Couldnt retrive a valid World reference."));
		}
		
	}
}

void AZombieSurvivalFPSGameMode::OnGameEnded(bool Won)
{
	bGameEndStatus = true;
	bGameWon = Won;

	for (int i = 0; i < Zombies.Num(); i++) {
		Zombies[i]->OnGameEnded(!Won);
	}

	/*
	AZombieSurvivalFPSCharacter * Character = Cast<AZombieSurvivalFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	
	if (IsValid(Character)) {
		Character->OnGameEnded(Won);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Character cast failed"));
	}
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (IsValid(PlayerController)) {
		AHUD * HUD = PlayerController->GetHUD();

		if (IsValid(HUD)) {
			HUD->ShowHUD();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Hud cast failed"));
		}

		EndGameWidgetInstance->AddToViewport();

		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerController is invalid"));
	}
	*/
}

void AZombieSurvivalFPSGameMode::TickTimer()
{
	if (Countdown <= 0) {
		SpawnZombies();
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

		if (IsValid(Scoreboard)) {
			Scoreboard->ClearCountdown();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Scoreboard not found."));
		}
	}
	else {
		Countdown--;
		if (IsValid(Scoreboard)) {
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

void AZombieSurvivalFPSGameMode::ZombieDeath(AZombieBase * Zombie)
{
	//Zombie is still valid because Destroy() is called after completing death animation.
	if (IsValid(Zombie)) {
		//Return 0 if no zombies were removed aka an unregistrated zombie died.
		if (Zombies.Remove(Zombie) > 0) {
			AliveZombies--;
			Zombies.Remove(Zombie);

			if (AliveZombies <= 0) {
				Countdown = TimeBetweenWaves;
				NextWave();
			}
		}

		UpdateCurrentScoreBy(100);
		UpdateCurrentMoneyBy(100);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("An invalid pointer to \"dead\" zombie was registrated."));
	}
}

void AZombieSurvivalFPSGameMode::TargetDestroyed(AActor * Target)
{
	if (IsValid(Target)) {
		Targets.Remove(Target);

		if (Target->ActorHasTag(TEXT("Final"))) { //Endgame
			//Trigger loss
			OnGameEnded(false);
		}

		Target->Destroy();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("An invalid pointer to a destroyed target was registrated."));
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

