// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieBase.h"
#include "ZombieRampage.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVALFPS_API AZombieRampage : public AZombieBase
{
	GENERATED_BODY()
	
public:
	AZombieRampage();

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
