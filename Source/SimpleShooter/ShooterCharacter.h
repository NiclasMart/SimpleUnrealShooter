// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"



UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FActionDelegateInt, int32);
	DECLARE_DELEGATE_OneParam(FActionDelegateBool, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FUIUpdateEvent, int32 Current, int32 Total);
	

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	void StartFireWeapon();
	void StopFireWeapon();
	void FireWeapon();

	void UpdateAmmunitionUI();

	void HandleDeath();

	bool IsDead() const; 
	TArray<class AWeapon*> GetWeapons() const { return Weapons; };

	FUIUpdateEvent OnMunitionChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(EditAnywhere)
		float ZoomDistance = 100.f;
	UPROPERTY(EditAnywhere)
		float ShotMissProbability = 0.f;
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AWeapon>> WeaponBlueprints;
	UPROPERTY()
		TArray<AWeapon*> Weapons;
	UPROPERTY()
		int32 ActiveWeaponIndex;

	UPROPERTY()
		class USpringArmComponent* CameraArmComp;
	UPROPERTY(EditAnywhere)
		class UHealth* HealthComponent;
	UPROPERTY(EditAnywhere)
		class UCharacterMotionComponent* MotionComponent;
	UPROPERTY()
		class AController* CharacterController;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bIsReloadingWeapon = false;
	bool bHoldingWeaponTrigger = false;
	float BaseAimLevel = 0;

	UFUNCTION()
		void FinishedReloading();
	void SetAimCamera(bool bZoomActive);
	void SwitchWeapon(float Value);
	void SwitchWeaponTo(int32 Index);
	void Reloading();
	void SpawnWeapons();	
};
