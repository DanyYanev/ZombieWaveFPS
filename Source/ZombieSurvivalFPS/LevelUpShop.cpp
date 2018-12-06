// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelUpShop.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieSurvivalFPSGameMode.h"


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
			UpdateMoney(0);
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

	if (CurrentLevel < MaxLevel) {
		if (Value >= CostForNextLevel) {
			Buttons[CurrentLevel]->SetState(EButtonState::VE_Purchasable);
			bPurchaseWasAvaiable = true;
		}
		else if (bPurchaseWasAvaiable) {
			Buttons[CurrentLevel]->SetState(EButtonState::VE_Locked);
		}
	}
	
}

void ALevelUpShop::LevelPurchased()
{
	if (IsValid(ZombieGameMode)) {

		UE_LOG(LogTemp, Display, TEXT("Level Purchased!"));

		ZombieGameMode->UpdateCurrentMoneyBy(-(CostForNextLevel));
		Buttons[CurrentLevel]->SetState(EButtonState::VE_Unlocked);

		CurrentLevel++;
		CostForNextLevel *= 2;
		bPurchaseWasAvaiable = false;

		// try and play the sound if specified
		if (PurchaseCue != NULL){
			UGameplayStatics::PlaySoundAtLocation(this, PurchaseCue, GetActorLocation());
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ZombieGameMode reference is not vlaid."));
	}
}

