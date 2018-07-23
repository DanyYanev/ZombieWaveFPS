// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "ZombieAI.h"


EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AZombieAI * ZombieAI = Cast<AZombieAI>(OwnerComp.GetAIOwner());

	AActor * Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ZombieAI->TargetKeyId));

	if (Target) {
		ZombieAI->MoveToActor(Target, 5., true, true, true, 0, true);
		if (ZombieAI->GetPawn()->GetActorLocation().Equals(Target->GetActorLocation(), 100.f)) {
			UE_LOG(LogTemp, Warning, TEXT("ACtorReached"));
			return EBTNodeResult::Succeeded;
		}
		else {
			return EBTNodeResult::Failed;
		}
		
	}

	return EBTNodeResult::Failed;
}
