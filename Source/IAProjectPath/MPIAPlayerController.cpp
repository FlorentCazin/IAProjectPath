// Fill out your copyright notice in the Description page of Project Settings.


#include "MPIAPlayerController.h"
#include "ButtonGameMode.h"
#include "LandscapeProxy.h"
#include "ButtonStartStopGameMode.h"
#include "MPIAGameMode.h"


AMPIAPlayerController::AMPIAPlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}


void AMPIAPlayerController::OnPossess(APawn* aPawn){
	Super::OnPossess(aPawn);
	Player = Cast<AMPIAPlayerCharacter>(aPawn);
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

void AMPIAPlayerController::ClickAction(FHitResult hit) {
	AMPIAGameMode* gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());
	if (hit.IsValidBlockingHit() && hit.GetActor()->IsA<ALandscapeProxy>() && !gamemode->Started) { //spawn item
		SpawningItemToReach(hit);
	}
	else if (hit.IsValidBlockingHit() && hit.GetActor()->IsA(AButtonGameMode::StaticClass()) && !gamemode->Started) { //switch Gamemode
		AButtonGameMode* button = Cast<AButtonGameMode>(hit.GetActor());
		button->SetGameMode();
	}
	else if (hit.IsValidBlockingHit() && hit.GetActor()->IsA(AButtonStartStopGameMode::StaticClass())) { //start / stop
		AButtonStartStopGameMode* button = Cast<AButtonStartStopGameMode>(hit.GetActor());
		button->OnClick();
	}
}



void AMPIAPlayerController::SpawningItemToReach(FHitResult hit) {
	targetsSpawned.Add(GetWorld()->SpawnActor<AActor>(Player->TargetToSpawn, hit.ImpactPoint, FRotator::ZeroRotator));
}
