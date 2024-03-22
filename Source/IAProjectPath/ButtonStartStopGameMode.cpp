// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonStartStopGameMode.h"
#include "MPIAPlayerController.h"
#include "MPIAGameMode.h"

// Sets default values
AButtonStartStopGameMode::AButtonStartStopGameMode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AButtonStartStopGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AButtonStartStopGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonStartStopGameMode::OnClick() {
	AMPIAGameMode* gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());
	AMPIAPlayerController *controller = Cast<AMPIAPlayerController>(GetWorld()->GetFirstPlayerController());
	// pas oublier les checks de si start et ya rien etc pas nullptr si necessaire
	if (gamemode->Started) { //then stop
		if (controller) {
			for (AActor* a : controller->targetsSpawned) {
				if (a) {
					controller->targetsSpawned.Remove(a);
					a->Destroy();
					controller->targetsSpawned.Shrink(); //reduce the array size
				}
			}
		}
		gamemode->Started = false;
	}
	else { //then start
		gamemode->Started = true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, gamemode->Started?TEXT("Started"):TEXT("Not Started"));
}

//tableau 1 2 3 pour graph exemple: ou faire autrement (noeudsattachés...)
//	 1    1 1 0
//   2    1 1 1
//   3    0 1 1

