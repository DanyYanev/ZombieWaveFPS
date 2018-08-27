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
	//ColorChanging
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
		SetState(EButtonState::VE_Unlocked);
	}

	//Play Sound

	UE_LOG(LogTemp, Warning, TEXT("Useed button :)"));

	//AZombieSurvivalFPSGameMode * GameMode = Cast<AZombieSurvivalFPSGameMode>(GetWorld()->GetAuthGameMode());
	//Notify StatHolder for purchase
}