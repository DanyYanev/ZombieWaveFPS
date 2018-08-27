// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableComponent.h"
#include "InteractableButton.generated.h"

UENUM()
namespace EButtonState
{
	enum State
	{
		VE_Locked			UMETA(DisplayName = "Locked"),
		VE_Unlocked	UMETA(DisplayName = "Unlocked"),
		VE_Purchasable			UMETA(DisplayName = "Purchasable")
	};
}


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIESURVIVALFPS_API AInteractableButton : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AInteractableButton();

private:
	EButtonState::State CurrentState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	SignatureOnHoverBegin OnHoverBeginDelegate;
	SignatureOnHoverEnd OnHoverEndDelegate;
	SignatureOnUse OnUseDelegate;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere)
	UInteractableComponent * InteractableComponent;

	UFUNCTION()
	void Use();

	UFUNCTION()
	void HoverBegin();

	UFUNCTION()
	void HoverEnd();

	void SetState(EButtonState::State NewState);
};
