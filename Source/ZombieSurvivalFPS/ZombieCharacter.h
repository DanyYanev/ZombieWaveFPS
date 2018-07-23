// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AZombieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent * Head;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent * Body;

	UPROPERTY(EditAnywhere)
	int Health;

	UPROPERTY(EditAnywhere)
	int Speed;

	UPROPERTY(EditAnywhere)
	int AttackDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * BehaviorTree;

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	virtual void OnHeadshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnBodyshot(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FORCEINLINE class UCapsuleComponent* GetHeadComp() const { return Head; }

	FORCEINLINE class UCapsuleComponent* GetBodyComp() const { return Body; }

	FORCEINLINE int GetAttackDamage() const { return AttackDamage; }

};
