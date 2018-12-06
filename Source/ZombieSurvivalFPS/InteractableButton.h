// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InteractableComponent.h"
#include "InteractableButton.generated.h"

//Enum class for Button States
UENUM()
enum class EButtonState : uint8
{
	VE_Locked			UMETA(DisplayName = "Locked"),
	VE_Unlocked			UMETA(DisplayName = "Unlocked"),
	VE_Purchasable		UMETA(DisplayName = "Purchasable")
};

DECLARE_DELEGATE(SignatureOnLevelPurchased);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZOMBIESURVIVALFPS_API AInteractableButton : public AActor
{
	GENERATED_BODY()

public:	
	AInteractableButton();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent * Mesh;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial * Locked;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial * Purchaseable;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial * Unlocked;

	UPROPERTY(EditAnywhere)
	UInteractableComponent * InteractableComponent;

	UFUNCTION()
	void Use();

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Deselect();

	void SetState(EButtonState NewState);

	void InitializeButton(SignatureOnLevelPurchased * pOnPurchasedDelegate);

protected:
	virtual void BeginPlay() override;

private:
	EButtonState CurrentState = EButtonState::VE_Locked;

	SignatureOnSelect OnSelectDelegate;

	SignatureOnDeselect OnDeselectDelegate;

	SignatureOnUse OnUseDelegate;

	SignatureOnLevelPurchased * OnLevelPurchasedDelegate;	
};
