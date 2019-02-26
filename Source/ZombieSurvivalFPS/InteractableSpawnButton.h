// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableButton.h"
#include "InteractableSpawnButton.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVALFPS_API AInteractableSpawnButton : public AInteractableButton
{
	GENERATED_BODY()
	

public:
	AInteractableSpawnButton();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class USceneComponent* SpawnPreviewLocation;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	class USceneComponent* SpawnLocation;

	UPROPERTY(EditAnywhere, NoClear, Category = "Gameplay")
	class TSubclassOf<class AWeaponBase> SpawnClass;

	UPROPERTY(VisibleAnywhere, Category = "Runtime")
	class AWeaponBase* SpawnedWeapon;

	UFUNCTION()
	void OnUseUnlocked();

protected:
	virtual void BeginPlay() override;

	AWeaponBase* SpawnActor(FTransform Transform);
};
