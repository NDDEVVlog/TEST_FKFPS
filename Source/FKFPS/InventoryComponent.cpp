// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "FPSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::Pickup(AFPSCharacter * Character) {
	
	APawn* OwnerPawn = Cast<APawn>(Character);
	if (OwnerPawn == nullptr) return;
	AController* OwnerController= OwnerPawn->GetController();
	if (OwnerController == nullptr)return;



	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FHitResult Hit;

	FCollisionQueryParams Params;
	
	Params.AddIgnoredActor(GetOwner());
	FVector EndPoint = Location + Rotation.Vector() * 200;
	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
	if (isHit) {

		DrawDebugLine(GetWorld(), Location, EndPoint, FColor::Red, false, 5.f);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr) {
			AWeapon* Weapon = Cast<AWeapon>(HitActor);
			if (Weapon != nullptr) {
				
				if(OccuppiedSlot.Num()+1 < SlotName.Num())

				if (OnUseWeapon == nullptr && OccuppiedSlot.Num() == 0 ) {
					OnUseWeapon = Weapon;
					WeaponWorldScale = Weapon->GetActorScale();

					Weapon->SetWeaponForPlayer(Character,SlotName[OccuppiedSlot.Num()]);
					Character->WeaponUsingNum = 0;
				}
				else {
					WeaponWorldScale = Weapon->GetActorScale();
					Weapon->SetWeaponForPlayer(Character, SlotName[OccuppiedSlot.Num()+1]);
					UE_LOG(LogTemp, Warning, TEXT("Num : %f"), OccuppiedSlot.Num());
				}
			}
		}

	}
}

void UInventoryComponent::Unequip(AFPSCharacter* Character) {
	if (OnUseWeapon == nullptr) return;

	OccuppiedSlot.Remove(OnUseWeapon);
	OnUseWeapon->SetOwner(nullptr);
	OnUseWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnUseWeapon->SetActorScale3D(WeaponWorldScale);
	OnUseWeapon->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OnUseWeapon->Mesh->SetCollisionObjectType(ECC_PhysicsBody);
	OnUseWeapon->Mesh->SetSimulatePhysics(true);
	OnUseWeapon->Mesh->AddForce(Character->GetActorForwardVector() * 100.f);
	OnUseWeapon = nullptr;
}

