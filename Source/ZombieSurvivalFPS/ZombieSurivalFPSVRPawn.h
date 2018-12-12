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
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VROrigin;

	/** Player Camera */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UCameraComponent* Camera;
	
	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	/** Motion controller Hand mesh (right hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* R_HandMesh;

	/** Motion controller Hand mesh (left hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* L_HandMesh;

	/** Motion controller grab sphere (right hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USphereComponent* R_GrabSphere;

	/** Motion controller grab sphere (left hand) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USphereComponent* L_GrabSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
