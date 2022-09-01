// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FKFPSProjectile.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class FKFPS_API AGrenade : public AFKFPSProjectile
{
	GENERATED_BODY()

public:
	AGrenade();

	virtual void BeginPlay() override;

	void Explode();

	UPROPERTY(EditAnyWhere, Category = "Combat Infor")
		float ExplodeRadius = 750.f;
	UPROPERTY(EditAnyWhere, Category = "Combat Infor")
		float ExplosionSrength = 750.f;
	UPROPERTY(EditAnyWhere, Category = "Combat Infor")
		float Damage=100.f;
	
};
