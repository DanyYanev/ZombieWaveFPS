// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "ZombieSurvivalFPSProjectile.h"
#include "ZombieSurvivalFPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head"));
	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));

	Head->SetupAttachment(GetCapsuleComponent());
	Body->SetupAttachment(GetCapsuleComponent());


	Health = 100;
	Speed = 1;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	Head->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnHeadshot);
	Body->OnComponentBeginOverlap.AddDynamic(this, &AZombieCharacter::OnBodyshot);
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation((GetActorForwardVector() * GetActorRotation().Vector().GetSafeNormal()) * 10);
	FVector Dir = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation();
	//LaunchCharacter(Dir.GetSafeNormal() * 100, true, true);
	LaunchCharacter(FVector(0.f, 0.f, 100.f), true, true);
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombieCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Dealt: %d, Remaining: %d"), Damage, Health);

	if (Health <= 0) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Death"));

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
			if (ZombieGameMode) {
				ZombieGameMode->ZombieDeath();
			}
		}
	}

	return 0.0f;
}

void AZombieCharacter::OnHeadshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile != nullptr) {

		AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

		if (GameMode) {
			AZombieSurvivalFPSGameMode * ZombieGameMode =  Cast<AZombieSurvivalFPSGameMode>(GameMode);
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

void AZombieCharacter::OnBodyshot(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AZombieSurvivalFPSProjectile* projectile = Cast<AZombieSurvivalFPSProjectile>(OtherActor);

	if (projectile) {

		UE_LOG(LogTemp, Warning, TEXT("Body shot attempting to deal: %d"), projectile->GetDamage() * 1);
		UGameplayStatics::ApplyPointDamage(this, projectile->GetDamage() * 1, GetActorLocation(), SweepResult, nullptr, OtherActor, nullptr);
		
		//Somehow calls TakeDamage of OtherActor
		OtherActor->Destroy();
	}
	
}


