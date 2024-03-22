// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MPIAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IAPROJECTPATH_API AMPIAGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AMPIAGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Started;

	//OneWay mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OneWayMod;

	//OneWay mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SeveralPointMod;

	//Circuit mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CircuitMod;

	//Graph array
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<tytpe> CircuitMod;

public:

	UFUNCTION(BlueprintCallable)
	void SetOneWayMod();

	UFUNCTION(BlueprintCallable)
	void SetSeveralMod();

	UFUNCTION(BlueprintCallable)
	void SetCircuitMod();

	
};
