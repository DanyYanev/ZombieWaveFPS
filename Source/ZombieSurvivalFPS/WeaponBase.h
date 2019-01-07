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
	AWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Statistic)
	float Damage = 0;

	/** Firerate in rounds per minute */
	UPROPERTY(EditAnywhere, Category = Statistic)
	float FireRate = 100;

	UPROPERTY(EditAnywhere, Category = Statistic)
	float MagazineSize = 0;

	/** In seconds */
	UPROPERTY(EditAnywhere, Category = Statistic)
	float ReloadTime = 0;

	UPROPERTY(EditAnywhere, Category = Statistic)
	bool bIsAutomatic = false;

	/** Sound to play each time gun is fired */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireCue;

	/** Sound to play each time gun is reloaded */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* ReloadCue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsReloading = false;

	int BulletsInMagazine;

	void FinishReload();

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

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Reload();

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
	SignatureOnFunction OnReloadDelegate;

};
