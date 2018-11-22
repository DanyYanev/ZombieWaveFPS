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

	//Box->SetupAttachment(GetAttachmentRoot());
	Box->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(INTERACTABLE_CHANNEL);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(INTERACTABLE_CHANNEL, ECollisionResponse::ECR_Overlap);
	Box->UpdateCollisionProfile();

	// ...
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	//Box->OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnHoverBegin);
	//Box->OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnHoverEnd);

}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UInteractableComponent::InitializeDelegates(SignatureOnSelect * pOnSelect, SignatureOnDeselect * pOnDeselect, SignatureOnUse * pOnUse)
{
	if(pOnSelect)
		OnSelectDelegate = pOnSelect;
	if(pOnDeselect)
		OnDeselectDelegate = pOnDeselect;
	if(pOnUse)
		OnUseDelegate = pOnUse;
}

void UInteractableComponent::Use()
{
	if (OnUseDelegate) {
		if (!OnUseDelegate->ExecuteIfBound()) {
			UE_LOG(LogTemp, Warning, TEXT("OnUseDelegate Not Bound"));
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("OnUseDelegate is null"));
	
}

void UInteractableComponent::Select()
{
	if (OnSelectDelegate) {
		if (!OnSelectDelegate->ExecuteIfBound()) {
			UE_LOG(LogTemp, Warning, TEXT("OnSelectDelegate Not Bound"));
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("OnSelectDelegate is null"));
}

void UInteractableComponent::Deselect()
{
	if (OnDeselectDelegate) {
		if (!OnDeselectDelegate->ExecuteIfBound()) {
			UE_LOG(LogTemp, Warning, TEXT("OnDeselectDelegate Not Bound"));
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("OnDeselectDelegate is null"));
}

/*
void UInteractableComponent::OnHoverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OnHoverBeginDelegate) {
		if (!OnHoverBeginDelegate->ExecuteIfBound()) {
			UE_LOG(LogTemp, Warning, TEXT("OnHoverBeginDelegate Not Bound"));
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("OnHoverBeginDelegate is null"));
}

void UInteractableComponent::OnHoverEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnHoverEndDelegate) {
		if (!OnHoverEndDelegate->ExecuteIfBound()) {
			UE_LOG(LogTemp, Warning, TEXT("OnHoverEndDelegate Not Bound"));
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("OnHoverEndDelegate is null"));
}
*/