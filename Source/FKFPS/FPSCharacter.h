// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FPSCharacter.generated.h"



class AWeapon;
class UInventoryComponent;
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

	void SwapWeapon(int32 NumberofSlot);

	void Reload();

	void UpdateAmmoInfo();

	void NotReload();

	void Pickup();

	void Unequip();
	
	void Num1();
	void Num2();
	void Num3();
	void Num4();
	void Num5();


	ACombatFPSGameModeBase* FPSGameMode;
	UInventoryComponent* Inventory;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")

	float MaxAmmo=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")

	float AmmoInReal=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")
		bool isReload = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")

	float Health=100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status Infor")
		int32 Kills = 0;



	UFUNCTION(BlueprintPure)
		bool bIsDead() const { return Health <= 0; };
	
	int32 WeaponUsingNum;

};
