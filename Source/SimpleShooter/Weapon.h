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

	bool PullTrigger();
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
	UPROPERTY(EditAnywhere)
		class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
		int32 MagazinSize = 10;

	int32 CurrentAmmunition;

};
