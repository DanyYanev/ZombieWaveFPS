// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "WeaponBase.generated.h"

UCLASS(abstract)
class ZOMBIESURVIVALFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditDefaultsOnly, Category = Statistic)
	float Damage = 0;

	//Firerate in rounds per minute
	UPROPERTY(EditDefaultsOnly, Category = Statistic)
	float FireRate = 100;

	UPROPERTY(EditDefaultsOnly, Category = Statistic)
	bool bIsAutomatic = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* MuzzleOffset;

	UPROPERTY(EditDefaultsOnly)
	class UInteractableComponent* InteractableComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();

	UFUNCTION()
	void BeginUse();

	UFUNCTION()
	void EndUse();

	UFUNCTION()
	void BeginGrab(USceneComponent * AttachActor);

	UFUNCTION()
	void EndGrab();

	UFUNCTION()
	void Select();

	UFUNCTION()
	void Deselect();

private:

	FTimerHandle AutomaticFireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	SignatureOnFunction OnBeginUseDelegate;
	SignatureOnFunction OnEndUseDelegate;
	SignatureOnFunction OnSelectDelegate;
	SignatureOnFunction OnDeselectDelegate;
	SignatureOnBeginGrab OnBeginGrabDelegate;
	SignatureOnFunction OnEndGrabDelegate;

};
