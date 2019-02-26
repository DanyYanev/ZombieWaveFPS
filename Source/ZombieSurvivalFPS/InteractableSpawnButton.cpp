// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableSpawnButton.h"
#include "Components/SceneComponent.h"
#include "WeaponBase.h"
#include "InteractableComponent.h"

AInteractableSpawnButton::AInteractableSpawnButton() {
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);
}

void AInteractableSpawnButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableSpawnButton::BeginPlay() {
	Super::BeginPlay();

	OnUseUnlockedDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));
	OnUsePurchaseableDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));
}

void AInteractableSpawnButton::OnUseUnlocked()
{
	if (IsValid(SpawnedWeapon)) {
		SpawnedWeapon->Destroy();
	}

	FTransform SpawnTransform = SpawnLocation->GetComponentTransform();
	SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(SpawnClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator());

}