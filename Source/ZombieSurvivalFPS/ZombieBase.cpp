// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieBase.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieBarrier.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "ZombieAI.h"
#include "Engine/World.h"


// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	Head->AttachToComponent(GetCapsuleComponent(), rules);
	Body->AttachToComponent(GetCapsuleComponent(), rules);
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

void AZombieBase::EndGame(bool Won)
{
	if (Won) {
		bIsCelebrating = true;
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

void AZombieBase::DealDamage(AActor * Target)
{
	if (IsValid(Target))
		UGameplayStatics::ApplyDamage(Target, GetAttackDamage(), GetController(), this, nullptr);
}

float AZombieBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Already dying dont try to render its killing twice
	if (bIsDying)
		return 0.0f;

	Health -= Damage;

	//UE_LOG(LogTemp, Warning, TEXT("Dealt: %d, Remaining: %d"), Damage, Health);

	if (Health <= 0) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		//Destroy();
		bIsDying = true;
		UE_LOG(LogTemp, Warning, TEXT("ZombieDeath"));

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->ZombieDeath(this);
			}
		}
	}

	return 0.0f;
}

