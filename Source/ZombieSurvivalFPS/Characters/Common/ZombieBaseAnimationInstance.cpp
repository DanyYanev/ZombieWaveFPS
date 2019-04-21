// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Common/ZombieBaseAnimationInstance.h"

UZombieBaseAnimationInstance::UZombieBaseAnimationInstance() {
	
	bIsAttacking = false;
	bIsCheering = false;
	bIsRunning = false;
	bIsDying = false;
}

void UZombieBaseAnimationInstance::NativeBeginPlay(){
	//Seed owning Pawn as ZombieBase
	OwningPawn = Cast<AZombieBase>(GetOwningActor());
	if (!IsValid(OwningPawn)) {
		UE_LOG(LogTemp, Error, TEXT("Owning pawn does not derive ZombieBase class!"));
	}
}

void UZombieBaseAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwningPawn)) {
		if (OwningPawn->GetVelocity().Size() > 0) {
			bIsRunning = true;
		}
		else {
			bIsRunning = false;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("OwningPawn not valid while AnimInstance keeps updating!"));
	}
}

void UZombieBaseAnimationInstance::SetIsAttacking(bool IsAttacking) {
	bIsAttacking = IsAttacking;
}

void UZombieBaseAnimationInstance::SetIsCheering(bool IsCheering) {
	bIsCheering = IsCheering;
}

void UZombieBaseAnimationInstance::SetIsDying(bool IsDying) {
	bIsDying = IsDying;
}

void UZombieBaseAnimationInstance::SetIsRunning(bool IsRunning) {
	bIsRunning = IsRunning;
}

void UZombieBaseAnimationInstance::SetTarget(AActor * newTarget) {
	Target = newTarget;
}


