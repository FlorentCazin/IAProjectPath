// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonGameMode.h"
#include "MPIAGameMode.h"
#include "MPIAPlayerController.h"


// Sets default values
AButtonGameMode::AButtonGameMode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AButtonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonGameMode::SetGameMode() {
	AMPIAPlayerController* controller = Cast<AMPIAPlayerController>(GetWorld()->GetFirstPlayerController());
	AMPIAGameMode *gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());

	//delete targets already spawned
	if (controller) {
		for (AActor* a : controller->targetsSpawned) {
			if (a) {
				controller->targetsSpawned.Remove(a);
				a->Destroy();
				controller->targetsSpawned.Shrink(); //reduce the array size
			}
		}
	}

	if (name.Equals("OnePoint")) {
		gamemode->SetOneWayMod();
		controller->ModeChoosen = 0;
	}
	else if (name.Equals("SeveralPoints")) {
		gamemode->SetSeveralMod();
		controller->ModeChoosen = 1;
	}
	else if (name.Equals("Circuit")) {
		gamemode->SetCircuitMod();
		controller->ModeChoosen = 2;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SetGameMode name does not exist"));
	}
}

