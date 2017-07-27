// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameframework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "GrabberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Grab();

	void Release();

	void DrawVectorInWorld(FVector PlayerLocation, FVector LineTraceEnd);

	FHitResult CheckForObjectHit(FVector PlayerLocation, FVector LineTraceEnd);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	APlayerController* PawnController;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	FVector PlayerViewPointLocation;

	FRotator PlayerViewPointRotation;

	FVector LineTraceEnd;

	UPROPERTY(EditAnywhere)
	float DebugVectorLength;

	void FindPhysicsHandle();

	void SetupInputComponent();

	void UpdatePlayerViewPoint();

	void UpdateGrabbedObjectLocation();
};
