// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPIAPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IAPROJECTPATH_API AMPIAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMPIAPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> LocationArray;
	
public:
	UFUNCTION(BlueprintCallable)
	FHitResult OnClickGetSingleLineTraceByChannel();
};


