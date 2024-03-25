// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonStartStopGameMode.h"
#include "MPIAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Vehicule.h"
#include "Graph.h"
#include "GraphNode.h"
#include "PlayerTargetToSpawn.h"
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

	TArray<AActor*> tmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphNode::StaticClass(), tmp);

	for (auto& n : tmp) {
		AGraphNode* node = Cast<AGraphNode>(n);
		arrayGraphNodes.Add(node);
	}
}

// Called every frame
void AButtonStartStopGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonStartStopGameMode::OnClick() {
	AMPIAGameMode* gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());
	AMPIAPlayerController *controller = Cast<AMPIAPlayerController>(GetWorld()->GetFirstPlayerController());
	TArray<AActor*> vehicules;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicule::StaticClass(), vehicules);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("size taregtscontroller : %d"), controller->targetsSpawned.Num()));
	// pas oublier les checks de si start et ya rien etc pas nullptr si necessaire
	if (gamemode->Started) { //then stop
		if (controller) {
			for (AActor* a : controller->targetsSpawned) {
				if (a->IsValidLowLevel()) {
					//controller->targetsSpawned.Remove(a);
					a->Destroy();
					//controller->targetsSpawned.Shrink(); //reduce the array size
					controller->onewaymodalreadyspawned = false;
				}
			}
			controller->targetsSpawned.Reset();
		}
		gamemode->Started = false;

		for (auto &v : vehicules) {
			AVehicule *vehicule = Cast<AVehicule>(v);
			vehicule->circuitIndexToReach = 0;
			vehicule->reachedIsDestination = false;
			vehicule->ClosestGraphNode = nullptr;
			//reset array
			vehicule->newArrayTargets.Reset();
			//Shrink()?
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("size : %d"), vehicule->newArrayTargets.Num()));
		}
	}
	else { //then start
		for (auto& v : vehicules) {
			AVehicule* vehicule = Cast<AVehicule>(v);
			//if vehicule target.num > 0
			vehicule->GraphPointsArray();
		}
		gamemode->Started = true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, gamemode->Started?TEXT("Started"):TEXT("Not Started"));
}

//tableau 1 2 3 pour graph exemple: ou faire autrement (noeudsattachés...)
//	 1    1 1 0
//   2    1 1 1
//   3    0 1 1

