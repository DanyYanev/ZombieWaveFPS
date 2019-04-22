// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGrux.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "Core/ZombieSurvivalFPSGameMode.h"
#include "Placeables/ZombieBarrier.h"
#include "Characters/Common/ZombieBaseAnimationInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Common/AI/ZombieAI.h"
#include "Engine/World.h"

// Sets default values
AZombieGrux::AZombieGrux()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitBox"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHitBox"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	Head->SetupAttachment(GetMesh(), FName("headSocket"));
	Body->SetupAttachment(GetMesh(), FName("bodySocket"));

	//Set default values (could be edited in editor later)
	Health = 300;
	Speed = 1;
	AttackDamage = 100;
}

// Called when the game starts or when spawned
void AZombieGrux::BeginPlay()
{
	Super::BeginPlay();

	Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieGrux::OnHeadshot);
	Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieGrux::OnBodyshot);
}

void AZombieGrux::Attack(AActor * Target)
{
	if (IsValid(Target)) {
		USkeletalMeshComponent * MeshRef = GetMesh();
		if (IsValid(MeshRef)) {
			UZombieBaseAnimationInstance* AnimInstance = Cast<UZombieBaseAnimationInstance>(MeshRef->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				AnimInstance->SetIsAttacking(true);
				AnimInstance->SetTarget(Target);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("MeshAnimBlueprint doesn't derive from ZombieBaseAnimationInstance"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Mesh is not valid."));
		}
	}
}

void AZombieGrux::OnHeadshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* Projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (IsValid(Projectile)) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (IsValid(GameMode)) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (IsValid(ZombieGameMode)) {
				ZombieGameMode->UpdateCurrentScoreBy(20);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
		}

		UGameplayStatics::ApplyPointDamage(this, Projectile->GetDamage() * 2, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
}

void AZombieGrux::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* Projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (IsValid(Projectile)) {

		UGameplayStatics::ApplyPointDamage(this, Projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}

}