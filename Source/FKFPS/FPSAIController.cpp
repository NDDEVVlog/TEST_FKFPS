// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AFPSAIController::BeginPlay() {
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (AIBehaviour != nullptr) {
		RunBehaviorTree(AIBehaviour);




		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), Player->GetActorLocation());
	}

}
void AFPSAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}