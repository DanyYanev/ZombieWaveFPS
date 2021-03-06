// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Placeables/InteractableComponent.h"
#include "WeaponBase.generated.h"


//Amount of times reload widget is updated
#define UPDATE_TICKS 50

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

	bool bIsGrabbed = false;

	int BulletsInMagazine;

	void FinishReload();

	void UpdateReloadBar();

	void UpdateAmmoBar();

	float CalculateLaserBeamDistance();

	void SetLaserBeamDistance(float Distance);

	void UpdateWidgetInstanceVisibility(UUserWidget* Target, bool isVisible);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//When attached the object moves according to this vector. Used to adjust positioning better due to static meshes having different pivot points.
	UPROPERTY(EditAnywhere)
	FVector AttachmentOffset = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LaserMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	class UParticleSystemComponent* LaserBeam;

	UPROPERTY(EditAnywhere, Category = Widget)
	class UWidgetComponent* ReloadBarWidgetComponent;

	UPROPERTY(EditAnywhere, Category = Widget)
	class UWidgetComponent* AmmoBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* MuzzleOffset;

	UPROPERTY(EditDefaultsOnly)
	class UInteractableComponent* InteractableComponent;

	UPROPERTY(VisibleAnywhere)
	bool IsRightHanded = true;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void TickReload();

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

	void SwitchHand();

	void LeftHandGrab();

	void RightHandGrab();

private:

	uint8_t UpdateTickCounter = 0;

	class UOneParamWidget* ReloadBarInstance;
	class UOneParamWidget* AmmoBarInstance;

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
