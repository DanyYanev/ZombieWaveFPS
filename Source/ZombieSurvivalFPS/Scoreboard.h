// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Scoreboard.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AScoreboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScoreboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * Mesh;

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
	
};
