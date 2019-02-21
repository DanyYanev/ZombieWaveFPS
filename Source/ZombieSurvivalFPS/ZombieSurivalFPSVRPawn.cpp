// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSurivalFPSVRPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "InteractableComponent.h"
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
	R_HandMesh->ComponentTags.Add(FName("RIGHT"));
	R_HandMesh->SetupAttachment(R_MotionController);

	L_HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_HandMesh"));
	L_HandMesh->SetOnlyOwnerSee(true);
	L_HandMesh->bCastDynamicShadow = false;
	L_HandMesh->CastShadow = false;
	L_HandMesh->ComponentTags.Add(FName("LEFT"));
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

	R_GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombieSurivalFPSVRPawn::R_OverlapBegin);
	R_GrabSphere->OnComponentEndOverlap.AddDynamic(this, &AZombieSurivalFPSVRPawn::R_OverlapEnd);

	L_GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombieSurivalFPSVRPawn::L_OverlapBegin);
	L_GrabSphere->OnComponentEndOverlap.AddDynamic(this, &AZombieSurivalFPSVRPawn::L_OverlapEnd);
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
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AZombieSurivalFPSVRPawn::Pause).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("R_Grab", IE_Pressed, this, &AZombieSurivalFPSVRPawn::R_BeginGrab);
	PlayerInputComponent->BindAction("R_Grab", IE_Released, this, &AZombieSurivalFPSVRPawn::R_EndGrab);
	PlayerInputComponent->BindAction("R_Use", IE_Pressed, this, &AZombieSurivalFPSVRPawn::R_BeginUse);
	PlayerInputComponent->BindAction("R_Use", IE_Released, this, &AZombieSurivalFPSVRPawn::R_EndUse);
	PlayerInputComponent->BindAction("R_Action", IE_Pressed, this, &AZombieSurivalFPSVRPawn::R_BeginAction);
	PlayerInputComponent->BindAction("R_Action", IE_Released, this, &AZombieSurivalFPSVRPawn::R_EndAction);

	PlayerInputComponent->BindAction("L_Grab", IE_Pressed, this, &AZombieSurivalFPSVRPawn::L_BeginGrab);
	PlayerInputComponent->BindAction("L_Grab", IE_Released, this, &AZombieSurivalFPSVRPawn::L_EndGrab);
	PlayerInputComponent->BindAction("L_Use", IE_Pressed, this, &AZombieSurivalFPSVRPawn::L_BeginUse);
	PlayerInputComponent->BindAction("L_Use", IE_Released, this, &AZombieSurivalFPSVRPawn::L_EndUse);
	PlayerInputComponent->BindAction("L_Action", IE_Pressed, this, &AZombieSurivalFPSVRPawn::L_BeginAction);
	PlayerInputComponent->BindAction("L_Action", IE_Released, this, &AZombieSurivalFPSVRPawn::L_EndAction);


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

	if (Val > 0.15 || Val < -0.15) {
		if (bCanTurn) {
			FRotator Rotation = VROrigin->GetComponentRotation();
			if (Val < 0) {
				Rotation.Yaw += -TurnInterval;
			}
			else {
				Rotation.Yaw += TurnInterval;
			}

			GetController()->SetControlRotation(Rotation);
			bCanTurn = false;
		}
	}
	else {
		bCanTurn = true;
	}
}


void AZombieSurivalFPSVRPawn::R_OverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Select New InteractableComponent if current is invalid 
	if (!IsValid(R_InteractableComponent)) {

		UInteractableComponent * InteractableComponent = OtherActor->FindComponentByClass<UInteractableComponent>();

		if (IsValid(InteractableComponent)) {
			R_InteractableComponent = InteractableComponent;
			R_InteractableComponent->Select();
		}
	}
}

void AZombieSurivalFPSVRPawn::R_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//Remove InteractableActor reference
	if (IsValid(R_InteractableComponent)) {
		if (IsValid(OtherActor)) {
			if (R_InteractableComponent->GetOwner() == OtherActor) {

				R_InteractableComponent->Deselect();
				R_InteractableComponent = NULL;
			}
		}
	}
	else {
		R_InteractableComponent = NULL;
	}
}


void AZombieSurivalFPSVRPawn::L_OverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Select New InteractableComponent if current is invalid 
	if (!IsValid(L_InteractableComponent)) {

		UInteractableComponent * InteractableComponent = OtherActor->FindComponentByClass<UInteractableComponent>();

		if (IsValid(InteractableComponent)) {
			L_InteractableComponent = InteractableComponent;
			L_InteractableComponent->Select();
		}
	}
}

void AZombieSurivalFPSVRPawn::L_OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//Remove InteractableActor reference
	if (IsValid(L_InteractableComponent)) {
		if (IsValid(OtherActor)) {
			if (L_InteractableComponent->GetOwner() == OtherActor) {

				L_InteractableComponent->Deselect();
				L_InteractableComponent = NULL;
			}
		}
	}
	else {
		L_InteractableComponent = NULL;
	}
}

void AZombieSurivalFPSVRPawn::R_BeginUse()
{
	if (IsValid(R_InteractableComponent)) {
		R_InteractableComponent->BeginUse();
	}
}

void AZombieSurivalFPSVRPawn::R_EndUse()
{
	if (IsValid(R_InteractableComponent)) {
		R_InteractableComponent->EndUse();
	}
}

void AZombieSurivalFPSVRPawn::R_BeginGrab()
{
	if (IsValid(R_InteractableComponent)) {
		if (R_InteractableComponent->BeginGrab(R_HandMesh)) {
			R_HandMesh->SetVisibility(false);
		}
	}
}

void AZombieSurivalFPSVRPawn::R_EndGrab()
{
	if (IsValid(R_InteractableComponent)) {
		R_InteractableComponent->EndGrab();
		R_HandMesh->SetVisibility(true);
	}
}

void AZombieSurivalFPSVRPawn::R_BeginAction()
{
	if (IsValid(R_InteractableComponent)) {
		R_InteractableComponent->BeginAction();
	}
}

void AZombieSurivalFPSVRPawn::R_EndAction()
{
	if (IsValid(R_InteractableComponent)) {
		R_InteractableComponent->EndAction();
	}
}

void AZombieSurivalFPSVRPawn::L_BeginUse()
{
	if (IsValid(L_InteractableComponent)) {
		L_InteractableComponent->BeginUse();
	}
}

void AZombieSurivalFPSVRPawn::L_EndUse()
{
	if (IsValid(L_InteractableComponent)) {
		L_InteractableComponent->EndUse();
	}
}

void AZombieSurivalFPSVRPawn::L_BeginGrab()
{
	if (IsValid(L_InteractableComponent)) {
		if (L_InteractableComponent->BeginGrab(L_HandMesh)) {
			L_HandMesh->SetVisibility(false);
		}
	}
}

void AZombieSurivalFPSVRPawn::L_EndGrab()
{
	if (IsValid(L_InteractableComponent)) {
		L_InteractableComponent->EndGrab();
		L_HandMesh->SetVisibility(true);
	}
}

void AZombieSurivalFPSVRPawn::L_BeginAction()
{
	if (IsValid(L_InteractableComponent)) {
		L_InteractableComponent->BeginAction();
	}
}

void AZombieSurivalFPSVRPawn::L_EndAction()
{
	if (IsValid(L_InteractableComponent)) {
		L_InteractableComponent->EndAction();
	}
}

void AZombieSurivalFPSVRPawn::Pause()
{
	/*
	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();

	if (IsValid(GameMode)) {
		AZombieSurvivalFPSGameMode * ZombieGameMode = Cast<AZombieSurvivalFPSGameMode>(GameMode);
		if (IsValid(ZombieGameMode)) {
			ZombieGameMode->SetGamePause();
			// On first tick, Behaviour tree should call SetNewTarget.
		}
	}
	*/
}
