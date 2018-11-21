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

	UFUNCTION(BlueprintCallable)
	bool IsDying() const { return bIsDying; }

	UFUNCTION(BlueprintCallable)
	bool IsCelebrating() const { return bIsCelebrating; }

	UFUNCTION()
	void DealDamage(AActor * Target);

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//FORCEINLINE class UCapsuleComponent* GetHeadComp() const { return Head; }

	//FORCEINLINE class UCapsuleComponent* GetBodyComp() const { return Body; }

	FORCEINLINE int GetAttackDamage() const { return AttackDamage; }

protected:
	virtual void BeginPlay() override;

	/*
	UPROPERTY(Category = Hitboxes, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent * Head;

	UPROPERTY(Category = Hitboxes, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent * Body;
	*/
	UPROPERTY(Category = Stats, EditAnywhere)
	int Health;

	UPROPERTY(Category = Stats, EditAnywhere)
	int Speed;

	UPROPERTY(Category = Stats, EditAnywhere)
	int AttackDamage;

	UPROPERTY(EditAnywhere)
	bool bIsDying = false;

	UPROPERTY(EditAnywhere)
	bool bIsCelebrating = false;

};
