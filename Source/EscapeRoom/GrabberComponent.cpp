// Fill out your copyright notice in the Description page of Project Settings.

#include "GrabberComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define MUTATE

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

	PawnController = GetWorld()->GetFirstPlayerController();
	FString  ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Log, TEXT("GrabberComponent Online: Reporting for Object - %s"), *ObjectName);
	
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

	//See what we hit.
}

