// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalFPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieCharacter.h"

AZombieSurvivalFPSProjectile::AZombieSurvivalFPSProjectile() 
{
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Damage = 50.f;
}

void AZombieSurvivalFPSProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile OnOverlap"));
	//Destroy();
}
