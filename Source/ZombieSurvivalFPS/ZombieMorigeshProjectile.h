// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "ZombieMorigeshProjectile.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieMorigeshProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Damage;
	
public:	
	// Sets default values for this actor's properties
	AZombieMorigeshProjectile();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	class UStaticMeshComponent* ProjectileMesh;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	class UBoxComponent * CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	FORCEINLINE int GetDamage() const { return Damage; }
};
