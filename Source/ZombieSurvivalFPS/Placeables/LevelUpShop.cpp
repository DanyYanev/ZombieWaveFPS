// Fill out your copyright notice in the Description page of Project Settings.

#include "Placeables/LevelUpShop.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ZombieSurvivalFPSGameMode.h"


// Sets default values
ALevelUpShop::ALevelUpShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Money = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Money"));
	TitleText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitleText"));

	SetRootComponent(Mesh);

	Money->SetupAttachment(Mesh);
	TitleText->SetupAttachment(Mesh);
	
}

// Called when the game starts or when spawned
void ALevelUpShop::BeginPlay()
{
	Super::BeginPlay();
	
	OnLevelPurchasedDelegate.BindUFunction(this, TEXT("LevelPurchased"));

	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (IsValid(GameMode)) {
		ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);

		if (IsValid(ZombieGameMode)) {
			ZombieGameMode->AttachLevelUpShop(this);
			ZombieGameMode->UpdateCurrentMoneyBy(0);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
	}

	MaxLevel = Buttons.Num();

	for (int i = 0; i < MaxLevel; i++) {
		Buttons[i]->InitializeButton(&OnLevelPurchasedDelegate);
	}
}		

void ALevelUpShop::UpdateMoney(int Value)
{
	Money->SetText(FText().FromString(FString::FromInt(Value) + FString("$")));

	for (AInteractableButton* Button : Buttons) {
		EButtonState CurrentState = Button->GetState();

		if (CurrentState == EButtonState::VE_Purchasable ||
			CurrentState == EButtonState::VE_Locked) 
		{
			int32 ButtonCost = Button->GetCost();
			if (Value >= ButtonCost) {
				Button->SetState(EButtonState::VE_Purchasable);
			}
			else {
				Button->SetState(EButtonState::VE_Locked);
			}
		}
	}
}

bool ALevelUpShop::LevelPurchased(UObject* ButtonObject)
{
	if (!IsValid(ZombieGameMode)) {
		UE_LOG(LogTemp, Error, TEXT("ZombieGameMode reference is not valid."));
		return false;
	}

	AInteractableButton* Button = Cast<AInteractableButton>(ButtonObject);
	if (!IsValid(Button)) {
		UE_LOG(LogTemp, Error, TEXT("ButtonObject reference is not valid."));
		return false;
	}

	int32 ButtonIndex = -1;
	Buttons.Find(Button, ButtonIndex);

	if (ButtonIndex == -1) {
		UE_LOG(LogTemp, Error, TEXT("Button reference is not registrated in ButtonsArray."));
		return false;
	}

	ZombieGameMode->UpdateCurrentMoneyBy(-(Button->GetCost()));
	Buttons[ButtonIndex]->SetState(EButtonState::VE_Unlocked);

	// try and play the sound if specified
	if (PurchaseCue != NULL) {
		UGameplayStatics::PlaySoundAtLocation(this, PurchaseCue, GetActorLocation());
	}

	return true;
}

