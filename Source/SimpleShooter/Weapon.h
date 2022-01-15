// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIMPLESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	bool PullTrigger(FVector AimLocation, FVector AimDirection);

	

	void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Fire();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, Category="VFX-Effects")
		class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category ="VFX-Effects")
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditDefaultsOnly, Category="Stats")
		int32 MagazinSize = 25;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float AutoFrequency = 2.3f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Damage = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		int32 Range = 2000;

	int32 CurrentAmmunition;
	float LastShotFired;

	void HandleImpact(const FHitResult& Hit);

};
