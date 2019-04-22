// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Common/ZombieBase.h"
#include "Characters/Common/AI/ZombieAI.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "Core/ZombieSurvivalFPSGameMode.h"
#include "Placeables/ZombieBarrier.h"
#include "Characters/Common/ZombieBaseAnimationInstance.h"
#include "UI/HealthBarWidget.h"
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

	//"This is the preferred method when initializing a component in a class constructor" The values are later overridden in Blueprints. 
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));

	FAttachmentTransformRules Rules = FAttachmentTransformRules(FAttachmentTransformRules::KeepRelativeTransform);

	HealthBarWidgetComponent->AttachToComponent(RootComponent, Rules);
}
   
// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	HealthBarInstance = Cast<UOneParamWidget>(HealthBarWidgetComponent->GetUserWidgetObject());

	if (IsValid(HealthBarWidgetComponent)) {
		HealthBarInstance = Cast<UOneParamWidget>(HealthBarWidgetComponent->GetUserWidgetObject());

		if (IsValid(HealthBarInstance)) {
			HealthBarInstance->UpdateParam((float)Health / MaxHealth);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("HealthBarWidget attached to WidgetComponent not of class UOneParamWidget"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("HealthBar Widget class is invalid"));
	}
}

void AZombieBase::Attack(AActor * Target)
{
	UE_LOG(LogTemp, Error, TEXT("If this calls something went terribly wrong. ***ZombieBase Attack***"));
}

void AZombieBase::DealDamageToTargetActor(AActor * DamagedActor)
{
	if (IsValid(DamagedActor)) {
		UGameplayStatics::ApplyDamage(DamagedActor, AttackDamage, GetController(), this, NULL);
	}
}

void AZombieBase::OnGameEnded(bool Won)
{
	if (Won) {
		USkeletalMeshComponent * MeshRef = GetMesh();
		if (IsValid(MeshRef)) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(MeshRef->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				AnimInstance->SetIsCheering(true);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Mesh is not valid."));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ZOMBIE ALIVE AND GAME WON"));
	}

	AAIController * AIController = Cast<AAIController>(GetController());

	if (IsValid(AIController)) {
		AIController->BrainComponent->StopLogic(TEXT("Game Over"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("AIController not found"));
	}
}

float AZombieBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Already dying dont try to render its killing twice
	if (bIsDying)
		return 0.0f;

	Health -= Damage;

	if (IsValid(HealthBarInstance)) {
		if (Health <= 0) {
			HealthBarInstance->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			if (IsValid(HealthBarInstance)) {
				HealthBarInstance->UpdateParam((float)Health / MaxHealth);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("HealthBarWidget attached to WidgetComponent not of class UOneParamWidget"));
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("HealthBarInstance instance is not valid."));
	}
	
	if (Health <= 0) {

		bIsDying = true;
		//Notifies AnimInstance that death has occured.
		USkeletalMeshComponent * MeshRef = GetMesh();
		if (IsValid(MeshRef)) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(MeshRef->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				AnimInstance->SetIsDying(true);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Mesh is not valid."));
		}

		//Disables character movement
		GetCharacterMovement()->DisableMovement();

		UE_LOG(LogTemp, Display, TEXT("ZombieDeath"));

		//Notifies Game mode of Zombie's death
		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (IsValid(GameMode)) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (IsValid(ZombieGameMode)) {
				ZombieGameMode->ZombieDeath(this);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
		}
	}

	return 0.0f;
}

