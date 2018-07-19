// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSGameMode.generated.h"

UCLASS(minimalapi)
class AZombieSurvivalFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int Score;

	UPROPERTY(VisibleAnywhere)
	int InitialZombies;

	UPROPERTY(VisibleAnywhere)
	int AliveZombies;

	UPROPERTY(VisibleAnywhere)
	int Wave;

	UPROPERTY(VisibleAnywhere)
	TArray<FTransform> ZombieSpawnEast;

	UPROPERTY(VisibleAnywhere)
	TArray<FTransform> ZombieSpawnWest;
public:

	/** The class of Zombie to spawn. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<ACharacter> ZombieClass;

	AZombieSurvivalFPSGameMode();

	UFUNCTION()
	void UpdateCurrentScoreBy(int Value);
	
	UFUNCTION()
	void ZombieDeath();
	
	UFUNCTION()
	void NextWave();

	UFUNCTION()
	virtual void BeginPlay() override;

	FORCEINLINE int GetScore() const { return Score; }
};

