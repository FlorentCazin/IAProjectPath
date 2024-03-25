// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"
#include "Kismet/GameplayStatics.h"
#include "GraphNode.h"


// Sets default values
AGraph::AGraph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGraph::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> tmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphNode::StaticClass(), tmp);
	for (auto& n : tmp) {
		NodesInGraph.Add(Cast<AGraphNode>(n));
	}
}

// Called every frame
void AGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGraph::ResetValueNodes() {
	for (AGraphNode* node : NodesInGraph) {
		node->AlreadyVisited = false;
		node->predeceseur = nullptr;
		node->FCost = 0.f;
	}
}

//return tout les noeuds entre begining et target compris
TArray<AActor*> AGraph::AStar(AGraphNode *beginingGraphNode, AGraphNode *targetGraphNode) {



	TArray<AActor*> resultNodes;

	//if begining node is not null
	if (beginingGraphNode && targetGraphNode) {
		openList.Add(beginingGraphNode);
	}
	else {
		ResetValueNodes();
		return resultNodes;
	}

	//Actual node
	AGraphNode* ActualNode = openList[0];

	while(ActualNode){ // && ActualNode != targetGraphNode OU mettre if(ActualNode != targetGraphNode) alors

		if (ActualNode == targetGraphNode) {
			openList.Remove(ActualNode);
			if (openList.Num() > 0) {
				ActualNode = openList[0];
			}
			else {
				ActualNode = nullptr;
			}
		}
		else {
			//Neighboors exploration
			for (int i = 0; i < ActualNode->links.Num(); i++) {
				//actual cost + cost between acutalnode and neighboor
				//check if already visited or lowest cost
				float ActualCostNeigboor = ActualNode->FCost + ActualNode->distanceBetweenLinks[i];
				if (!ActualNode->links[i]->AlreadyVisited || (ActualCostNeigboor < ActualNode->links[i]->FCost)) { //never visited or lowest
					ActualNode->links[i]->FCost = ActualCostNeigboor;
					ActualNode->links[i]->predeceseur = ActualNode;
					ActualNode->links[i]->AlreadyVisited = true;
					openList.Add(ActualNode->links[i]);
				}
			}

			//ActualNode->AlreadyVisited = true;
			openList.Remove(ActualNode);
			if (openList.Num() > 0) {
				ActualNode = openList[0];
			}
			else {
				ActualNode = nullptr;
			}
		}
	}

	//Recupere tout les predecesseur jusqu'au nullptr (begining donc) pour recuperer le chemin le plus cour
	ActualNode = targetGraphNode;
	resultNodes.Insert(targetGraphNode,0);

	while (ActualNode != beginingGraphNode) { //tant que ya un predeceseur
		resultNodes.Insert(ActualNode->predeceseur, 0);
		ActualNode = ActualNode->predeceseur;
	}
	resultNodes.Insert(beginingGraphNode, 0);

	ResetValueNodes();
	return resultNodes;

}


