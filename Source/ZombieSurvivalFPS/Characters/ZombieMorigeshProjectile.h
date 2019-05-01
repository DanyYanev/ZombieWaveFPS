// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Common/ProjectileBase.h"
#include "ZombieMorigeshProjectile.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieMorigeshProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int Damage;

	UPROPERTY(VisibleAnywhere)
	AController* OwnerController;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
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

	FORCEINLINE void SetOwnerController(AController* pOwnerController) { OwnerController = pOwnerController; }

	FORCEINLINE int GetDamage() const { return Damage; }

protected:

	virtual void BeginPlay() override;
};
