// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieCharacter.h"
#include "Scoreboard.h"
#include "ZombieSurvivalFPSGameMode.generated.h"

UCLASS(minimalapi)
class AZombieSurvivalFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

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
	FTimerHandle CountdownTimerHandle;

	UFUNCTION()
	void TickTimer();

	UFUNCTION()
	void SpawnZombies();


public:

	/** The class of Zombie to spawn. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<ACharacter> ZombieClass;

	AZombieSurvivalFPSGameMode();

	//In seconds
	UPROPERTY(EditAnywhere)
	int TimeBetweenWaves;

	UFUNCTION()
	void UpdateCurrentScoreBy(int Value);

	UFUNCTION()
	void UpdateCurrentMoneyBy(int Value);
	
	UFUNCTION()
	void ZombieDeath();

	UFUNCTION()
	void TargetDestroyed(AActor * Barricade);
	
	UFUNCTION()
	void NextWave();

	UFUNCTION()
	void AttachScoreboard(AScoreboard * ScoreboardInstance);

	UFUNCTION()
	virtual void BeginPlay() override;

	FORCEINLINE int GetScore() const { return Score; }

	FORCEINLINE const TArray<AActor*> * GetTargets() const { return &Targets; }
};

