// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataLibrary.h"
#include "Engine/DataTable.h"
#include "Components/SphereComponent.h"

#include "GameFramework/Actor.h"

#include "Weapon.generated.h"


class AFPSCharacter;

class USkeletalMeshComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FKFPS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
		EWeaponType WeaponType = EWeaponType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")

		FGennericWeaponInfo WeaponInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
	UDataTable* ItemDataTable;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
	USkeletalMeshComponent* Mesh;
	//USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
	float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
		float AmmoPerReload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
		UParticleSystem* Muzzle;
	UPROPERTY(BlueprintReadWrite)
	float InRealAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFKFPSProjectile> ProjectileClass;
	float DamageNum;
	AFPSCharacter* Character;

	float FireRate;

	void SetWeaponForPlayer(AFPSCharacter* thisCharacter, FName Text);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		bool bIsReloading = false;

	void GetDataFromTable();

	virtual void Shoot();

	virtual void Reload();
};
