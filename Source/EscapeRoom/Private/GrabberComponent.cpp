// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabberComponent.h"
#include "Components/PrimitiveComponent.h"
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
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find PhysicsHandle for object: %s"), *GetOwner()->GetName());
	}
}

void UGrabberComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<RUNTIME_GENERATED UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberComponent ERROR: Can't find InputComponent for object: %s"), *GetOwner()->GetName());
		return;
	}
	else {
		// Bind Input Actions
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberComponent::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberComponent::Release);
	}
}

void UGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("GrabberComponent - Attempting to grab object "));

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle is NULL. Avoided NPE in UUGrabberComponent::Grab() on object:  %s"),
			*GetOwner()->GetName());
		return;
	}

	UpdatePlayerViewPoint();
	DrawVectorInWorld(PlayerViewPointLocation, LineTraceEnd);
	FHitResult LineTraceHit = CheckForObjectHit(PlayerViewPointLocation, LineTraceEnd);
	AActor* HitActor = LineTraceHit.GetActor();

	// See what we hit.
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrabberComponent Ray Hitting Object: %s"), *HitActor->GetName());
		// Pick Up object
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			LineTraceHit.GetComponent(),
			NAME_None,
			LineTraceHit.GetComponent()->GetOwner()->GetActorLocation(),
			PlayerViewPointRotation);
	}
}

void UGrabberComponent::UpdatePlayerViewPoint()
{
	if (PawnController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawncontroller is NULL. Avoided NPE in UGrabberComponent::UpdatePlayerViewPoint() on object:  %s"), 
			*GetOwner()->GetName());
		return;
	}

	PlayerViewPointLocation = FVector();
	PlayerViewPointRotation = FRotator();
	PawnController->GetPlayerViewPoint(MUTATE PlayerViewPointLocation, MUTATE PlayerViewPointRotation);
	LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * DebugVectorLength);
}

void UGrabberComponent::DrawVectorInWorld(FVector PlayerLocation, FVector LineTraceEnd)
{
	DrawDebugLine(GetWorld(), PlayerLocation, LineTraceEnd, FColor(255, 50, 0), false, 0.0f, 0.0f, 10.0f);
}

FHitResult UGrabberComponent::CheckForObjectHit(FVector PlayerLocation, FVector LineTraceEnd)
{
	//Ray-cast out to reach distance
	FHitResult HitResult = FHitResult();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		MUTATE HitResult,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	return HitResult;
}

void UGrabberComponent::Release()
{
	// Drop item
	UE_LOG(LogTemp, Error, TEXT("GrabberComponent - Releasing object"));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePlayerViewPoint();
	// Log player view point.
	UE_LOG(LogTemp, Display, TEXT("GrabberComponent Location: %s, Rotation: %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString());

	UpdateGrabbedObjectLocation();
}

void UGrabberComponent::UpdateGrabbedObjectLocation()
{
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle is NULL. Avoided NPE in UGrabberComponent::Release() on object:  %s"),
			*GetOwner()->GetName());
		return;
	}

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}