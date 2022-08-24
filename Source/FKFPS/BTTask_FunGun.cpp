// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FunGun.h"
#include "AIController.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "NavigationSystem.h"
UBTTask_FunGun::UBTTask_FunGun() {
	NodeName = TEXT("Find Gun");
}

EBTNodeResult::Type UBTTask_FunGun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	AFPSCharacter* Character = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) return EBTNodeResult::Failed;



	//UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(),  100.f);


	return EBTNodeResult::Succeeded;
}