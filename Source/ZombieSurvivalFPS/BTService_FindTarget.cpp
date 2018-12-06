// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_FindTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "ZombieAI.h"

UBTService_FindTarget::UBTService_FindTarget() {
	bCreateNodeInstance = true;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AZombieAI * ZombieAI = Cast<AZombieAI>(OwnerComp.GetAIOwner());

	if (ZombieAI) {

		AActor * Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ZombieAI->TargetKeyId));
		
		if (!IsValid(Target)) {
			ZombieAI->SetNewTarget();
		}
	}
}
