// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "ZombieBarrier.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieBarrier();

	UPROPERTY(EditAnywhere)
	int Health;

	UPROPERTY(EditAnywhere)
	int TargetNumber = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> TargetPoints;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
};
