// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableComponent.h"

#define INTERACTABLE_CHANNEL ECC_GameTraceChannel2

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

	Box->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(INTERACTABLE_CHANNEL);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(INTERACTABLE_CHANNEL, ECollisionResponse::ECR_Overlap);
	//Box->UpdateCollisionProfile();
}

void UInteractableComponent::InitializeUseDelegates(SignatureOnFunction * pOnBeginUse, SignatureOnFunction * pOnEndUse)
{
	if (pOnBeginUse) {
		OnBeginUseDelegate = pOnBeginUse;
	}
	if (pOnEndUse) {
		OnEndUseDelegate = pOnEndUse;
	}
}

void UInteractableComponent::InitializeSelectDelegates(SignatureOnFunction * pOnSelect, SignatureOnFunction * pOnDeselect)
{
	if (pOnSelect) {
		OnSelectDelegate = pOnSelect;
	}
	if (pOnDeselect) {
		OnDeselectDelegate = pOnDeselect;
	}
}

void UInteractableComponent::InitializeGrabDelegates(SignatureOnBeginGrab * pOnBeginGrab, SignatureOnFunction * pOnEndGrab)
{
	if (pOnBeginGrab) {
		OnBeginGrabDelegate = pOnBeginGrab;
	}
	if (pOnEndGrab) {
		OnEndGrabDelegate = pOnEndGrab;
	}
}

void UInteractableComponent::InitializeActionDelegates(SignatureOnFunction * pOnBeginAction, SignatureOnFunction * pOnEndAction)
{
	if (pOnBeginAction) {
		OnBeginActionDelegate = pOnBeginAction;
	}
	if (pOnEndAction) {
		OnEndActionDelegate = pOnEndAction;
	}
}

bool UInteractableComponent::BeginUse()
{
	if (bIsInteractionActive) {
		if (OnBeginUseDelegate) {
			if (OnBeginUseDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}

	return false;
}

bool UInteractableComponent::EndUse()
{
	if (bIsInteractionActive) {
		if (OnEndUseDelegate) {
			if (OnEndUseDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}

	return false;
}

bool UInteractableComponent::BeginAction()
{
	if (bIsInteractionActive) {
		if (OnBeginActionDelegate) {
			if (OnBeginActionDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}
	return false;
}

bool UInteractableComponent::EndAction()
{
	if (bIsInteractionActive) {
		if (OnEndActionDelegate) {
			if (OnEndActionDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}
	return false;
}

bool UInteractableComponent::Select()
{
	if (bIsInteractionActive) {
		if (OnSelectDelegate) {
			if (OnSelectDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}
	return false;
}

bool UInteractableComponent::Deselect()
{
	if (bIsInteractionActive) {
		if (OnDeselectDelegate) {
			if (OnDeselectDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}
	return false;
}

bool UInteractableComponent::BeginGrab(USceneComponent * AttachActor)
{
	if (bIsInteractionActive) {
		if (OnBeginGrabDelegate) {
			if (OnBeginGrabDelegate->ExecuteIfBound(AttachActor)) {
				return true;
			}
		}
	}
	return false;
}

bool UInteractableComponent::EndGrab()
{
	if (bIsInteractionActive) {
		if (OnEndGrabDelegate) {
			if (OnEndGrabDelegate->ExecuteIfBound()) {
				return true;
			}
		}
	}
	return false;
}