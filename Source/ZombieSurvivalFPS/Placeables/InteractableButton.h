// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Placeables/InteractableComponent.h"
#include "InteractableButton.generated.h"

//Enum class for Button States
UENUM()
enum class EButtonState : uint8
{
	VE_Locked			UMETA(DisplayName = "Locked"),
	VE_Unlocked			UMETA(DisplayName = "Unlocked"),
	VE_Purchasable		UMETA(DisplayName = "Purchasable")
};


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

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	int32 Cost = 0;

	UFUNCTION()
	void Use();

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Deselect();

	FORCEINLINE int32 GetCost() const { return Cost; };

	FORCEINLINE EButtonState GetState() const { return CurrentState; };

	void SetState(EButtonState NewState);

	void InitializeButton(SignatureOnLevelPurchased * pOnPurchasedDelegate);

protected:
	virtual void BeginPlay() override;

	EButtonState CurrentState = EButtonState::VE_Locked;

	SignatureOnFunction OnUseDelegate;

	SignatureOnFunction OnSelectDelegate;

	SignatureOnFunction OnDeselectDelegate;

	SignatureOnFunction OnUseUnlockedDelegate;

	SignatureOnFunction OnUsePurchaseableDelegate;

	SignatureOnFunction OnUseLockedDelegate;

	SignatureOnLevelPurchased* OnLevelPurchasedDelegate;
};