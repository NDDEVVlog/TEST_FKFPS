// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class FKFPS_API AFPSAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	APawn* Player;

private:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehaviour;
};
