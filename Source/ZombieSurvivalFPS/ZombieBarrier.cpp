// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieBarrier.h"
#include <algorithm>
#include "GameFramework/GameModeBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void AZombieBarrier::UpdateHealthText()
{
	if (IsValid(HealthText)) {
		HealthText->SetText(FText().FromString(FString::FromInt(CurrentHealth) + FString("/") + FString::FromInt(Health)));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("HealthText not initialized!"));
	}
}

// Sets default values
AZombieBarrier::AZombieBarrier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	TextRotation = CreateDefaultSubobject<UTextRotationComponent>(TEXT("TextRotation"));

	SetRootComponent(Mesh);

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	HealthText->AttachToComponent(Mesh, rules);

	AddOwnedComponent(TextRotation);
	TextRotation->AddTextComponent(HealthText);
}


void AZombieBarrier::BeginPlay()
{
	Super::BeginPlay();

	FVector Scale3D = GetActorScale3D();
	float Length;
	//Offset of end points before end of actor
	float Offset = 40;
	//Which way the barrier is scaled
	bool bScaleX = Scale3D.X > Scale3D.Y;

	
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	//GetActor Length to calculate number of targets needed
	if (bScaleX) {
		Length = (FVector(Origin.X + BoxExtent.X, Origin.Y, Origin.Z) - FVector(Origin.X - BoxExtent.X, Origin.Y, Origin.Z)).Size();
	}
	else {
		Length = (FVector(Origin.X, Origin.Y + BoxExtent.Y, Origin.Z) - FVector(Origin.X, Origin.Y - BoxExtent.Y, Origin.Z)).Size();
	}

	Length -= 2 * Offset;
	TargetNumber = Length / 150;

	for (int i = 0; i < TargetNumber + 1; i++) { //Add last point at the end of actor
		if (bScaleX) {
			TargetPoints.Add(FVector(Origin.X - BoxExtent.X + (i * (Length / (float)TargetNumber)) + Offset, Origin.Y, Origin.Z));
		}
		else {
			TargetPoints.Add(FVector(Origin.X, Origin.Y - BoxExtent.Y + (i * (Length / (float)TargetNumber)) + Offset, Origin.Z));
		}
	}

	CurrentHealth = Health;
	UpdateHealthText();
}

float AZombieBarrier::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	CurrentHealth -= DamageAmount;
	UpdateHealthText();

	if (CurrentHealth <= 0) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (IsValid(GameMode)) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (IsValid(ZombieGameMode)) {
				TextRotation->DestroyComponent();
				ZombieGameMode->TargetDestroyed(this);
				//Destroy called from GameMode through Target Destroyed
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
		}
	}

	return 0.0f;
}
