// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "InteractableComponent.h"
#include "Engine/World.h"
#include "ZombieBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
	LaserMesh->SetupAttachment(Mesh);

	LaserBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserBeamParticle"));
	LaserBeam->SetupAttachment(LaserMesh);

	ReloadBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ReloadBarWidget"));
	ReloadBarWidgetComponent->SetupAttachment(Mesh);
	
	AmmoBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoBarWidget"));
	AmmoBarWidgetComponent->SetupAttachment(Mesh);

	MuzzleOffset = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	BulletsInMagazine = MagazineSize;
	
	if (IsValid(ReloadBarWidgetComponent)) {
		ReloadBarInstance = Cast<UOneParamWidget>(ReloadBarWidgetComponent->GetUserWidgetObject());

		//UI for weapon hidden until picked up.
		if (IsValid(ReloadBarInstance)) {
			//UpdateWidgetInstanceVisibility already checks if ReloadBarInstance is valid but doesnt throw a warning.
			UpdateWidgetInstanceVisibility(ReloadBarInstance, false);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Reload Widget parent not of class UOneParamWidget"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Reload Widget class is invalid"));
	}

	if (IsValid(ReloadBarWidgetComponent)) {
		AmmoBarInstance = Cast<UOneParamWidget>(AmmoBarWidgetComponent->GetUserWidgetObject());

		if (IsValid(AmmoBarInstance)) {
			//Both UpdateWidgetInstanceVisibility and UpdateAmmo Bar check if ReloadBarInstance is valid but don't throw a Warning.
			UpdateAmmoBar();
			UpdateWidgetInstanceVisibility(AmmoBarInstance, false);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Ammo Widget parent not of class UOneParamWidget"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Ammo Widget class is invalid"));
	}

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
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Distance = CalculateLaserBeamDistance();

	LaserBeam->SetFloatParameter(FName("BeamLength"), Distance);
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
		UpdateAmmoBar();

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
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::TickReload, ReloadTime/ UPDATE_TICKS, true);
		UpdateWidgetInstanceVisibility(ReloadBarInstance, true);
	}
	
}

void AWeaponBase::TickReload()
{
	//Check for final tick
	if (UpdateTickCounter != UPDATE_TICKS) {
		UpdateTickCounter++;
		UpdateReloadBar();
	}
	else {
		UpdateTickCounter = 0;
		FinishReload();
	}

}

void AWeaponBase::FinishReload()
{
	bIsReloading = false;
	BulletsInMagazine = MagazineSize;
	UpdateAmmoBar();
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	UpdateWidgetInstanceVisibility(ReloadBarInstance, false);
}

void AWeaponBase::UpdateReloadBar()
{
	if (IsValid(ReloadBarInstance)) {
		//Convert UpdateTickCounter to 0-1 value
		ReloadBarInstance->UpdateParam((1.f / UPDATE_TICKS) * UpdateTickCounter);
	}
}

void AWeaponBase::UpdateAmmoBar()
{
	if (IsValid(AmmoBarInstance)) {
		AmmoBarInstance->UpdateParam(BulletsInMagazine);
	}
}

float AWeaponBase::CalculateLaserBeamDistance()
{
	FVector Start = LaserBeam->GetComponentLocation();
	FVector Forward = LaserBeam->GetForwardVector();
	//Multiply forward vector by X moves it "forward" x units
	FVector End = Start + (Forward * 3000);

	FCollisionQueryParams Params = FCollisionQueryParams(FName("LaserBeamTrace"));

	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	if (RV_Hit.IsValidBlockingHit()) {
		
		return RV_Hit.Distance;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("DISTANCE: %f"), RV_Hit.Distance));

	//Hit nothing
	return 3000.f;
}

void AWeaponBase::UpdateWidgetInstanceVisibility(UUserWidget * Target, bool isVisible)
{
	if (IsValid(Target)) {
		if (isVisible) {
			Target->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			Target->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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

	UpdateWidgetInstanceVisibility(AmmoBarInstance, true);
}

void AWeaponBase::EndGrab()
{
	Mesh->SetSimulatePhysics(true);
	FDetachmentTransformRules rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(rules);

	UpdateWidgetInstanceVisibility(ReloadBarInstance, false);
	UpdateWidgetInstanceVisibility(AmmoBarInstance, false);
}

void AWeaponBase::Select()
{
	Mesh->SetRenderCustomDepth(true);
}

void AWeaponBase::Deselect()
{
	Mesh->SetRenderCustomDepth(false);
}