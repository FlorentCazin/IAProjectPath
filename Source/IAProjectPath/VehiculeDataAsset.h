// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Math/Vector.h"
#include "Chaos/Matrix.h"
#include "VehiculeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class IAPROJECTPATH_API UVehiculeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	float mass;
	FVector position;
	FVector velocity;
	FMatrix orientation;
	float max_force;
	float max_speed;
};
