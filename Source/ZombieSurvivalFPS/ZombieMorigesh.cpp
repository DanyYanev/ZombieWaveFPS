// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieMorigesh.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "ZombieBarrier.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "ZombieAI.h"
#include "Engine/World.h"


// Sets default values
AZombieMorigesh::AZombieMorigesh()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);

	Head->AttachToComponent(GetCapsuleComponent(), rules);
	Body->AttachToComponent(GetCapsuleComponent(), rules);


	Health = 100;
	Speed = 1;
	AttackDamage = 100;
}

// Called when the game starts or when spawned
void AZombieMorigesh::BeginPlay()
{
	Super::BeginPlay();

	Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnHeadshot);
	Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieMorigesh::OnBodyshot);
}

void AZombieMorigesh::EndGame(bool Won)
{
	if (Won) {
		bIsCelebrating = true;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("ZOMBIE ALIVE AND GAME WON"));

	AZombieAI * ZombieAI = Cast<AZombieAI>(GetController());

	if (ZombieAI) {
		ZombieAI->BrainComponent->StopLogic(TEXT("Game Over"));
	}

}

// Called every frame
void AZombieMorigesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation((GetActorForwardVector() * GetActorRotation().Vector().GetSafeNormal()) * 10);
	//FVector Dir = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	//LaunchCharacter(Dir.GetSafeNormal() * 100, true, true);
	//LaunchCharacter(FVector(0.f, 0.f, 100.f), true, true);
}

// Called to bind functionality to input
void AZombieMorigesh::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieMorigesh::DealDamage(AActor * Target)
{
	if (IsValid(Target))
		UGameplayStatics::ApplyDamage(Target, GetAttackDamage(), GetController(), this, nullptr);
}

float AZombieMorigesh::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Already dying dont try to render its killing twice
	if (bIsDying)
		return 0.0f;

	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Dealt: %d, Remaining: %d"), Damage, Health);

	if (Health <= 0) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		//Destroy();
		bIsDying = true;
		UE_LOG(LogTemp, Warning, TEXT("Death"));

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				//ZombieGameMode->ZombieDeath(this);
			}
		}
	}

	return 0.0f;
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

		UE_LOG(LogTemp, Warning, TEXT("Headshot attempting to deal: %d"), projectile->GetDamage() * 2);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 2, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
}

void AZombieMorigesh::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile) {

		UE_LOG(LogTemp, Warning, TEXT("Body shot attempting to deal: %d"), projectile->GetDamage() * 1);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);

		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}

}

