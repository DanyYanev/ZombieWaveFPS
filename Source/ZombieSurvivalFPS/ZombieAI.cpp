// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAI.h"
#include "ZombieCharacter.h"
#include "ZombieBarrier.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "Math/NumericLimits.h"

AZombieAI::AZombieAI() {
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BrainComponent = CreateDefaultSubobject<UBrainComponent>(TEXT("BrainComponent"));

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
	AActor * Target = FindClosestTarget();
	BehaviorTreeComp->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(TargetKeyId, Target);

	AZombieBarrier * Barrier = Cast<AZombieBarrier>(Target);

	if (Barrier) {
		FVector TargetPoint = FindClosestTargetPoint();
		BehaviorTreeComp->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(TargetPointKeyId, TargetPoint);
	/*
		DrawDebugPoint(
			GetWorld(),
			TargetPoint,
			50,
			FColor::Red,
			false,
			20
		);
	*/
	}
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

FVector AZombieAI::FindClosestTargetPoint()
{
	FVector NewTarget;

	AZombieBarrier * Barrier = Cast<AZombieBarrier>(BehaviorTreeComp->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetKeyId));

	if (Barrier) {
		float MinDistance = TNumericLimits< float >::Max();
		APawn * Character = GetPawn();
		TArray<FVector> * const TargetPoints = &Barrier->TargetPoints;
		if (Character) {
			for (int i = 0; i < TargetPoints->Num(); i++) {
				FVector Distance = Character->GetActorLocation() - (*TargetPoints)[i];

				if (Distance.Size() < MinDistance) {
					NewTarget = (*TargetPoints)[i];
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
		TargetPointKeyId = BlackboardComp->GetKeyID("TargetPoint");

		//SetNewTarget();

		BehaviorTreeComp->StartTree(*Zombie->BehaviorTree);
	}
}
