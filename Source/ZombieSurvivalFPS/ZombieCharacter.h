// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieCharacter : public AZombieBase
{
	GENERATED_BODY()

public:
	AZombieCharacter();

	virtual void Attack(AActor * Target) override;

	UFUNCTION(BlueprintCallable)
	void DealDamageToTargetActor(AActor * DamagedActor);

	UPROPERTY(Category = Hitboxes, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent * Head;

	UPROPERTY(Category = Hitboxes, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent * Body;

	UFUNCTION()
	virtual void OnHeadshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnBodyshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	virtual void BeginPlay() override;

};
