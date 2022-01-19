// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "SimpleShooterGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Health.h"
#include "Weapon.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealth>(TEXT("Health"));
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();

	CharacterController = GetController();
	HealthComponent = Cast<UHealth>(GetComponentByClass(UHealth::StaticClass()));
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHoldingWeaponTrigger) FireWeapon();
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::StartFireWeapon);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AShooterCharacter::StopFireWeapon);
	PlayerInputComponent->BindAxis(TEXT("SwitchWeapon"), this, &AShooterCharacter::SwitchWeapon);
	PlayerInputComponent->BindAction<FActionDelegate>(TEXT("SwitchToPrimaryWeapon"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponTo, 0);
	PlayerInputComponent->BindAction<FActionDelegate>(TEXT("SwitchToSecondaryWeapon"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponTo, 1);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::Turn);

	//controller bindings
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AShooterCharacter::TurnRate);
}

void AShooterCharacter::SwitchWeapon(float Value)
{
	if (Value == 0) return;

	Weapons[ActiveWeaponIndex]->SetInactive();

	if (Value > 0)
	{
		++ActiveWeaponIndex;
		if (ActiveWeaponIndex > Weapons.Num() - 1) ActiveWeaponIndex = 0;

	}
	else if (Value < 0)
	{
		--ActiveWeaponIndex;
		if (ActiveWeaponIndex < 0) ActiveWeaponIndex = Weapons.Num() - 1;
	}

	Weapons[ActiveWeaponIndex]->SetAsActiveWeapon();
}

void AShooterCharacter::SwitchWeaponTo(int32 Index)
{
	if (Index < 0 || Index > Weapons.Num() - 1) return;
	if (Index == ActiveWeaponIndex) return;

	Weapons[ActiveWeaponIndex]->SetInactive();
	ActiveWeaponIndex = Index;
	Weapons[ActiveWeaponIndex]->SetAsActiveWeapon();
}

void AShooterCharacter::MoveForward(float Value)
{
	float Speed = Value * ForwardMovementSpeed;
	AddMovementInput(GetActorForwardVector() * Speed);
}

void AShooterCharacter::MoveSideways(float Value)
{
	float Speed = Value * ForwardMovementSpeed;
	AddMovementInput(GetActorRightVector() * Speed);
}

void AShooterCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookAroundSpeedMouse);
}

void AShooterCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * LookAroundSpeedMouse);
}

void AShooterCharacter::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * LookAroundSpeedController * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void AShooterCharacter::TurnRate(float Value)
{
	AddControllerYawInput(Value * LookAroundSpeedController * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void AShooterCharacter::HandleDeath()
{
	ASimpleShooterGameModeBase* CurrentGameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	if (CurrentGameMode != nullptr)
	{
		CurrentGameMode->ActorDied(this);
	}

	SetActorEnableCollision(false);
	DetachFromControllerPendingDestroy();
}

bool AShooterCharacter::IsDead() const
{
	if (HealthComponent) return HealthComponent->IsDead();
	else return true;
}

void AShooterCharacter::StartFireWeapon()
{
	bHoldingWeaponTrigger = true;
}

void AShooterCharacter::StopFireWeapon()
{
	bHoldingWeaponTrigger = false;
}

void AShooterCharacter::FireWeapon()
{
	if (!CharacterController) return;

	FVector Location;
	FRotator Rotation;
	CharacterController->GetPlayerViewPoint(Location, Rotation);

	bool MunitionWasEmpty = !Weapons[ActiveWeaponIndex]->PullTrigger(Location, Rotation.Vector());
	if (MunitionWasEmpty) Weapons[ActiveWeaponIndex]->Reload();
}

void AShooterCharacter::SpawnWeapons()
{
	ActiveWeaponIndex = 0;
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);	//hide existing bone with weapon (mesh specific)

	for (int32 i = 0; i < WeaponBlueprints.Num(); i++)
	{
		Weapons.Add(GetWorld()->SpawnActor<AWeapon>(WeaponBlueprints[i])); //spawns actor into the world
		Weapons[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket")); //attaches weapon to socket on the mesh
 		Weapons[i]->SetOwner(this); //set owner of the weapon
 
 		Weapons[i]->SetInactive();
	}

	Weapons[ActiveWeaponIndex]->SetAsActiveWeapon();
	
}

