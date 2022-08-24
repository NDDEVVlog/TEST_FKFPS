// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DataTable.h"
#include "DataLibrary.generated.h"


/**
 * 
 */

UENUM()
enum class EWeaponType : uint8 {
	None UMETA(DisplayName = "None"),
	AK47 UMETA(DisplayName = "AK47"),
	M4A1 UMETA(DisplayName = "M4A1")
	
};

USTRUCT(BlueprintType)
struct FGennericWeaponInfo {
	GENERATED_BODY()


		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon Info")
	float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
	USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")

	UTexture2D* WeaponIamge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float AmmoPerReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		UParticleSystem* Particle;
};


USTRUCT(BlueprintType)
struct FWeaponInformationTable : public FTableRowBase {
	GENERATED_BODY()


		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")

		UTexture2D* WeaponIamge;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		float AmmoPerReload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Info")
		UParticleSystem* Particle;
};


UCLASS()
class FKFPS_API UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
