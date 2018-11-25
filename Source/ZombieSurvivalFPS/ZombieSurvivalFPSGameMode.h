// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieCharacter.h"
#include "Scoreboard.h"
#include "LevelUpShop.h"
#include "Blueprint/UserWidget.h"
#include "ZombieSurvivalFPSGameMode.generated.h"

UCLASS(minimalapi)
class AZombieSurvivalFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	bool bGameEndStatus;

	UPROPERTY(VisibleAnywhere)
	bool bIsPaused;

	UPROPERTY(VisibleAnywhere)
	bool bGameWon;

	UPROPERTY(VisibleAnywhere)
	int Countdown = TimeBetweenWaves;

	UPROPERTY(VisibleAnywhere)
	int InitialZombies;

	UPROPERTY(VisibleAnywhere)
	int Score;

	UPROPERTY(VisibleAnywhere)
	int Money;

	UPROPERTY(VisibleAnywhere)
	int AliveZombies;

	UPROPERTY(VisibleAnywhere)
	int Wave;

	UPROPERTY(VisibleAnywhere)
	TArray<FTransform> ZombieSpawnEast;

	UPROPERTY(VisibleAnywhere)
	TArray<FTransform> ZombieSpawnWest;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> Targets;

	UPROPERTY(VisibleAnywhere)
	AScoreboard * Scoreboard;

	UPROPERTY(VisibleAnywhere)
	ALevelUpShop * LevelUpShop;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle CountdownTimerHandle;

	//In seconds
	UPROPERTY(EditAnywhere)
	int TimeBetweenWaves = 20;

	UFUNCTION()
	void TickTimer();

	UFUNCTION()
	void SpawnZombies();

	UFUNCTION()
	void EndGame(bool Won);

	void NextWave();

	UPROPERTY(VisibleAnywhere)
	TArray<AZombieBase *> Zombies;

	UUserWidget * EndGameWidgetInstance;

	UUserWidget * PausedWidgetInstance;

public:

	/** The class of Zombie to spawn. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AZombieBase> BasicZombieClass;

	/** The class of Zombie to spawn. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AZombieBase> MorigeshZombieClass;

	/** The class of Zombie to spawn. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<AZombieBase> GruxZombieClass;

	/*The widget to spawn after pause*/
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> PausedWidgetClass;

	/* The widget to spawn after game end*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> EndGameWidgetClass;

	AZombieSurvivalFPSGameMode();

	void UpdateCurrentScoreBy(int Value);

	void UpdateCurrentMoneyBy(int Value);
	
	void ZombieDeath(AZombieBase * Zombie);

	void TargetDestroyed(AActor * Barricade);

	void AttachScoreboard(AScoreboard * ScoreboardInstance);

	void AttachLevelUpShop(ALevelUpShop * LevelUpShopInstance);

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "GameStatus")
	bool IsGameOver() const { return bGameEndStatus; }

	UFUNCTION(BlueprintCallable, Category = "GameStatus")
	bool IsGameWon() const { return bGameWon; }

	UFUNCTION(BlueprintCallable, Category = "GameStatus")
	void SetGamePause();

	//For skipping wait time between waves.
	void QuickStartWave();

	FORCEINLINE const TArray<AActor*> * GetTargets() const { return &Targets; }
};

