// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCharacter.h"
#include "TeamData.h"
#include "Engine/DataTable.h"

#include "TeamClass.generated.h"

/**
 * 
 */
UCLASS()
class FKFPS_API ATeamClass : public AFPSCharacter
{
	GENERATED_BODY()
	
public :

//	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		EDataTeam Team = EDataTeam::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		FGenericTeamInfo TeamInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		UDataTable* TeamDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		FName TeamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		UMaterial* Material;

};
