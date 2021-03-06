// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "SimpleShooterGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Health.h"
#include "Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "CharacterMotionComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealth>(TEXT("Health"));
	MotionComponent = CreateDefaultSubobject<UCharacterMotionComponent>(TEXT("Motion"));
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	CharacterController = GetController();

	CameraArmComp = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
	if (CameraArmComp) BaseAimLevel = CameraArmComp->TargetArmLength;
}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHoldingWeaponTrigger && !bIsReloadingWeapon) FireWeapon();
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
	if (MotionComponent) MotionComponent->SetupMotionInputBindings(this, PlayerInputComponent);

	PlayerInputComponent->BindAction<FActionDelegateBool>(TEXT("ZoomView"), IE_Pressed, this, &AShooterCharacter::SetAimCamera, true);
	PlayerInputComponent->BindAction<FActionDelegateBool>(TEXT("ZoomView"), IE_Released, this, &AShooterCharacter::SetAimCamera, false);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::StartFireWeapon);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AShooterCharacter::StopFireWeapon);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::Reloading);
	PlayerInputComponent->BindAxis(TEXT("SwitchWeapon"), this, &AShooterCharacter::SwitchWeapon);
	PlayerInputComponent->BindAction<FActionDelegateInt>(TEXT("SwitchToPrimaryWeapon"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponTo, 0);
	PlayerInputComponent->BindAction<FActionDelegateInt>(TEXT("SwitchToSecondaryWeapon"), IE_Pressed, this, &AShooterCharacter::SwitchWeaponTo, 1);
}

void AShooterCharacter::SetAimCamera(bool bZoomActive)
{
	if (!CameraArmComp) return;

	float CurrentDistance = CameraArmComp->TargetArmLength;
	if (bZoomActive) CameraArmComp->TargetArmLength = FMath::Max(CurrentDistance - ZoomDistance, 50.f);
	else CameraArmComp->TargetArmLength = BaseAimLevel;
}

void AShooterCharacter::SwitchWeapon(float Value)
{
	if (Value == 0) return;

	int32 NextWeaponIndex = ActiveWeaponIndex;
	if (Value > 0)
	{
		++NextWeaponIndex;
		if (NextWeaponIndex > Weapons.Num() - 1) NextWeaponIndex = 0;

	}
	else if (Value < 0)
	{
		--NextWeaponIndex;
		if (NextWeaponIndex < 0) NextWeaponIndex = Weapons.Num() - 1;
	}

	SwitchWeaponTo(NextWeaponIndex);
}

void AShooterCharacter::SwitchWeaponTo(int32 Index)
{
	if (Index < 0 || Index > Weapons.Num() - 1) return;
	if (Index == ActiveWeaponIndex) return;

	Weapons[ActiveWeaponIndex]->SetInactive();
	ActiveWeaponIndex = Index;
	Weapons[ActiveWeaponIndex]->SetAsActiveWeapon();

	UpdateAmmunitionUI();
}

void AShooterCharacter::Reloading()
{
	if (bIsReloadingWeapon) return;

	FTimerHandle ReloadingTimer = FTimerHandle();
	GetWorldTimerManager().SetTimer(
		ReloadingTimer,
		this, 
		&AShooterCharacter::FinishedReloading,
		Weapons[ActiveWeaponIndex]->GetReloadTime()
	);
	
	bIsReloadingWeapon = true;
}

void AShooterCharacter::FinishedReloading()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset Reloading"));
	bIsReloadingWeapon = false;

	Weapons[ActiveWeaponIndex]->Reload();
	UpdateAmmunitionUI();
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

	bool bShotMiss = FMath::RandRange(0.f, 1.f) < ShotMissProbability;
	bool MunitionWasEmpty = !Weapons[ActiveWeaponIndex]->PullTrigger(Location, Rotation.Vector(), bShotMiss);
	if (MunitionWasEmpty) Reloading();

	UpdateAmmunitionUI();
}

void AShooterCharacter::UpdateAmmunitionUI()
{
	int32 CurrentAmmo = Weapons[ActiveWeaponIndex]->GetCurrentAmmo();
	int32 MagSize = Weapons[ActiveWeaponIndex]->GetTotalAmmo();
	OnMunitionChanged.Broadcast(CurrentAmmo, MagSize);
}

void AShooterCharacter::SpawnWeapons()
{
	UE_LOG(LogTemp, Warning, TEXT("Initialize Weapons %s"), *GetName());
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

	UpdateAmmunitionUI();
}

