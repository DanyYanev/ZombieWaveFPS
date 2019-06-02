// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ZombieSurivalFPSVRPawn.generated.h"

UCLASS()
class ZOMBIESURVIVALFPS_API AZombieSurivalFPSVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZombieSurivalFPSVRPawn();

	/** Origin scene component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class UCapsuleComponent* VROrigin;

	/** Player Camera */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class UCameraComponent* Camera;

	/** Player Head Hitbox*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class USphereComponent* Head;

	/** Floating Pawn Movement Component*/
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* MovementComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller Hand mesh (right hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* R_HandMesh;

	/** Motion controller grab sphere (right hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USphereComponent* R_GrabSphere;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	/** Motion controller Hand mesh (left hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* L_HandMesh;

	/** Motion controller grab sphere (left hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USphereComponent* L_GrabSphere;

	/** Intervals at which to rotate Camera when turning (In degreese) */
	UPROPERTY(EditAnywhere, Category = Gameplay)
	float TurnInterval = 45.f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float SpeedMultiplier = 0.5f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** Handles turning */
	void Turn(float Val);

	void R_BeginUse();
	void R_EndUse();

	void R_BeginGrab();
	void R_EndGrab();

	void R_BeginAction();
	void R_EndAction();

	void L_BeginUse();
	void L_EndUse();

	void L_BeginGrab();
	void L_EndGrab();

	void L_BeginAction();
	void L_EndAction();

	void Pause();

private:

	/**Allows one call of turn without releasing the joystick to default position*/
	bool bCanTurn = true;

	//References to the currently selected interactable component
	class UInteractableComponent* R_InteractableComponent;

	class UInteractableComponent* L_InteractableComponent;

	FTimerHandle SuicideTimerHandler;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void R_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void R_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void L_OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void L_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION()
	void QuitGame() { FGenericPlatformMisc::RequestExit(false); }
};
