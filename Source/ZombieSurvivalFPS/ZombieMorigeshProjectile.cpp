// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieMorigeshProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AZombieMorigeshProjectile::AZombieMorigeshProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

 	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	ProjectileMesh->SetupAttachment(CollisionComp);

	Damage = 70.f;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigeshProjectile::OnOverlap);
}

void AZombieMorigeshProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Morigesh projectile OnOverlap"));
	Destroy();
}
