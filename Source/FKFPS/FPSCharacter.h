// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"
class AWeapon;



class ACombatFPSGameModeBase;


UCLASS()
class FKFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRateGamepad = 1;

	FTimerHandle FireTimeHandle;


	struct FInventory {


		TArray	<AWeapon*> PrimaryWeapon;


	};

	FInventory Inventory;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")
		AWeapon* OnUseWeapon;

public:	






	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Fires a projectile. */
	void OnPrimaryAction();

	void ReleasePrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void FireShoot();

	void SwapWeapon();

	void Reload();

	void UpdateAmmoInfo();

	void NotReload();

	ACombatFPSGameModeBase* FPSGameMode;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


	
	




public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")

	float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")

	float AmmoInReal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")
		bool isReload = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")

	float Health=100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")
		int32 Kills = 0;



	UFUNCTION(BlueprintPure)
		bool isDead() const { return Health <= 0; };
	
	int32 WeaponUsingNum;



	bool hasPrimaryWeapon = false;
	bool hasSecondaryWeapon = false;
};
