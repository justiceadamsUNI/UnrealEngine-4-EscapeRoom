// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReporter.h"
#include "UnrealString.h"
#include "Array.h"

FString GetObjectPositionAsString(float X, float Y, float Z);

// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();
	FString  ObjectName = GetOwner()->GetName();
	float X = GetOwner()->GetTransform().GetTranslation().X;
	float Y = GetOwner()->GetTransform().GetTranslation().Y;
	float Z = GetOwner()->GetTransform().GetTranslation().Z;

	UE_LOG(LogTemp, Log, TEXT("Position Reporter: BeginPlay() - Object: %s is at %s"), *ObjectName, *GetObjectPositionAsString(X, Y, Z));
}


// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString GetObjectPositionAsString(float X, float Y, float Z ) {
	FStringFormatArg positionInit[] = {FStringFormatArg(X), FStringFormatArg(Y), FStringFormatArg(Z)};

	TArray<FStringFormatArg> Positions;
	Positions.Append(positionInit, ARRAY_COUNT(positionInit));
	return FString::Format(*FString("X={0}, Y={1}, Z={2}"), Positions);
}

