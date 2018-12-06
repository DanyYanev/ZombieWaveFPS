// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "ZombieSurvivalFPSProjectile.generated.h"

UCLASS(config=Game)
class AZombieSurvivalFPSProjectile : public AActor
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	int Damage;

public:
	AZombieSurvivalFPSProjectile();

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* ProjectileMesh;
	
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	FORCEINLINE int GetDamage() const { return Damage; }
};

