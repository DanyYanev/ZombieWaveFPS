// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 *  Class created for UpdateHealth to be called from code.
 */
UCLASS()
class ZOMBIESURVIVALFPS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//HealthPercentage ranges from 0-1
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(float HealthPercentage);
	
};
