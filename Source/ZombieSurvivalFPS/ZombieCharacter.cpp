// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieBarrier.h"
#include "ZombieBaseAnimationInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ZombieAI.h"
#include "Engine/World.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitBox"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHitBox"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	//GetMesh()->SetupAttachment(Head, FName("headSocket"));
	Head->SetupAttachment(GetMesh(), FName("headSocket"));
	Body->SetupAttachment(GetMesh(), FName("bodySocket"));
	//GetMesh()->SetupAttachment(Body, FName("bodySocket"));

	if (!(Body && Head))
		UE_LOG(LogTemp, Error, TEXT("SOMETHING WENT HORRIBLY WRONG"));

	Health = 100;
	Speed = 1;
	AttackDamage = 100;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnHeadshot);
	Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnBodyshot);
}

void AZombieCharacter::Attack(AActor * Target)
{
	if (Target) {
		if (!GetMesh()) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(GetMesh()->GetAnimInstance());
			if (!AnimInstance) {
				AnimInstance->SetIsAttacking(true);
				AnimInstance->SetTarget(Target);
			}
			else
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Mesh is NULL"));
	}
}

void AZombieCharacter::DealDamageToTargetActor(AActor * DamagedActor)
{
	if (IsValid(DamagedActor)) {
		UGameplayStatics::ApplyDamage(DamagedActor, AttackDamage, GetController(), this, NULL);
	}
}

void AZombieCharacter::OnHeadshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile != nullptr) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode =  Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->UpdateCurrentScoreBy(20);
			}
		}
		
		//UE_LOG(LogTemp, Warning, TEXT("Headshot attempting to deal: %d"), projectile->GetDamage() * 2);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 2, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
}

void AZombieCharacter::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile) {

		//UE_LOG(LogTemp, Warning, TEXT("Body shot attempting to deal: %d"), projectile->GetDamage() * 1);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);
		
		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
	
}
