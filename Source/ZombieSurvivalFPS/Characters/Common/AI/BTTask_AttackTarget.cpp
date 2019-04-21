// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Characters/ZombieCharacter.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "Characters/Common/AI/ZombieAI.h"
#include "Kismet/GameplayStatics.h"


EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AZombieAI * ZombieAI = Cast<AZombieAI>(OwnerComp.GetAIOwner());
	if (!IsValid(ZombieAI))
		return EBTNodeResult::Failed;

	AActor * Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ZombieAI->TargetKeyId));

	if (IsValid(Target)) {
		ZombieAI->AttackTarget(Target);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
