// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FPSCharacter.h"
#include "InventoryComponent.h"
#include "FKFPSProjectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));	
	RootComponent = Mesh;

}

void AWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) {
	Super::PostEditChangeProperty(PropertyChangeEvent);
	GetDataFromTable();
}


void AWeapon::GetDataFromTable() {
	if (WeaponType == EWeaponType::None)return;

	if (ItemDataTable) {
		const FName RowName = FName(UEnum::GetDisplayValueAsText(WeaponType).ToString());
		FWeaponInformationTable* Row = ItemDataTable->FindRow<FWeaponInformationTable>(RowName, "Weapon Details");
		if (Row) {
			Mesh->SetSkeletalMesh(Row->Mesh);

			MaxAmmo = Row->MaxAmmo;
			UE_LOG(LogTemp, Warning, TEXT("MaxAmmo:%f"), MaxAmmo);


			AmmoPerReload = Row->AmmoPerReload;
			UE_LOG(LogTemp, Warning, TEXT("AmmoPerReload:%f"), AmmoPerReload);

			Muzzle = Row->Particle;

			DamageNum = Row->Damage;
			UE_LOG(LogTemp, Warning, TEXT("DamageNum:%f"), DamageNum);

			ProjectileClass = Row->ProjectileClass;

		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	GetDataFromTable();
	InRealAmmo = AmmoPerReload;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Shoot() {


	if(InRealAmmo>0 && !bIsReloading ){
		InRealAmmo--;

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

			FVector SocketPos = Mesh->GetSocketLocation("Projectile");
			const FVector SpawnLocation = SocketPos + SpawnRotation.RotateVector(FVector(0.0f, 0.0f, 0.0f));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			AActor* Projectile = World->SpawnActor<AFKFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Projectile->SetOwner(this);
		}
	}

	}
	else if (InRealAmmo <= 0 && MaxAmmo!=0)
	{
		Character->Reload();
	
	}
}
void AWeapon::Reload() {
	if (InRealAmmo < MaxAmmo)
	{
		MaxAmmo = InRealAmmo + MaxAmmo - AmmoPerReload;
		InRealAmmo = AmmoPerReload;
		UE_LOG(LogTemp, Warning, TEXT("Max Ammo:%f"), MaxAmmo);
	}
	else if (MaxAmmo != 0 && AmmoPerReload > MaxAmmo) {
		
		auto CaculateAmmo = AmmoPerReload - InRealAmmo;
		auto AmmoWillAdd = MaxAmmo - CaculateAmmo;
		if (AmmoWillAdd >= 0) {
			InRealAmmo += CaculateAmmo;
			MaxAmmo = AmmoWillAdd;
		}
		else {
			InRealAmmo += MaxAmmo;
			MaxAmmo = 0;
		}
	}
	if (MaxAmmo < 0) {
		MaxAmmo = 0;
	}

	Character->UpdateAmmoInfo();
}


void AWeapon::SetWeaponForPlayer(AFPSCharacter* thisCharacter, FName Text) {
	Character = thisCharacter;
	Character->Inventory->OccuppiedSlot.Add(this);
	Character->UpdateAmmoInfo();

	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	
	FVector NewRalative = FVector(0, 0, 0);
	FRotator NewRotator = FRotator(0, 0, 0);

	this->SetActorRelativeLocation(NewRalative);
	this->SetActorRelativeRotation(NewRotator);
	this->AttachToComponent(thisCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,Text);
	this->SetOwner(thisCharacter);
}
