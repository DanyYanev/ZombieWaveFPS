// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelUpShop.h"


// Sets default values
ALevelUpShop::ALevelUpShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Money = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Money"));
	TitleText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitleText"));
	DamageStatComponent = CreateDefaultSubobject<UUpgradableStatItemComponent>(TEXT("DamageStatComponent"));

	SetRootComponent(Mesh);

	Money->SetupAttachment(Mesh);
	TitleText->SetupAttachment(Mesh);
	DamageStatComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ALevelUpShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelUpShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelUpShop::UpdateMoney(int Value)
{
	Money->SetText(FText().FromString(FString::FromInt(Value) + FString("$")));
}

