// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DELEGATE(SignatureOnFunction);
DECLARE_DELEGATE_OneParam(SignatureOnBeginGrab, USceneComponent*)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API UInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UPROPERTY(EditAnywhere)
	UBoxComponent * Box;

	//Setup function delegates to call on BeginUse, EndUse, Select, Deselect, HoverBegin, HoverEnd
	void InitializeDelegates(SignatureOnFunction * pOnBeginUse, SignatureOnFunction * pOnEndUse = NULL, SignatureOnFunction * pOnSelect = NULL, SignatureOnFunction * pOnDeselect = NULL, SignatureOnBeginGrab * pOnBeginGrab = NULL, SignatureOnFunction * pOnEndGrab = NULL);

	void BeginUse();

	void EndUse();

	void BeginGrab(USceneComponent * AttachActor);

	void EndGrab();

	void Select();
	
	void Deselect();

private:
	SignatureOnFunction * OnBeginUseDelegate;
	SignatureOnFunction * OnEndUseDelegate;
	SignatureOnBeginGrab * OnBeginGrabDelegate;
	SignatureOnFunction * OnEndGrabDelegate;
	SignatureOnFunction * OnSelectDelegate;
	SignatureOnFunction * OnDeselectDelegate;
};