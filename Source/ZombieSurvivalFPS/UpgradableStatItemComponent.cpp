// Fill out your copyright notice in the Description page of Project Settings.

#include "UpgradableStatItemComponent.h"


// Sets default values for this component's properties
AUpgradableStatItemComponent::AUpgradableStatItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	StatName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RootBox"));

	//StatName->SetupAttachment(this);

	for (int i = 0; i < TotalLevels; i++) {
		AInteractableButton * button = CreateDefaultSubobject<AInteractableButton>(FName(*(FString("Button") + FString::FromInt(i))));
		//button->SetupAttachment(this);
		LevelBoxes.Add(button);
	}

	// ...
}


// Called when the game starts
void AUpgradableStatItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

