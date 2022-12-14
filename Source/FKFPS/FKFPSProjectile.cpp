// Copyright Epic Games, Inc. All Rights Reserved.

#include "FKFPSProjectile.h"
#include "FPSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"

AFKFPSProjectile::AFKFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetupAttachment(Mesh);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFKFPSProjectile::OnSphereBeginOverlap);		// set up a notification for when this component hits something blocking
	CollisionComp->bHiddenInGame = false;
	// Players can't walk on it
	CollisionComp->CanCharacterStepUpOn = ECB_Yes;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->InitialSpeed = 18000.f;
	ProjectileMovement->MaxSpeed = 18000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 300.0f;
}

void AFKFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void AFKFPSProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	AWeapon* ProjectileOwner = Cast<AWeapon>(GetOwner());
	if (ProjectileOwner == nullptr) return;

	APawn* Character = Cast<APawn>(ProjectileOwner->GetOwner());
	if (Character == nullptr) return;
	
	AController* OwnerController = Character->GetController();
	if (OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	auto Damage = ProjectileOwner->DamageNum;

	FPointDamageEvent DamageEvent(Damage, SweepResult, -Rotation.Vector(), nullptr);
	OtherActor->TakeDamage(Damage, DamageEvent, OwnerController,this);

}