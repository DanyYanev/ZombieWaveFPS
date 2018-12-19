// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "InteractableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "InteractableComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	MuzzleOffset = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnBeginUseDelegate.BindUFunction(this, TEXT("BeginUse"));
	OnEndUseDelegate.BindUFunction(this, TEXT("EndUse"));
	OnSelectDelegate.BindUFunction(this, TEXT("Select"));
	OnDeselectDelegate.BindUFunction(this, TEXT("Deselect"));
	OnBeginGrabDelegate.BindUFunction(this, TEXT("BeginGrab"));
	OnEndGrabDelegate.BindUFunction(this, TEXT("EndGrab"));

	if (IsValid(InteractableComponent)) {
		InteractableComponent->InitializeDelegates(&OnBeginUseDelegate, &OnEndUseDelegate, &OnSelectDelegate, &OnDeselectDelegate, &OnBeginGrabDelegate, &OnEndGrabDelegate);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Interactable Component is not valid."));
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire()
{
}

void AWeaponBase::BeginUse()
{
	if (bIsAutomatic) {
		if (FireRate != 0) {
			//Trigger shoot FireRate times a minute.
			GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimerHandle, this, &AWeaponBase::Fire, (float)60 / FireRate, true);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Firerate is 0"));
		}
		
	}
	else {
		Fire();
	}
}

void AWeaponBase::EndUse()
{
	if (bIsAutomatic) {
		GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimerHandle);
	}
}

void AWeaponBase::BeginGrab(USceneComponent * AttachActor)
{
	Mesh->SetSimulatePhysics(false);
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	AttachToComponent(AttachActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weaponSocket"));
}

void AWeaponBase::EndGrab()
{
	Mesh->SetSimulatePhysics(true);
	FDetachmentTransformRules rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(rules);
}

void AWeaponBase::Select()
{
	Mesh->SetRenderCustomDepth(true);
}

void AWeaponBase::Deselect()
{
	Mesh->SetRenderCustomDepth(false);
}