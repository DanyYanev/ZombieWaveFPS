// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ZombieBase.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

	void EndGame(bool Won);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack(AActor * Target);

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * BehaviorTree;

	UFUNCTION()
	void DealDamage(AActor * Target);

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE int GetAttackDamage() const { return AttackDamage; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category = Stats, EditDefaultsOnly)
	int Health;

	UPROPERTY(Category = Stats, EditDefaultsOnly)
	int Speed;

	UPROPERTY(Category = Stats, EditDefaultsOnly)
	int AttackDamage;

	UPROPERTY(VisibleAnywhere)
	bool bIsDying = false;

};
