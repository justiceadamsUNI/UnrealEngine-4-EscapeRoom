// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameframework/Actor.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -65.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	float LastDoorOpenTime;

	AActor* ActorThatCanOpen;

	AActor* Owner;
};
