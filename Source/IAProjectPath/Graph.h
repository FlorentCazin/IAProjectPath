// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphNode.h"
#include "Graph.generated.h"

UCLASS()
class IAPROJECTPATH_API AGraph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraph();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AGraphNode*> NodesInGraph;

	//un noeud etudier passe dans la liste ouverte, contient tout les noeuds étudiés
	TArray<AGraphNode*> openList;

	//contient les noeuds qui sont concidérés comme faisant parti du chemin solution (doit passer par la liste ouverte avant de venir ici)
	TArray<AGraphNode*> closeList;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> AStar(AGraphNode *beginingGraphNode, AGraphNode *targetGraphNode);

};
