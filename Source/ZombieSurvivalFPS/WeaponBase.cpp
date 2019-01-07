// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "InteractableComponent.h"
#include "Engine/World.h"
#include "ZombieBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Root);
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
	OnReloadDelegate.BindUFunction(this, TEXT("Reload"));

	if (IsValid(InteractableComponent)) {
		InteractableComponent->InitializeUseDelegates(&OnBeginUseDelegate, &OnEndUseDelegate);
		InteractableComponent->InitializeSelectDelegates(&OnSelectDelegate, &OnDeselectDelegate);
		InteractableComponent->InitializeGrabDelegates(&OnBeginGrabDelegate, &OnEndGrabDelegate);
		InteractableComponent->InitializeActionDelegates(&OnReloadDelegate);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Interactable Component is not valid."));
	}

	BulletsInMagazine = MagazineSize;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire()
{
	if (BulletsInMagazine <= 0) {
		Reload();
	} else if (!bIsReloading) {
		FVector Start = MuzzleOffset->GetComponentLocation();
		FVector Forward = MuzzleOffset->GetForwardVector();
		//Multiply forward vector by X moves it "forward" x units
		FVector End = Start + (Forward * 3000);

		FCollisionQueryParams Params = FCollisionQueryParams(FName("FireTrace"));

		FHitResult RV_Hit(ForceInit);

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			Params
		);

		UGameplayStatics::PlaySoundAtLocation(this, FireCue, GetActorLocation());
		BulletsInMagazine--;

		if (RV_Hit.IsValidBlockingHit()) {
			if (IsValid(RV_Hit.GetActor())) {
				AZombieBase* Target = Cast<AZombieBase>(RV_Hit.GetActor());
				if (IsValid(Target)) {
					UGameplayStatics::ApplyPointDamage(Target, Damage, GetActorLocation(), RV_Hit, NULL, this, NULL);
				}
			}
		}
	}
}

void AWeaponBase::Reload()
{
	if(!bIsReloading && BulletsInMagazine < MagazineSize){
		bIsReloading = true;
		UGameplayStatics::PlaySoundAtLocation(this, ReloadCue, GetActorLocation());
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::FinishReload, ReloadTime, false);
	}
	
}

void AWeaponBase::FinishReload()
{
	bIsReloading = false;
	BulletsInMagazine = MagazineSize;
}

void AWeaponBase::BeginUse()
{
	if (bIsAutomatic) {
		if (FireRate != 0) {
			//Shoot first bullet manually as timer fires after set time.
			Fire();
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