// Fill out your copyright notice in the Description page of Project Settings.

#include "Placeables/Scoreboard.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Core/ZombieSurvivalFPSGameMode.h"


// Sets default values
AScoreboard::AScoreboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	Score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Score"));
	Money = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Money"));
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText"));
	MoneyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MoneyText"));
	Wave = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Wave"));
	Countdown = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Countdown"));

	SetRootComponent(Mesh);
	
	InteractableComponent->SetupAttachment(Mesh);
	Score->SetupAttachment(Mesh);
	Money->SetupAttachment(Mesh);
	ScoreText->SetupAttachment(Mesh);
	MoneyText->SetupAttachment(Mesh);
	Wave->SetupAttachment(Mesh);
	Countdown->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AScoreboard::BeginPlay()
{
	Super::BeginPlay();

	OnUseDelegate.BindUFunction(this, TEXT("Use"));
	OnSelectDelegate.BindUFunction(this, TEXT("Select"));
	OnDeselectDelegate.BindUFunction(this, TEXT("Deselect"));

	if (IsValid(InteractableComponent)) {
		InteractableComponent->InitializeUseDelegates(&OnUseDelegate);
		InteractableComponent->InitializeSelectDelegates(&OnSelectDelegate, &OnDeselectDelegate);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Interactable Component is not valid."));
	}

	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (IsValid(GameMode)) {
		AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);

		if (IsValid(ZombieGameMode)) {
			ZombieGameMode->AttachScoreboard(this);
			Wave->SetText(FText().FromString(FString("")));
			Countdown->SetText(FText().FromString(FString("")));
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
	}
}

void AScoreboard::UpdateScore(int Value)
{
	Score->SetText(FText().FromString(FString::FromInt(Value)));
}

void AScoreboard::UpdateMoney(int Value)
{
	Money->SetText(FText().FromString(FString::FromInt(Value) + FString("$")));
}

void AScoreboard::UpdateWave(int Value)
{
	Wave->SetText(FText().FromString(FString("Wave ") + FString::FromInt(Value)));
}

void AScoreboard::UpdateCountdown(int Value)
{
	Countdown->SetText(FText().FromString(FString("in ") + FString::FromInt(Value)));
}

void AScoreboard::ClearCountdown()
{
	Countdown->SetText(FText().FromString(FString("")));
}

void AScoreboard::Select()
{
	Mesh->SetRenderCustomDepth(true);
}

void AScoreboard::Deselect()
{
	Mesh->SetRenderCustomDepth(false);
}

void AScoreboard::Use()
{
	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (IsValid(GameMode)) {
		AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (IsValid(ZombieGameMode)) {
				ZombieGameMode->QuickStartWave();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
			}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't retrieve a valid GameMode."));
	}
}

