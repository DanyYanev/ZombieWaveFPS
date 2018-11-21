// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieBase.h"
#include "ZombieBaseAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVALFPS_API UZombieBaseAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UZombieBaseAnimationInstance();
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsCheering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	AActor * Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Logic")
	AZombieBase* OwningPawn;

public:
	void SetIsRunning(bool IsRunning);

	void SetIsAttacking(bool IsAttacking);
	
	void SetIsCheering(bool IsCheering);
	
	void SetIsDying(bool IsDying);

	void SetTarget(AActor * newTarget);

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
