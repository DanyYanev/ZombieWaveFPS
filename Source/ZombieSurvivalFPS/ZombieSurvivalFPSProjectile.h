// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "ZombieSurvivalFPSProjectile.generated.h"

UCLASS(config=Game)
class AZombieSurvivalFPSProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	AZombieSurvivalFPSProjectile();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};

