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

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	Locked = CreateDefaultSubobject<UMaterial>(TEXT("LockedM"));
	Purchaseable = CreateDefaultSubobject<UMaterial>(TEXT("PurchaseableM"));
	Unlocked = CreateDefaultSubobject<UMaterial>(TEXT("UnlockedM"));

	SetRootComponent(Mesh);

	InteractableComponent->SetupAttachment(Mesh);
}


// Called when the game starts
void AInteractableButton::BeginPlay()
{
	Super::BeginPlay();

	SetState(EButtonState::VE_Locked);

	OnHoverBeginDelegate.BindUFunction(this, TEXT("HoverBegin"));
	OnHoverEndDelegate.BindUFunction(this, TEXT("HoverEnd"));
	OnUseDelegate.BindUFunction(this, TEXT("Use"));

	InteractableComponent->InitializeDelegates(&OnHoverBeginDelegate, &OnHoverEndDelegate, &OnUseDelegate);

}

void AInteractableButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableButton::SetState(EButtonState::State NewState)
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
	SetState(EButtonState::VE_Locked);
}

void AInteractableButton::HoverBegin()
{
	Mesh->SetRenderCustomDepth(true);
}

void AInteractableButton::HoverEnd()
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
		} else
			UE_LOG(LogTemp, Error, TEXT("OnLevelPurchasedDelegate is null"));
	}

	//Play Sound

	UE_LOG(LogTemp, Warning, TEXT("Useed button :)"));

	//AZombieSurvivalFPSGameMode * GameMode = Cast<AZombieSurvivalFPSGameMode>(GetWorld()->GetAuthGameMode());
	//Notify StatHolder for purchase
}