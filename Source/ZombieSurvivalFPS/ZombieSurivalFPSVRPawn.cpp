// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSurivalFPSVRPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

// Sets default values
AZombieSurivalFPSVRPawn::AZombieSurivalFPSVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VROrigin);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	L_MotionController->SetupAttachment(RootComponent);

	R_HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_HandMesh"));
	R_HandMesh->SetOnlyOwnerSee(true);
	R_HandMesh->bCastDynamicShadow = false;
	R_HandMesh->CastShadow = false;
	R_HandMesh->SetupAttachment(R_MotionController);

	L_HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_HandMesh"));
	L_HandMesh->SetOnlyOwnerSee(true);
	L_HandMesh->bCastDynamicShadow = false;
	L_HandMesh->CastShadow = false;
	L_HandMesh->SetupAttachment(L_MotionController);
	//Inverse right hand to become left hand.
	L_HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));

	R_GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("R_GrabSphere"));
	R_GrabSphere->SetupAttachment(R_MotionController);

	L_GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("L_GrabSphere"));
	L_GrabSphere->SetupAttachment(L_MotionController);

}

// Called when the game starts or when spawned
void AZombieSurivalFPSVRPawn::BeginPlay()
{
	Super::BeginPlay();

	//Required for shipping.
	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);

	//Set tracking origin for Vive/Oculus
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

// Called every frame
void AZombieSurivalFPSVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieSurivalFPSVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZombieSurivalFPSVRPawn::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AZombieSurivalFPSVRPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZombieSurivalFPSVRPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AZombieSurivalFPSVRPawn::Turn);
}

void AZombieSurivalFPSVRPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AZombieSurivalFPSVRPawn::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// Move correspondingly to camera (HMD)
		FVector Direction = Camera->GetForwardVector();
		Direction.Z = 0.f;

		AddMovementInput(Direction, Val * 0.15);
	}
}

void AZombieSurivalFPSVRPawn::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		//  Move correspondingly to camera (HMD)
		FVector Direction = Camera->GetRightVector();
		Direction.Z = 0.f;

		AddMovementInput(Direction, Val * 0.15);
	}
}

void AZombieSurivalFPSVRPawn::Turn(float Val)
{
	UE_LOG(LogTemp, Error, TEXT("%d"), Val);

	if (Val != 0.0f) {
		if (bCanTurn) {
			FRotator Rotation = VROrigin->GetComponentRotation();
			if (Val < 0) {
				Rotation.Yaw += -TurnInterval;
			}
			else {
				Rotation.Yaw += TurnInterval;
			}

			VROrigin->SetWorldRotation(Rotation);
			bCanTurn = false;
		}
	}
	else {
		bCanTurn = true;
	}
}

