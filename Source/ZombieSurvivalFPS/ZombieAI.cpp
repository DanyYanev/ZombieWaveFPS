// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAI.h"
#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "Math/NumericLimits.h"

AZombieAI::AZombieAI() {
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	PrimaryActorTick.bCanEverTick = true;
	//Targets = nullptr;
}

void AZombieAI::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (GameMode) {
		AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
		if (ZombieGameMode) {
			Targets = ZombieGameMode->GetTargets();
			// On first tick, Behaviour tree should call SetNewTarget.
		}
	}
}

void AZombieAI::AttackTarget()
{
	AZombieCharacter * Zombie = Cast<AZombieCharacter>(GetPawn());

	if (Zombie) {
		AActor * Target = Cast<AActor>(BehaviorTreeComp->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetKeyId));
		if (Target) {
			Zombie->DealDamage(Target);
		}
	}
}

void AZombieAI::SetNewTarget()
{
	BehaviorTreeComp->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(TargetKeyId, FindClosestTarget());
}

AActor * AZombieAI::FindClosestTarget()
{
	AActor * NewTarget = nullptr;

	if (Targets->Num() != 0) {
		float MinDistance = TNumericLimits< float >::Max();	
		APawn * Character = GetPawn();
		if (Character) {
			for (int i = 0; i < Targets->Num(); i++) {
				FVector Distance = Character->GetActorLocation() - (*Targets)[i]->GetActorLocation();

				if (Distance.Size() < MinDistance) {
					NewTarget = (*Targets)[i];
					MinDistance = Distance.Size();
				}
			}
		}
		
	}

	return NewTarget;
}

void AZombieAI::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AZombieCharacter * Zombie = Cast<AZombieCharacter>(InPawn);

	if (Zombie && Zombie->BehaviorTree) {
		BlackboardComp->InitializeBlackboard(*Zombie->BehaviorTree->BlackboardAsset);

		TargetKeyId = BlackboardComp->GetKeyID("Target");

		//SetNewTarget();

		BehaviorTreeComp->StartTree(*Zombie->BehaviorTree);
	}
}
