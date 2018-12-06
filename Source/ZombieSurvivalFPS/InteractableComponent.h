// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DELEGATE(SignatureOnSelect);
DECLARE_DELEGATE(SignatureOnDeselect);
DECLARE_DELEGATE(SignatureOnUse);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API UInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UPROPERTY(EditAnywhere)
	UBoxComponent * Box;

	//Setup function delegates to call on Use, Select, Deselect, HoverBegin, HoverEnd
	void InitializeDelegates(SignatureOnSelect * pOnSelect, SignatureOnDeselect * pOnDeselect, SignatureOnUse * pOnUse);

	void Use();

	void Select();
	
	void Deselect();

private:
	SignatureOnSelect * OnSelectDelegate;
	SignatureOnDeselect * OnDeselectDelegate;
	SignatureOnUse * OnUseDelegate;
};