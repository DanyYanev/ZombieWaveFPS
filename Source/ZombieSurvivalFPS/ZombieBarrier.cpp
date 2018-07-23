// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieBarrier.h"
#include "GameFramework/GameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "Engine/World.h"


// Sets default values
AZombieBarrier::AZombieBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	RootComponent = Mesh;
}


float AZombieBarrier::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	Health -= DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Dealt to barricade: %d, Remaining: %d"), DamageAmount, Health);

	if (Health <= 0) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->TargetDestroyed(this);
			}
		}

		Destroy();
		UE_LOG(LogTemp, Error, TEXT("Barricade Destroyed"));
	}
	return 0.0f;
}
