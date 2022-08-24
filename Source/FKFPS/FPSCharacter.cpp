// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "CombatFPSGameModeBase.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	
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



	PlayerInputComponent->BindAction("Swap", IE_Pressed, this, &AFPSCharacter::SwapWeapon);

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
	if (OnUseWeapon == nullptr) return;

	GetWorldTimerManager().SetTimer(FireTimeHandle, this,&AFPSCharacter::FireShoot, 0.1f, true);

}
void AFPSCharacter::FireShoot() {
	OnUseWeapon->Shoot();
	UpdateAmmoInfo();

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


void AFPSCharacter::SwapWeapon() {
	if(Inventory.PrimaryWeapon.Num() == 2)
	if (WeaponUsingNum == 0) {
		Inventory.PrimaryWeapon[WeaponUsingNum]->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Second Weapon"));

		WeaponUsingNum = 1;
		Inventory.PrimaryWeapon[WeaponUsingNum]->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPlace"));
		OnUseWeapon = Inventory.PrimaryWeapon[WeaponUsingNum];
	}
	else {
		Inventory.PrimaryWeapon[WeaponUsingNum]->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Second Weapon"));

		WeaponUsingNum = 0;

		Inventory.PrimaryWeapon[WeaponUsingNum]->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponPlace"));
		OnUseWeapon = Inventory.PrimaryWeapon[WeaponUsingNum];
	}
	
	UpdateAmmoInfo();
}

//Take Damage
float AFPSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	float DamageAplly = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	auto DamageToApply = FMath::Min(Health, DamageAplly);
	Health -= DamageToApply;
	if (isDead()) {
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
	if (OnUseWeapon == nullptr) return;

	isReload = true; 
	OnUseWeapon->isReloading = true;
	
	FTimerHandle Reloadtime;
	GetWorldTimerManager().SetTimer(Reloadtime,this,&AFPSCharacter::NotReload, 3.3f,false);
	if(MaxAmmo != 0)
	OnUseWeapon->Reload();
	

}

//Stop the reload animation
void AFPSCharacter::NotReload() {
	isReload = false;
	OnUseWeapon->isReloading = false;
}


//Update Ammo Info
void AFPSCharacter::UpdateAmmoInfo() {
	MaxAmmo = OnUseWeapon->MaxAmmo;
	AmmoInReal = OnUseWeapon->InRealAmmo;


}