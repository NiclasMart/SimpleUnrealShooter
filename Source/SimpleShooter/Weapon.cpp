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

bool AWeapon::Fire()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, MeshComp, TEXT("MuzzleFlashSocket"));
	UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
	return true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

