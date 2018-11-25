#include "TextRotationComponent.h"
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

	Target = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (!Target) {
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
	if (IsValid(TextField) && Target) {
		FVector Direction = Target->GetActorLocation() - TextField->GetOwner()->GetActorLocation();

		TextField->SetRelativeRotation(Direction.Rotation());
	}
}

void UTextRotationComponent::AddTextComponent(UTextRenderComponent * TextComponent)
{
	TextFields.AddUnique(TextComponent);
}

void UTextRotationComponent::SetTarget(AActor * NewTarget)
{
	Target = NewTarget;
}

