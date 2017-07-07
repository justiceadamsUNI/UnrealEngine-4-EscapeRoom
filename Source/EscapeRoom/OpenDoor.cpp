// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


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

	ActorThatCanOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// See if the trigger is hit. If So Open the Door.
	if (PressurePlate->IsOverlappingActor(ActorThatCanOpen))
	{
		OpenDoor();
	}

}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	FRotator Rotation = FRotator(0.0f, -65.0f, 0.0f);
	Owner->SetActorRotation(Rotation);
}


