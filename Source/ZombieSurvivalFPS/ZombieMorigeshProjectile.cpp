// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieMorigeshProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AZombieMorigeshProjectile::AZombieMorigeshProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Damage = 50.f;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigeshProjectile::OnOverlap);
}

void AZombieMorigeshProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile OnOverlap"));
	//Destroy();
}
