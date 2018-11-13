// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieMorigesh.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieBarrier.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ZombieAI.h"
#include "Engine/World.h"


// Sets default values
AZombieMorigesh::AZombieMorigesh()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadHitBox"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHitBox"));

	ProjectileOffSet = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileOffSet"));

	ProjectileOffSet->SetupAttachment(GetMesh(), FName("FX_Dagger"));
	Head->SetupAttachment(GetMesh(), FName("headSocket"));
	Body->SetupAttachment(GetMesh(), FName("bodySocket"));

	Health = 100;
	Speed = 2;
	AttackDamage = 150;
}

// Called when the game starts or when spawned
void AZombieMorigesh::BeginPlay()
{
	Super::BeginPlay();

	if (Head) {
		Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnHeadshot);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("HeadHitbox is NULL"));


	if (Body) {
		Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnBodyshot);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("BodyHitbox is NULL"));
}

void AZombieMorigesh::OnHeadshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile != nullptr) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->UpdateCurrentScoreBy(20);
			}
		}

		//UE_LOG(LogTemp, Warning, TEXT("Headshot attempting to deal: %d"), projectile->GetDamage() * 2);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 2, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
}

void AZombieMorigesh::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile) {

		//UE_LOG(LogTemp, Warning, TEXT("Body shot attempting to deal: %d"), projectile->GetDamage() * 1);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}

}

