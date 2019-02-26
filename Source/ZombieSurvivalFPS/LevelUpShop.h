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

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent * Mesh;

	/** Sound to play each time something is bought */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* PurchaseCue;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * TitleText;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * Money;

	UPROPERTY(EditAnywhere)
	TArray<AInteractableButton *> Buttons;

	UPROPERTY(EditAnywhere)
	TArray<int32> CostPerButton;

	UFUNCTION()
	void UpdateMoney(int Value);

	UFUNCTION()
	bool LevelPurchased(UObject* ButtonObject);
	
protected:
	virtual void BeginPlay() override;

private:

	int CurrentLevel = 0;

	int CostForNextLevel = 500;

	int MaxLevel;

	AZombieSurvivalFPSGameMode * ZombieGameMode;

	SignatureOnLevelPurchased OnLevelPurchasedDelegate;
};
