// CopyRight Nick 2020
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindHandle();
	SetUpInputComp();


}

void UGrabber::FindHandle(){
		Handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Handle) {
		// found
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No physics handle found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComp(){

	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComp) {
		InputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grab(){
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotaion;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotaion
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotaion.Vector()*Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComToGrab = HitResult.GetComponent();

	UE_LOG(LogTemp, Warning, TEXT("Grabber press on %s!"), *GetOwner()->GetName());


	if (HitResult.GetActor())
	Handle->GrabComponentAtLocation(
		ComToGrab,
		NAME_None,
		LineTraceEnd
	);
	

}

void UGrabber::Release(){
	UE_LOG(LogTemp, Warning, TEXT("Grabber release on %s!"), *GetOwner()->GetName());
	Handle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotaion;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotaion
	);
	// ... For debug only
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotaion.Vector()*Reach;

	if (Handle->GrabbedComponent){

		Handle->SetTargetLocation(LineTraceEnd);
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const{


	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotaion;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotaion
	);
	// ... For debug only
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotaion.Vector()*Reach;
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0,255,0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );


	FHitResult Hit;

	FCollisionQueryParams TraceParms(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParms
	);

	AActor *ActorHit = Hit.GetActor();
	
	return Hit;
}