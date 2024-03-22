// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTargetToSpawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerTargetToSpawn::APlayerTargetToSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerTargetToSpawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> tmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphNode::StaticClass(), tmp);

	for (auto& n : tmp) {
		AGraphNode* node = Cast<AGraphNode>(n);
		if (!ClosestGraphNode) {
			ClosestGraphNode = node;
		}
		else {
			float oldDistance = (GetActorLocation() - ClosestGraphNode->GetActorLocation()).Size();
			float newDistance = (GetActorLocation() - node->GetActorLocation()).Size();
			if (oldDistance > newDistance) {
				ClosestGraphNode = node;
			}
		}
	}
	
}

// Called every frame
void APlayerTargetToSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

