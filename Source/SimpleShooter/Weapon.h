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

	bool PullTrigger(FVector AimLocation, FVector AimDirection, bool bIsMiss = false);
	void Reload();
	void SetAsActiveWeapon();
	void SetInactive();

	float GetReloadTime() const { return ReloadTime; };
	int32 GetCurrentAmmo() const { return CurrentAmmunition; };
	int32 GetTotalAmmo() const { return MagazinSize; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayFireEffects();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, Category="Effects")
		class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category ="Effects")
		UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere, Category = "Effects")
		class USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* CritSound;

	UPROPERTY(EditDefaultsOnly, Category="Stats")
		int32 MagazinSize = 25;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float AutoFrequency = 2.3f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Damage = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		int32 Range = 2000;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float ReloadTime = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float CritMultiplier = 1.f;

	int32 CurrentAmmunition;
	float LastShotFired;

	void HandleImpact(const FHitResult& Hit);

};
