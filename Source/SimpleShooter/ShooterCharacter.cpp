// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Health.h"
#include "Weapon.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealth>(TEXT("Health"));
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);	//hide existing bone with weapon (mesh specific)
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponBlueprint);	//spawns actor into the world
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket")); //attaches weapon to socket on the mesh
	Weapon->SetOwner(this); //set owner of the weapon

	CharacterController = GetController();
	HealthComp = Cast<UHealth>(GetComponentByClass(UHealth::StaticClass()));
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

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::Turn);
	//controller bindings
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AShooterCharacter::TurnRate);
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
	SetActorEnableCollision(false);
	DetachFromControllerPendingDestroy();
}

bool AShooterCharacter::IsDead() const
{
	if (HealthComp) return HealthComp->IsDead();
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

	bool MunitionWasEmpty = !Weapon->PullTrigger(Location, Rotation.Vector());
	if (MunitionWasEmpty) Weapon->Reload();
}

