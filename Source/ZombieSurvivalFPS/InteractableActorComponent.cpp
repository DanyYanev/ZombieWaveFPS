// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActorComponent.h"


// Sets default values for this component's properties
UInteractableActorComponent::UInteractableActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	Box = CreateDefaultSubobject<UBoxComponent>("Collider");

	//USceneComponent * SceneComp = Cast<USceneComponent>(&(GetOwner()->GetComponentByClass(USceneComponent::StaticClass())[0]));

	//Box->SetupAttachment(SceneComp);
	// ...
}
