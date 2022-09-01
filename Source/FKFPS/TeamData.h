// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "TeamData.generated.h"

/**
 * 
 */

UENUM()
enum class EDataTeam :uint8 {
	None UMETA(DisplayName = "None"),
	Terrorist UMETA(DisplayName = "Terrorist"),
	CounterTerrorist UMETA(DisplayName = "CounterTerrorist")
};

USTRUCT(BlueprintType)
struct FGenericTeamInfo {
	GENERATED_BODY()

	EDataTeam TeamType; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		FName TeamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		UMaterial* Material;
	

};
USTRUCT(BlueprintType)
struct FTeamInforTable: public FTableRowBase {
	GENERATED_BODY()

		EDataTeam TeamType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		FName TeamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Info")
		UMaterial* Material;


};
UCLASS()
class FKFPS_API UTeamData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
