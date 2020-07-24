// CopyRight Nick 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach {100.f};

	UPhysicsHandleComponent * Handle {nullptr};
	UInputComponent *InputComp {nullptr};

	void Grab();
	void Release();
	void FindHandle();
	void SetUpInputComp();

	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayersReach() const;
	FVector GetPlayersWorldPos() const;
};
