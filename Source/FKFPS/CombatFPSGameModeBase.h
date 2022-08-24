// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatFPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FKFPS_API ACombatFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Time=300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Minutes=2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Seconds=0;
	
	void CountDown();
protected:
	void BeginPlay();
public:

	int32 RedMembers;
	int32 BlueMembers;

	void ActorDied(AActor* actorDied);

	int32 GetRedMemberNum();
	int32 GetBlueMemberNum();
};
