// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

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

bool AWeapon::PullTrigger()
{
	if (CurrentAmmunition > 0)
	{
		Fire();
		return true;
	}
	else return false;
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

