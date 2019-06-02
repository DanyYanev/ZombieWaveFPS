// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Placeables/InteractableComponent.h"
#include "Scoreboard.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AScoreboard : public AActor
{
	GENERATED_BODY()

public:	
	AScoreboard();

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
	UInteractableComponent * InteractableComponent;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * ScoreText;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * MoneyText;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * Score;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * Money;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * Wave;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * Countdown;

	UFUNCTION()
	void UpdateScore(int Value);

	UFUNCTION()
	void UpdateMoney(int Value);

	UFUNCTION()
	void UpdateWave(int Value);

	UFUNCTION()
	void UpdateCountdown(int Value);

	UFUNCTION()
	void ClearCountdown();
	
	UFUNCTION()
	void Select();

	UFUNCTION()
	void Deselect();

	UFUNCTION()
	void Use();

	UFUNCTION()
	void OnGameEnded(bool Won);

protected:
	virtual void BeginPlay() override;

private:

	SignatureOnFunction OnUseDelegate;
	SignatureOnFunction OnSelectDelegate;
	SignatureOnFunction OnDeselectDelegate;
};
