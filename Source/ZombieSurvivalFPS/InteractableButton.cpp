// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableButton.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "Engine/World.h"


// Sets default values for this component's properties
AInteractableButton::AInteractableButton()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(Mesh);

	Locked = CreateDefaultSubobject<UMaterial>(TEXT("LockedM"));
	Purchaseable = CreateDefaultSubobject<UMaterial>(TEXT("PurchaseableM"));
	Unlocked = CreateDefaultSubobject<UMaterial>(TEXT("UnlockedM"));

}


// Called when the game starts
void AInteractableButton::BeginPlay()
{
	Super::BeginPlay();

	OnSelectDelegate.BindUFunction(this, TEXT("Select"));
	OnDeselectDelegate.BindUFunction(this, TEXT("Deselect"));
	OnUseDelegate.BindUFunction(this, TEXT("Use"));

	InteractableComponent->InitializeDelegates(&OnSelectDelegate, &OnDeselectDelegate, &OnUseDelegate);
}

void AInteractableButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableButton::SetState(EButtonState NewState)
{
	CurrentState = NewState;
	
	if (CurrentState == EButtonState::VE_Locked) {
		Mesh->SetMaterial(0, Locked);
		UE_LOG(LogTemp, Warning, TEXT("Button set to VE_Locked"));
	}
	else if (CurrentState == EButtonState::VE_Purchasable) {
		Mesh->SetMaterial(0, Purchaseable);
		UE_LOG(LogTemp, Warning, TEXT("Button set to VE_Purchasable"));
	} 
	else if (CurrentState == EButtonState::VE_Unlocked) {
		Mesh->SetMaterial(0, Unlocked);
		UE_LOG(LogTemp, Warning, TEXT("Button set to VE_Unlocked"));
	}
}

void AInteractableButton::InitializeButton(SignatureOnLevelPurchased * pOnPurchasedDelegate)
{
	OnLevelPurchasedDelegate = pOnPurchasedDelegate;
}

void AInteractableButton::Select()
{
	Mesh->SetRenderCustomDepth(true);
}

void AInteractableButton::Deselect()
{
	Mesh->SetRenderCustomDepth(false);
}

void AInteractableButton::Use()
{
	if (CurrentState == EButtonState::VE_Purchasable) {
		if (OnLevelPurchasedDelegate) {
			if (!OnLevelPurchasedDelegate->ExecuteIfBound()) {
				UE_LOG(LogTemp, Error, TEXT("OnLevelPurchasedDelegate not bound"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("OnLevelPurchasedDelegate is null"));
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Useed button :)"));

}
