// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
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

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeapon::PullTrigger(FVector AimLocation,  FVector AimDirection)
{
	if (CurrentAmmunition <= 0) return false;
	
	Fire();

	DrawDebugLine(GetWorld(), AimLocation, AimLocation + AimDirection * 1000.f, FColor::Red, false, 3.f);
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, AimLocation, AimLocation + AimDirection * Range, ECollisionChannel::ECC_GameTraceChannel1);
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, Hit.Location, Hit.ImpactNormal.Rotation());

		if (Hit.GetActor())
			UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());			
	}
	
	return true;
	
}

void AWeapon::Fire()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("MuzzleFlashSocket"));
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

