// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableSpawnButton.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "WeaponBase.h"
#include "Placeables/InteractableComponent.h"

AInteractableSpawnButton::AInteractableSpawnButton() {
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);

	CostText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CostText"));
	CostText->SetupAttachment(RootComponent);
}

void AInteractableSpawnButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableSpawnButton::BeginPlay() {
	Super::BeginPlay();

	OnUseUnlockedDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));
	OnUsePurchaseableDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));

	CostText->SetText(FText().FromString(FString::FromInt(Cost) + FString("$")));
}

void AInteractableSpawnButton::OnUseUnlocked()
{
	if (IsValid(SpawnedWeapon)) {
		SpawnedWeapon->Destroy();
	}

	FTransform SpawnTransform = SpawnLocation->GetComponentTransform();
	SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(SpawnClass, SpawnTransform.GetLocation(), SpawnTransform.Rotator());

}