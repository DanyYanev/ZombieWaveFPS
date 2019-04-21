// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "UI/TextRotationComponent.h"
#include "ZombieBarrier.generated.h"


UCLASS()
class ZOMBIESURVIVALFPS_API AZombieBarrier : public AActor
{
	GENERATED_BODY()

public:
	AZombieBarrier();

	UPROPERTY(Category = Stats, EditAnywhere)
	int MaxHealth;

	UPROPERTY(VisibleAnywhere)
	int TargetNumber = 0;

	int CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> TargetPoints;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * HealthText;

	UPROPERTY(EditAnywhere)
	UTextRotationComponent * TextRotation;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:	
	void UpdateHealthText();

	void GenerateTargetPoints();

	//The distance from the end of the barrier to the most outter point.
	static const float Offset;

	//The distance between two generated target points.
	static const float Distance;
};
