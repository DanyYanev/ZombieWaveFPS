// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UI/OneParamWidget.h"
#include "ZombieBase.generated.h"

UCLASS(Abstract)
class ZOMBIESURVIVALFPS_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * BehaviorTree;

	UPROPERTY(EditAnywhere, Category = Widget)
	UWidgetComponent* HealthBarWidgetComponent;

	void OnGameEnded(bool Won);

	//ZombieAI Calls this for an attack to be issued
	virtual void Attack(AActor * Target);

	UFUNCTION(BlueprintCallable)
	void DealDamageToTargetActor(AActor * DamagedActor);

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE int GetAttackDamage() const { return AttackDamage; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category = Stats, EditAnywhere)
	int MaxHealth;

	int Health;

	UPROPERTY(Category = Stats, EditAnywhere)
	int Speed;

	UPROPERTY(Category = Stats, EditAnywhere)
	int AttackDamage;

	UPROPERTY(VisibleAnywhere)
	bool bIsDying = false;

private:
	UOneParamWidget* HealthBarInstance;
};
