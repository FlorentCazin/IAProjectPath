// Fill out your copyright notice in the Description page of Project Settings.


#include "MPIAPlayerController.h"
#include "ButtonGameMode.h"
#include "LandscapeProxy.h"
#include "ButtonStartStopGameMode.h"


AMPIAPlayerController::AMPIAPlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}


void AMPIAPlayerController::OnPossess(APawn* aPawn){
	Super::OnPossess(aPawn);
	Player = Cast<AMPIAPlayerCharacter>(aPawn);
	gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());
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
	if(gamemode->OneWayMod){
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("%d"), targetsSpawned.Num()));
		if (targetsSpawned.Num() < 1) {
			targetsSpawned.Add(GetWorld()->SpawnActor<AActor>(Player->TargetToSpawn, hit.ImpactPoint, FRotator::ZeroRotator));
		}
	}
	else {
		targetsSpawned.Add(GetWorld()->SpawnActor<AActor>(Player->TargetToSpawn, hit.ImpactPoint, FRotator::ZeroRotator));
	}
}
