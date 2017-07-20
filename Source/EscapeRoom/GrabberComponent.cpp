// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabberComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define MUTATE
#define RUNTIME_GENERATED

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<RUNTIME_GENERATED UInputComponent>();
	PawnController = GetWorld()->GetFirstPlayerController();

	FString  ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Log, TEXT("GrabberComponent Online: Reporting for Object - %s"), *ObjectName);

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find PhysicsHandle for object: "), *ObjectName);
	}

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find InputComponent for object: "), *ObjectName);
		//Bind Input action
	} else {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberComponent::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberComponent::Release);
		//ToDo: Bind another action to drop!
	}
	
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("GrabberComponent - WE GRABBING BOI "));
}

void UGrabberComponent::Release()
{
	UE_LOG(LogTemp, Error, TEXT("GrabberComponent - WE RELEASING BOI "));
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Log player view point.
	FVector PlayerViewPointLocation = FVector();
	FRotator PlayerViewPointRotation = FRotator();

	PawnController->GetPlayerViewPoint(MUTATE PlayerViewPointLocation, MUTATE PlayerViewPointRotation);
	UE_LOG(LogTemp, Warning, TEXT("GrabberComponent Location: %s, Rotation: %s"), 
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString());

	//Draw Vector in world.
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * DebugVectorLength);
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 50, 0), false, 0.0f, 0.0f, 10.0f);
	
	//Ray-cast out to reach distance
	FHitResult LineTraceHit = FHitResult();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		MUTATE LineTraceHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	//See what we hit.
	if (LineTraceHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabberComponent Ray Hitting Object: %s"), *LineTraceHit.GetActor()->GetName())
	}
}

