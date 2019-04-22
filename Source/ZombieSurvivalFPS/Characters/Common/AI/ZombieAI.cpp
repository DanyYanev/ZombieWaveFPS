// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Common/AI/ZombieAI.h"
#include "Characters/ZombieCharacter.h"
#include "Placeables/ZombieBarrier.h"
#include "Core/ZombieSurvivalFPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "Math/NumericLimits.h"

AZombieAI::AZombieAI() {
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BrainComponent = BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));

	PrimaryActorTick.bCanEverTick = true;
	//Targets = nullptr;
}

void AZombieAI::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (IsValid(GameMode)) {
		AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
		if (IsValid(ZombieGameMode)) {
			Targets = ZombieGameMode->GetTargets();
			// On first tick, Behaviour tree calls SetNewTarget.
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Game mode cast failed."))
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Unable to retrieve GameMode."))
	}
}

void AZombieAI::AttackTarget(AActor * Target)
{

	APawn* PawnRef = GetPawn();
	AZombieBase* Zombie = Cast<AZombieBase>(PawnRef);

	if (IsValid(Zombie)) {
		Zombie->Attack(Target);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Pawn isn't deriving from ZombieBase."));
	}
}

void AZombieAI::SetNewTarget()
{
	AActor * Target = FindClosestTarget();

	if (IsValid(Target)) {
		

		AZombieBarrier * Barrier = Cast<AZombieBarrier>(Target);

		if (IsValid(Barrier)) {

			BehaviorTreeComp->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(TargetKeyId, Target);

			FVector TargetPoint = FindClosestTargetPoint();
			BehaviorTreeComp->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(TargetPointKeyId, TargetPoint);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Target Actor isn't Barrier."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Closest target actor is not valid."));
	}
	
}

AActor * AZombieAI::FindClosestTarget()
{
	AActor * NewTarget = nullptr;

	if (Targets != NULL) {
		if (Targets->Num() != 0) {
			float MinDistance = TNumericLimits< float >::Max();
			APawn * CharacterRef = GetPawn();
			if (IsValid(CharacterRef)) {
				for (int i = 0; i < Targets->Num(); i++) {
					FVector Distance = CharacterRef->GetActorLocation() - (*Targets)[i]->GetActorLocation();

					if (Distance.Size() < MinDistance) {
						NewTarget = (*Targets)[i];
						MinDistance = Distance.Size();
					}
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Unable to retrieve a valid Character."));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Targets array is NULL."))
	}

	return NewTarget;
}

FVector AZombieAI::FindClosestTargetPoint()
{
	FVector NewTarget;

	AZombieBarrier * Barrier = Cast<AZombieBarrier>(BehaviorTreeComp->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetKeyId));

	if (IsValid(Barrier)) {
		float MinDistance = TNumericLimits< float >::Max();
		APawn * CharacterRef = GetPawn();
		TArray<FVector> * const TargetPoints = &Barrier->TargetPoints;
		if (IsValid(CharacterRef)) {
			for (int i = 0; i < TargetPoints->Num(); i++) {
				FVector Distance = CharacterRef->GetActorLocation() - (*TargetPoints)[i];

				if (Distance.Size() < MinDistance) {
					NewTarget = (*TargetPoints)[i];
					MinDistance = Distance.Size();
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Character reference isn't valid."));
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Barrier isn't valid during choosing targetpoint."));
	}

	return NewTarget;
}

void AZombieAI::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AZombieBase * Zombie = Cast<AZombieBase>(InPawn);

	if (IsValid(Zombie)){
		if (IsValid(Zombie->BehaviorTree)) {
			BlackboardComp->InitializeBlackboard(*Zombie->BehaviorTree->BlackboardAsset);

			TargetKeyId = BlackboardComp->GetKeyID("Target");
			TargetPointKeyId = BlackboardComp->GetKeyID("TargetPoint");

			BehaviorTreeComp->StartTree(*Zombie->BehaviorTree);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Zombies BehaviorTree is not valid."));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Trying to possess a pawn thats not deriving from ZombieBase."));
	}
}

FVector AZombieAI::GetTargetPoint()
{
	if (IsValid(BehaviorTreeComp)) {
		FVector TargetPoint = BehaviorTreeComp->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(TargetPointKeyId);
		return TargetPoint;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Zombies BehaviorTreeComp is not valid."));
	}

	return FVector(-1, -1, -1);
}