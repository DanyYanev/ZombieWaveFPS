// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieBase.h"
#include "ZombieAI.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieBarrier.h"
#include "ZombieBaseAnimationInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"


// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
}
   
// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieBase::Attack(AActor * Target)
{
	UE_LOG(LogTemp, Error, TEXT("If this calls something went terribly wrong. ***ZombieBase Attack***"));
}

void AZombieBase::EndGame(bool Won)
{
	if (Won) {
		if (GetMesh()) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance) {
				AnimInstance->SetIsCheering(true);
			}
			else
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Mesh is NULL"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("ZOMBIE ALIVE AND GAME WON"));

	AAIController * AIController = Cast<AAIController>(GetController());

	if (AIController) {
		AIController->BrainComponent->StopLogic(TEXT("Game Over"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("AIController not found"));

}

float AZombieBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Already dying dont try to render its killing twice
	if (bIsDying)
		return 0.0f;

	Health -= Damage;

	//UE_LOG(LogTemp, Warning, TEXT("Dealt: %d, Remaining: %d"), Damage, Health);

	if (Health <= 0) {

		//Notifies AnimInstance that death has occured.
		if (GetMesh()) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance) {
				AnimInstance->SetIsDying(true);
			}
			else
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));

		}
		else
			UE_LOG(LogTemp, Error, TEXT("Mesh is NULL"));

		//Disables character movement
		GetCharacterMovement()->DisableMovement();

		UE_LOG(LogTemp, Warning, TEXT("ZombieDeath"));

		//Notifies Game mode of Zombie's death
		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->ZombieDeath(this);
			}
		}

		bIsDying = true;
	}

	return 0.0f;
}

