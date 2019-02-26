// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableSpawnButton.h"
#include "Components/SceneComponent.h"
#include "WeaponBase.h"
#include "InteractableComponent.h"

AInteractableSpawnButton::AInteractableSpawnButton() {
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocation"));
	SpawnLocation->SetupAttachment(RootComponent);

	SpawnPreviewLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPreviewLocation"));
	SpawnPreviewLocation->SetupAttachment(RootComponent);
}

void AInteractableSpawnButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableSpawnButton::BeginPlay() {
	Super::BeginPlay();

	//Spawn Actor for preview

	AWeaponBase* PreviewActor = SpawnActor(SpawnPreviewLocation->GetComponentTransform());

	if (IsValid(PreviewActor)) {
		//Turn off interactable component
		PreviewActor->InteractableComponent->bIsActive = false;
	}

	OnUseUnlockedDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));
	OnUsePurchaseableDelegate.BindUFunction(this, TEXT("OnUseUnlocked"));
}

AWeaponBase* AInteractableSpawnButton::SpawnActor(FTransform Transform)
{
	AWeaponBase* SpawnedActor = NULL;
	
	UWorld* World = GetWorld();
	if (IsValid(World)) {
		SpawnedActor = World->SpawnActor<AWeaponBase>(SpawnClass, Transform.GetLocation(), Transform.Rotator());
	}

	return SpawnedActor;
}

void AInteractableSpawnButton::OnUseUnlocked()
{
	if (IsValid(SpawnedWeapon)) {
		SpawnedWeapon->Destroy();
	}

	SpawnedWeapon = SpawnActor(SpawnLocation->GetComponentTransform());
}