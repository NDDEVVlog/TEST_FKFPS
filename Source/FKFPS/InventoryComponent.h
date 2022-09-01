// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryComponent.generated.h"

class AFPSCharacter;
class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FKFPS_API UInventoryComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	int32 Slot; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatStatus")

	TArray<AWeapon*> OccuppiedSlot;

	AWeapon* OnUseWeapon;

	FVector WeaponWorldScale;

	TArray<FName> SlotName = {"WeaponPlace","FirstWeapon","Second Weapon","ThirdWeapon"};

	void Unequip(AFPSCharacter* Character);

	void Pickup(AFPSCharacter* Character);
};


