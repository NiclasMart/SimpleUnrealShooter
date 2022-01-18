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

bool AWeapon::PullTrigger(FVector AimLocation,  FVector AimDirection)
{
	if (CurrentAmmunition <= 0) return false;
	if (LastShotFired + 1.f / AutoFrequency > GetWorld()->GetTimeSeconds()) return true;
	
	PlayFireEffects();

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(TArray<AActor*>{ this, GetOwner()});
	/*DrawDebugLine(GetWorld(), AimLocation, AimLocation + AimDirection * 1000.f, FColor::Red, false, 3.f);*/
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
	//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, Hit.Location, Hit.ImpactNormal.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, Hit.Location);

	if (Hit.GetActor())
		UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
}

void AWeapon::PlayFireEffects()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComp, TEXT("MuzzleFlashSocket"));
	UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ammo: %d"), --CurrentAmmunition));
}

void AWeapon::Reload()
{
	CurrentAmmunition = MagazinSize;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reloading"));
}

