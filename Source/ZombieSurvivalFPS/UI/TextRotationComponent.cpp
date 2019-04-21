#include "UI/TextRotationComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTextRotationComponent::UTextRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTextRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* PlayerReference = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (IsValid(PlayerReference)) {
		SetTarget(PlayerReference);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Player not found to bind to TextRotationComponent as target"));
	}
}

// Called every frame
void UTextRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int ArrSize = TextFields.Num();

	if (ArrSize != 0 && Target) {
		for (int i = 0; i < ArrSize; i++) {
			SetRotationToTarget(TextFields[i]);
		}
	}
}

void UTextRotationComponent::SetRotationToTarget(UTextRenderComponent * TextField)
{
	if (IsValid(Target)) {
		if (IsValid(TextField)) {
			FVector Direction = Target->GetActorLocation() - TextField->GetOwner()->GetActorLocation();
			TextField->SetRelativeRotation(Direction.Rotation());
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("TextField of TextRotationComponent isn't valid."));
		}
	} 
	else {
		UE_LOG(LogTemp, Error, TEXT("Target isn't valid."));
	}
}

void UTextRotationComponent::AddTextComponent(UTextRenderComponent * TextComponent)
{
	TextFields.AddUnique(TextComponent);
}

void UTextRotationComponent::SetTarget(AActor * NewTarget)
{
	if (IsValid(NewTarget)) {
		Target = NewTarget;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Trying to set Target of TextRotationComponent to an invalid one."));
	}
}