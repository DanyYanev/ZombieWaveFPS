// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ZombieAI.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVALFPS_API AZombieAI : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;
	
	
	UPROPERTY(transient)
	UBehaviorTreeComponent* BehaviorTreeComp;

	UFUNCTION()
	AActor * FindClosestTarget();

public:
	AZombieAI();

	UPROPERTY(VisibleAnywhere)
	uint8 TargetKeyId;

	UPROPERTY(EditAnywhere)
	bool bIsAttacking;

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool Value) { bIsAttacking = Value; }

	UFUNCTION(BlueprintCallable)
	void AttackTarget();	

	TArray<AActor *> const * Targets;

	void SetNewTarget();

	virtual void BeginPlay() override;

	virtual void Possess(APawn* InPawn) override;
};
