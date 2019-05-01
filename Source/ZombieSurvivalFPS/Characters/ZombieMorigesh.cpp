// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieMorigesh.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "Core/ZombieSurvivalFPSGameMode.h"
#include "Placeables/ZombieBarrier.h"
#include "Characters/Common/ZombieBaseAnimationInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Common/AI/ZombieAI.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


// Sets default values
AZombieMorigesh::AZombieMorigesh()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitBox"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHitBox"));

	ProjectileOffSet = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOffSet"));

	ProjectileOffSet->SetupAttachment(RootComponent);
	Head->SetupAttachment(GetMesh(), FName("headSocket"));
	Body->SetupAttachment(GetMesh(), FName("bodySocket"));

	//Set default values (could be edited in editor later)
	Health = 150;
	Speed = 2;
	//Never issues a melee attack.
	AttackDamage = 0;
}

// Called when the game starts or when spawned
void AZombieMorigesh::BeginPlay()
{
	Super::BeginPlay();

	Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnHeadshot);
	Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnBodyshot);

}

void AZombieMorigesh::Attack(AActor * Target)
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

void AZombieMorigesh::OnHeadshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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

void AZombieMorigesh::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* Projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (IsValid(Projectile)) {

		UGameplayStatics::ApplyPointDamage(this, Projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}

}

void AZombieMorigesh::LaunchProjectile()
{
	if (IsValid(ProjectileClass)) {
		UWorld* const World = GetWorld();
		if (IsValid(World))
		{
			AZombieAI* ControllerRef = Cast<AZombieAI>(GetController());
			if (IsValid(ControllerRef)) {
				FVector TargetPoint = ControllerRef->GetTargetPoint();
				FVector Location = ProjectileOffSet->GetComponentLocation();
				FVector Direction = TargetPoint - ProjectileOffSet->GetComponentLocation();
				FRotator Rotation = Direction.Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// spawn the projectile at the muzzle
				AZombieMorigeshProjectile* SpawnedProjectile = World->SpawnActor<AZombieMorigeshProjectile>(ProjectileClass, Location, Rotation, ActorSpawnParams);
				if (IsValid(SpawnedProjectile)) {
					SpawnedProjectile->SetOwner(this);
					SpawnedProjectile->SetOwnerController(GetController());
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("Zombie Morigesh failed to spawn projectile."));
				}
				//DrawDebugPoint(GetWorld(), Location, 20, FColor(255, 0, 255), true, 3);
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Controller not of type ZombieAI."));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid World."));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass not set."));
	}
}

