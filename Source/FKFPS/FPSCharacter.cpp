// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "CombatFPSGameModeBase.h"
#include "Containers/Array.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Kills = 0;
	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FPSGameMode = Cast<ACombatFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
}
	
// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Bind Input Events
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AFPSCharacter::Pickup);
	PlayerInputComponent->BindAction("Unequip", IE_Pressed, this, &AFPSCharacter::Unequip);
	PlayerInputComponent->BindAction("1", IE_Pressed, this, &AFPSCharacter::Num1);
	PlayerInputComponent->BindAction("2", IE_Pressed, this, &AFPSCharacter::Num2);
	PlayerInputComponent->BindAction("3", IE_Pressed, this, &AFPSCharacter::Num3);
    PlayerInputComponent->BindAction("4", IE_Pressed, this, &AFPSCharacter::Num4);
	PlayerInputComponent->BindAction("5", IE_Pressed, this, &AFPSCharacter::Num5);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &AFPSCharacter::ReleasePrimaryAction);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::Reload);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}
void AFPSCharacter::OnPrimaryAction()
{	
	GetWorldTimerManager().SetTimer(FireTimeHandle, this,&AFPSCharacter::FireShoot, 0.1f, true);
}
void AFPSCharacter::FireShoot() {
	if (Inventory->OnUseWeapon == nullptr) return;
	
	Inventory->OnUseWeapon->Shoot();
	UpdateAmmoInfo();
}

void AFPSCharacter::Num1() {
	SwapWeapon(1);
}
void AFPSCharacter::Num2() {
	SwapWeapon(2);

}
void AFPSCharacter::Num3() {
	SwapWeapon(3);

}
void AFPSCharacter::Num4() {
	SwapWeapon(4);

}
void AFPSCharacter::Num5() {
	SwapWeapon(5);

}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
	
}
void AFPSCharacter::Unequip() {
	Inventory->Unequip(this);
}

void AFPSCharacter::SwapWeapon(int32 NumberofSlot) {
	if ((NumberofSlot-1) >= Inventory->OccuppiedSlot.Num()) return;


	if (Inventory->OnUseWeapon != nullptr) {
		if (Inventory->OccuppiedSlot[NumberofSlot - 1] == nullptr) return;
		int32 index = 0;

		for (int32 i = 0; i < Inventory->OccuppiedSlot.Num(); i++) {
			if (Inventory->OnUseWeapon == Inventory->OccuppiedSlot[i]) {
				index = i;
				UE_LOG(LogTemp, Warning, TEXT("Bruh"));

				break;
			}
		}
		// how tf did i do it
		Inventory->OnUseWeapon->Mesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Inventory->SlotName[index + 1]);
		Inventory->OnUseWeapon = Inventory->OccuppiedSlot[NumberofSlot - 1];
		Inventory->OnUseWeapon->Mesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Inventory->SlotName[0]);
		UpdateAmmoInfo();
	}
	else {
		Inventory->OnUseWeapon = Inventory->OccuppiedSlot[NumberofSlot - 1];
		Inventory->OnUseWeapon->Mesh->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Inventory->SlotName[0]);
	}
}

void AFPSCharacter::Pickup() {
	Inventory->Pickup(this);

}

//Take Damage
float AFPSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	float DamageAplly = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	auto DamageToApply = FMath::Min(Health, DamageAplly);
	Health -= DamageToApply;
	if (bIsDead()) {
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FPSGameMode->ActorDied(this);
		AFPSCharacter* Causer = Cast<AFPSCharacter>((DamageCauser->GetOwner())->GetOwner());
		if (Causer == nullptr) {
			return Health;
		}
		else {
			Causer->Kills++;
		}
	}
	return Health;
}


void AFPSCharacter::ReleasePrimaryAction()
{
	GetWorldTimerManager().ClearTimer(FireTimeHandle);
}


void AFPSCharacter::Reload() {
	if (Inventory->OnUseWeapon == nullptr) return;
	isReload = true; 
	Inventory->OnUseWeapon->bIsReloading = true;

	FTimerHandle Reloadtime;
	GetWorldTimerManager().SetTimer(Reloadtime,this,&AFPSCharacter::NotReload, 3.3f,false);
}

//Stop the reload animation
void AFPSCharacter::NotReload() {
	isReload = false;
	if (Inventory->OnUseWeapon == nullptr) return;
	Inventory->OnUseWeapon->bIsReloading = false;

	if (MaxAmmo != 0)
		Inventory->OnUseWeapon->Reload();

}


//Update Ammo Info
void AFPSCharacter::UpdateAmmoInfo() {
	
	MaxAmmo = Inventory->OnUseWeapon->MaxAmmo;
	AmmoInReal = Inventory->OnUseWeapon->InRealAmmo;
}