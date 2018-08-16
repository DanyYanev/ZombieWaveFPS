// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "TextRotationComponent.h"
#include "ZombieBarrier.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieBarrier : public AActor
{
	GENERATED_BODY()

	void UpdateHealthText();

public:	
	// Sets default values for this actor's properties
	AZombieBarrier();

	UPROPERTY(EditAnywhere)
	int Health;

	int CurrentHealth;

	UPROPERTY(VisibleAnywhere)
	int TargetNumber = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> TargetPoints;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent * HealthText;

	UPROPERTY(EditAnywhere)
	UTextRotationComponent * TextRotation;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
};
