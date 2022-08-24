// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckGun.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void UBTService_CheckGun::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//UGameplayStatics::GetActorOfClass(GetWorld(),)

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Player == nullptr) return;

	if (OwnerComp.GetAIOwner() == nullptr) {
		return;

		
	}

	if (OwnerComp.GetAIOwner()->LineOfSightTo(Player)) {

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());

	}
	else {
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}