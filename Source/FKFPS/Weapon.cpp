// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "FKFPSProjectile.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	//SphereRadius = 32.f;


	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetSphereRadius(90.0f);
	//CollisionSphere->bGenerateOverlapEvents = true;
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionSphere->bHiddenInGame = false;


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


		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	GetDataFromTable();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);

	InRealAmmo = AmmoPerReload;


}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	

	
	CollisionSphere->SetGenerateOverlapEvents(false);
	
	Character = Cast<AFPSCharacter>(OtherActor);
	if (Character) {
		if (Character->hasPrimaryWeapon== false)
		{
			Character->WeaponUsingNum = 0;
			Character->hasPrimaryWeapon = true;
			Character->OnUseWeapon = this;

			SetWeaponForPlayer(Character, TEXT("WeaponPlace"));

			
			
			return;
		}
		else if(Character->hasSecondaryWeapon == false && Character->hasPrimaryWeapon == true) {
			Character->hasSecondaryWeapon = true;

			
			SetWeaponForPlayer(Character, TEXT("Second Weapon"));

			return;

		}
	}

}
									

void AWeapon::Shoot() {


	if(InRealAmmo>0 && !isReloading ){
		InRealAmmo--;

		






	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

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
	if (InRealAmmo < MaxAmmo) {
		MaxAmmo = InRealAmmo + MaxAmmo - AmmoPerReload;

		InRealAmmo = AmmoPerReload;
		UE_LOG(LogTemp, Warning, TEXT("Max Ammo:%f"), MaxAmmo);

	}
	else if (MaxAmmo!= 0 && AmmoPerReload > MaxAmmo) {


		
		auto CaculateAmmo =AmmoPerReload- InRealAmmo;

		

		MaxAmmo =AmmoPerReload- FMath::Min(MaxAmmo, CaculateAmmo);
		InRealAmmo +=CaculateAmmo ;

		MaxAmmo = 0;
	//	


		UE_LOG(LogTemp, Warning, TEXT("In Real Ammo:%f"), InRealAmmo);

	}

	Character->UpdateAmmoInfo();
}


void AWeapon::SetWeaponForPlayer(AFPSCharacter* thisCharacter, FName Text) {
	thisCharacter->Inventory.PrimaryWeapon.Add(this);

	FVector NewRalative = FVector(0, 0, 0);
	this->SetActorRelativeLocation(NewRalative);
	this->AttachToComponent(thisCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,Text);
	CollisionSphere->SetHiddenInGame(true);
	thisCharacter->UpdateAmmoInfo();

	this->SetOwner(thisCharacter);
}
