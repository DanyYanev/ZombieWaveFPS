// Fill out your copyright notice in the Description page of Project Settings.

#include "Scoreboard.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "ZombieSurvivalFPSGameMode.h"


// Sets default values
AScoreboard::AScoreboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	

	Score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Score"));
	Money = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Money"));
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText"));
	MoneyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MoneyText"));
	Wave = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Wave"));
	Countdown = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Countdown"));

	SetRootComponent(Mesh);
	
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

	AZombieSurvivalFPSGameMode * GameMode = Cast<AZombieSurvivalFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		GameMode->AttachScoreboard(this);
		Wave->SetText("");
		Countdown->SetText("");
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("GameMode Cast Failed"));
	}
	
}

// Called every frame
void AScoreboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreboard::UpdateScore(int Value)
{
	Score->SetText(FString::FromInt(Value));
}

void AScoreboard::UpdateMoney(int Value)
{
	Money->SetText(FString::FromInt(Value) + FString("$"));
}

void AScoreboard::UpdateWave(int Value)
{
	Wave->SetText(FString("Wave ") + FString::FromInt(Value));
}

void AScoreboard::UpdateCountdown(int Value)
{
	Countdown->SetText(FString("in ") + FString::FromInt(Value));
}

void AScoreboard::ClearCountdown()
{
	Countdown->SetText("");
}

