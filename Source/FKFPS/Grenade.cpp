// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "FPSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"

AGrenade::AGrenade() {
}

void AGrenade::BeginPlay() {
	Super::BeginPlay();

	FTimerHandle Time;
	GetWorldTimerManager().SetTimer(Time, this, &AGrenade::Explode, 5.f, false);
	UE_LOG(LogTemp, Warning, TEXT("Boom"));
}

void AGrenade::Explode() {



	TArray<FHitResult> HitArray;
	FVector Start = this->GetActorLocation();
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(ExplodeRadius);
	bool bSweepHit = GetWorld()->SweepMultiByChannel(HitArray, Start, Start, FQuat::Identity, ECC_WorldStatic, SphereShape);
	if (bSweepHit) {
		for (const FHitResult HitResult : HitArray) {
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());
			if (MeshComp) {
				MeshComp->AddRadialImpulse(Start,ExplodeRadius, ExplosionSrength, ERadialImpulseFalloff::RIF_Constant, true);
			}
			APawn* pawn = Cast<APawn>(HitResult.GetActor());
			if (pawn) {
				TArray<AActor*> ignoredActors;
				UGameplayStatics::ApplyRadialDamage( // Apply Radial Damage
					GetWorld(),
					Damage,
					GetActorLocation(),
					ExplodeRadius,
					UDamageType::StaticClass(),
					ignoredActors,
					this,
					nullptr,
					false,
					ECollisionChannel::ECC_Visibility
					);
			}
		}
	}
	Destroy();
	
}