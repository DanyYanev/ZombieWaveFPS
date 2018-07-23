// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSCharacter.h"
#include "ZombieAI.h"
#include "Kismet/GameplayStatics.h"


EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AZombieAI * ZombieAI = Cast<AZombieAI>(OwnerComp.GetAIOwner());
	if(!ZombieAI)
		return EBTNodeResult::Failed;

	AZombieCharacter * Zombie = Cast<AZombieCharacter>(ZombieAI->GetPawn());
	if (!Zombie)
		return EBTNodeResult::Failed;

	AActor * Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ZombieAI->TargetKeyId));

	if (Target) {

		ZombieAI->bIsAttacking = true;

		//UGameplayStatics::ApplyDamage(Target, Zombie->GetAttackDamage(), ZombieAI, Zombie, nullptr);

		//ZombieAI->bIsAttacking = false;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}