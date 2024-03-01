// Fill out your copyright notice in the Description page of Project Settings.


#include "MPIAPlayerController.h"
#include "LandscapeProxy.h"


AMPIAPlayerController::AMPIAPlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	OneWayMod = true;
	SeveralPointMod = false;
	CircuitMod = false;
	Player = Cast<AMPIAPlayerCharacter>(GetPawn());
}




FHitResult AMPIAPlayerController::OnClickGetSingleLineTraceByChannel() {
	UWorld* World = GetWorld(); //world reference
	FHitResult Hit;
	if (World) {
		TArray<AActor*> ignoreActors;
		FVector startLocation = GetPawn()->GetActorLocation();
		FVector2D mousePosition;
		GetMousePosition(mousePosition.X, mousePosition.Y);
		FVector WorldPosition, WorldDirection;
		DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, WorldPosition, WorldDirection);
		FVector endLocation = WorldPosition + (WorldDirection * 15000);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActors(ignoreActors);
		World->LineTraceSingleByChannel(
			Hit,
			startLocation,
			endLocation,
			ECC_Visibility,
			CollisionParams
		);
		return Hit;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("GetWorld problem!"));
		return Hit;
	}
}

void AMPIAPlayerController::SetOneWayMod() {
	OneWayMod = true;
	SeveralPointMod = false;
	CircuitMod = false;
}


void AMPIAPlayerController::SetSeveralMod() {
	OneWayMod = false;
	SeveralPointMod = true;
	CircuitMod = false;
}


void AMPIAPlayerController::SetCircuitMod() {
	OneWayMod = false;
	SeveralPointMod = false;
	CircuitMod = true;
}

void AMPIAPlayerController::SpawningItemToReach(FHitResult hit) {
	if (hit.GetActor()->IsA(ALandscapeProxy)) {
		GetWorld()->SpawnActor<AActor>(Player->TargetToSpawn, hit.ImpactPoint);
	}
}