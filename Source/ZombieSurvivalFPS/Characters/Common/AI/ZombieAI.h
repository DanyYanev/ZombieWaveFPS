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

public:
	AZombieAI();

	UPROPERTY(VisibleAnywhere)
	uint8 TargetKeyId;

	UPROPERTY(VisibleAnywhere)
	uint8 TargetPointKeyId;

	void AttackTarget(AActor * Target);

	TArray<AActor *> const * Targets;

	void SetNewTarget();

	FVector GetTargetPoint();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BehaviorTreeComp;

	AActor * FindClosestTarget();

	FVector FindClosestTargetPoint();

};
