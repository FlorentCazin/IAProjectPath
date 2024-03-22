// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNode.h"

// Sets default values
AGraphNode::AGraphNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGraphNode::BeginPlay()
{
	Super::BeginPlay();
	for (auto& node : links) {
		//not optimized ²..
		distanceBetweenLinks.Add(GetActorLocation().Size() - node->GetActorLocation().Size());
		DrawDebugLine(GetWorld(), GetActorLocation(), node->GetActorLocation(), FColor::White, true);
	}
	
}

// Called every frame
void AGraphNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

