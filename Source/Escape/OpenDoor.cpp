// CopyRight Nick 2020


#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#include "Components/AudioComponent.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set"), *GetOwner()->GetName());
	}	

	FindAudioComp();
}

void UOpenDoor::FindAudioComp() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent){
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio!"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	if (TotalMassOfActors() > MassToOpenDoors){

		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay){

		CloseDoor(DeltaTime);
	}

	// float CurrentYaw =  GetOwner()->GetActorRotation().Yaw;
	// FRotator OpenDoorRotation {0.f, (CurrentYaw + 90.f), 0.f};
	// OpenDoorRotation.Yaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + 90.f, DeltaTime, 2);
	// GetOwner()->SetActorRotation(OpenDoorRotation);
// 
}
void UOpenDoor::OpenDoor(float DeltaTime){
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Didnt find the audio"));
		return;
	}

	if (!OpenDoorSound) {
		AudioComponent->Play();
		UE_LOG(LogTemp, Warning, TEXT("Sound should be played here!"));
		OpenDoorSound = true;
	}

}

void UOpenDoor::CloseDoor(float DeltaTime){
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!AudioComponent) {
		return;
	}

	if (!CloseDoorSound) {
		AudioComponent->Play();
		CloseDoorSound = true;
	}

}

float UOpenDoor::TotalMassOfActors() const {
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor *Actor: OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}