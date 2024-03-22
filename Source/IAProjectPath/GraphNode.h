// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphNode.generated.h"

UCLASS()
class IAPROJECTPATH_API AGraphNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphNode();

	//nodes linked to this node
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AGraphNode*> links;

	//Array in same order than links which represent the distance between this node and is links
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> distanceBetweenLinks;

	//le cout pour aller du point de départ au nœud considéré
	float GCost;

	//le cout pour aller du nœud considéré au point de destination (distancebetweenlinks) à vol d'oiseau
	float HCost;

	//somme des précédents, mais mémorisé pour ne pas le recalculer à chaque fois (peut etre pas ici)
	float FCost = 0.f;

	bool AlreadyVisited = false;

	//Previous Node
	AGraphNode* predeceseur;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
