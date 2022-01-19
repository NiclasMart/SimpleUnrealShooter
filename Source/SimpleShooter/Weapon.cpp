// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
		
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	MeshComp->SetupAttachment(Root);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmunition = MagazinSize;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon::PullTrigger(FVector AimLocation,  FVector AimDirection, bool bIsMiss)
{
	if (CurrentAmmunition <= 0) return false;
	if (LastShotFired + 1.f / AutoFrequency > GetWorld()->GetTimeSeconds()) return true;
	
	PlayFireEffects();
	--CurrentAmmunition;

	if (bIsMiss) return true;
	
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(TArray<AActor*>{ this, GetOwner()});

	bool bHitSomething =  GetWorld()->LineTraceSingleByChannel(
		Hit, 
		AimLocation, 
		AimLocation + AimDirection * Range, 
		ECollisionChannel::ECC_GameTraceChannel1,
		Params
	);

	if (bHitSomething) HandleImpact(Hit);
	LastShotFired = GetWorld()->GetTimeSeconds();
	return true;
}

void AWeapon::HandleImpact(const FHitResult& Hit)
{	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, Hit.Location, Hit.ImpactNormal.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, Hit.Location);
	
	if (!Hit.GetActor()) return;
	
	float DamageDealt = Damage;
	if (Hit.BoneName.ToString().Compare("head") == 0)
	{
		DamageDealt *= CritMultiplier;
		UGameplayStatics::SpawnSoundAtLocation(this, CritSound, Hit.Location);
	}
	UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageDealt, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	
		
}

void AWeapon::PlayFireEffects()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComp, TEXT("MuzzleFlashSocket"));
}

void AWeapon::Reload()
{
	CurrentAmmunition = MagazinSize;
}

void AWeapon::SetAsActiveWeapon()
{
	SetActorHiddenInGame(false);
}

void AWeapon::SetInactive()
{
	SetActorHiddenInGame(true);
}

