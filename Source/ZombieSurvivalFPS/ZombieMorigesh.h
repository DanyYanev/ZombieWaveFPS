// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ZombieBase.h"
#include "ZombieMorigesh.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieMorigesh : public AZombieBase

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieMorigesh();

	UFUNCTION()
	virtual void OnHeadshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnBodyshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;

};
