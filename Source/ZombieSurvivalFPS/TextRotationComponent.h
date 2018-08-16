// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "TextRotationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API UTextRotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTextRotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<UTextRenderComponent * > TextFields;

	AActor * Target;

	void SetRotationToTarget(UTextRenderComponent *);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddTextComponent(UTextRenderComponent * TextComponent);

	void SetTarget(AActor * NewTarget);
};
