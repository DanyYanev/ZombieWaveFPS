// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "InteractableButton.h"
#include "Components/BoxComponent.h"
#include "UpgradableStatItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API AUpgradableStatItemComponent : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AUpgradableStatItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	TArray<AInteractableButton*> LevelBoxes;
	
	int CurrentLevel = 0;

	int TotalLevels = 5;

	UTextRenderComponent * StatName;
	
};
