// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_DELEGATE(SignatureOnHoverBegin);
DECLARE_DELEGATE(SignatureOnHoverEnd);
DECLARE_DELEGATE(SignatureOnUse);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVALFPS_API UInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	SignatureOnHoverBegin * OnHoverBeginDelegate;
	SignatureOnHoverEnd * OnHoverEndDelegate;
	SignatureOnUse * OnUseDelegate;

public:	

	UPROPERTY(EditAnywhere)
	UBoxComponent * Box;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeDelegates(SignatureOnHoverBegin * pOnHoverBegin, SignatureOnHoverEnd * pOnHoverEnd, SignatureOnUse * pOnUse);

	UFUNCTION()
	void OnHoverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnHoverEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};