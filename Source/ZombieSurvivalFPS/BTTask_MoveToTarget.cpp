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

	if (ZombieAI) {
		FVector TargetPoint = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(ZombieAI->TargetPointKeyId);

		ZombieAI->MoveToLocation(TargetPoint, 5.f, true, true, true, true);

		EPathFollowingStatus::Type status = ZombieAI->GetPathFollowingComponent()->GetStatus();

		if (ZombieAI->GetPawn()->GetActorLocation().Equals(TargetPoint, 100.f)) {
			return EBTNodeResult::Succeeded;
		}
		else {
			return EBTNodeResult::Failed;
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Zombie not possessed by AI"));
	
		

	return EBTNodeResult::Failed;
}
