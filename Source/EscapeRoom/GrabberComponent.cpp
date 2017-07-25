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
}

// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnController = GetWorld()->GetFirstPlayerController();
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabberComponent::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find PhysicsHandle for object: "), *GetOwner()->GetName());
	}
}

void UGrabberComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<RUNTIME_GENERATED UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find InputComponent for object: "), *GetOwner()->GetName());
		//Bind Input action
	}
	else {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberComponent::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberComponent::Release);
	}
}

void UGrabberComponent::Grab()
{
	// Pick up item
	UE_LOG(LogTemp, Error, TEXT("GrabberComponent - WE GRABBING BOI "));

	//Get player view point.
	FVector PlayerViewPointLocation = FVector();
	FRotator PlayerViewPointRotation = FRotator();
	PawnController->GetPlayerViewPoint(MUTATE PlayerViewPointLocation, MUTATE PlayerViewPointRotation);

	//Draw Vector in world and check hit status.
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * DebugVectorLength);
	DrawVectorInWorld(PlayerViewPointLocation, LineTraceEnd);
	FHitResult LineTraceHit = CheckForObjectHit(PlayerViewPointLocation, LineTraceEnd);

	//See what we hit.
	if (LineTraceHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabberComponent Ray Hitting Object: %s"), *LineTraceHit.GetActor()->GetName())
	}
}

void UGrabberComponent::Release()
{
	// Drop item
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
	UE_LOG(LogTemp, Display, TEXT("GrabberComponent Location: %s, Rotation: %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString());
}

void UGrabberComponent::DrawVectorInWorld(FVector PlayerLocation, FVector LineTraceEnd)
{
	DrawDebugLine(GetWorld(), PlayerLocation, LineTraceEnd, FColor(255, 50, 0), false, 0.0f, 0.0f, 10.0f);
}

FHitResult UGrabberComponent::CheckForObjectHit(FVector PlayerLocation, FVector LineTraceEnd)
{
	//Ray-cast out to reach distance
	FHitResult LineTraceHit = FHitResult();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		MUTATE LineTraceHit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	return LineTraceHit;
}

