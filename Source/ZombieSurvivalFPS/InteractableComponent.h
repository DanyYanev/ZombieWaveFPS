// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DELEGATE(SignatureOnFunction);
DECLARE_DELEGATE_OneParam(SignatureOnBeginGrab, USceneComponent*)

/** Provides interface for 4 programmable actions*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API UInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractableComponent();

	UPROPERTY(EditAnywhere)
	UBoxComponent * Box;

	//UPROPERTY(EditAnywhere)
	//bool bIsActive = true;

	//Setup function delegates to call on BeginUse, EndUse, Select, Deselect, HoverBegin, HoverEnd
	void InitializeUseDelegates(SignatureOnFunction * pOnBeginUse, SignatureOnFunction * pOnEndUse = NULL);
	void InitializeSelectDelegates(SignatureOnFunction * pOnSelect, SignatureOnFunction * pOnDeselect = NULL);
	void InitializeGrabDelegates(SignatureOnBeginGrab * pOnBeginGrab, SignatureOnFunction * pOnEndGrab = NULL);
	void InitializeActionDelegates(SignatureOnFunction * pOnBeginAction, SignatureOnFunction * pOnEndAction = NULL);

	bool BeginUse();

	bool EndUse();

	bool BeginAction();

	bool EndAction();

	bool BeginGrab(USceneComponent * AttachActor);

	bool EndGrab();

	bool Select();
	
	bool Deselect();

private:
	SignatureOnFunction * OnBeginUseDelegate;
	SignatureOnFunction * OnEndUseDelegate;

	SignatureOnFunction * OnBeginActionDelegate;
	SignatureOnFunction * OnEndActionDelegate;

	SignatureOnBeginGrab * OnBeginGrabDelegate;
	SignatureOnFunction * OnEndGrabDelegate;

	SignatureOnFunction * OnSelectDelegate;
	SignatureOnFunction * OnDeselectDelegate;
};