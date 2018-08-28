// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "InteractableButton.h"
#include "LevelUpShop.generated.h"

class AZombieSurvivalFPSGameMode;

UCLASS()
class ZOMBIESURVIVALFPS_API ALevelUpShop : public AActor
{
	GENERATED_BODY()

public:

	ALevelUpShop();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent * TitleText;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent * Money;

	UPROPERTY(EditAnywhere)
		TArray<AInteractableButton *> Buttons;

	UFUNCTION()
	void UpdateMoney(int Value);

	UFUNCTION()
	void LevelPurchased();
	
private:

	bool bPurchaseWasAvaiable = false;

	int CurrentLevel = 0;

	int CostForNextLevel = 500;

	int MaxLevel;

	AZombieSurvivalFPSGameMode * GameMode;

	SignatureOnLevelPurchased OnLevelPurchasedDelegate;
};
