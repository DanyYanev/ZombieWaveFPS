// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "DrawDebugHelpers.h"
#include "ZombieAI.h"


EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AZombieAI * ZombieAI = Cast<AZombieAI>(OwnerComp.GetAIOwner());

	FVector TargetPoint = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(ZombieAI->TargetPointKeyId);

	//TargetPoint = FVector(-476.945374, 441.319275, 268.373718);

	ZombieAI->MoveToLocation(TargetPoint, 5.f, true, true, true, true);

	EPathFollowingStatus::Type status = ZombieAI->GetPathFollowingComponent()->GetStatus();
	/*
	DrawDebugPoint(
		GetWorld(),
		TargetPoint,
		50,
		FColor::Blue,
		false,
		20
	);
	*/
	if (ZombieAI->GetPawn()->GetActorLocation().Equals(TargetPoint, 100.f)) {
		//UE_LOG(LogTemp, Warning, TEXT("ACtorReached"));
		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}
		

	return EBTNodeResult::Failed;
}
