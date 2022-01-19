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

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	void StartFireWeapon();
	void StopFireWeapon();
	void FireWeapon();
	void HandleDeath();

	bool IsDead() const; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category="Movement")
		float ForwardMovementSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float LookAroundSpeedMouse = 1.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float LookAroundSpeedController = 1.f;
	UPROPERTY(EditAnywhere)
		float ZoomDistance = 100.f;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<class AWeapon>> WeaponBlueprints;
	UPROPERTY()
		TArray<AWeapon*> Weapons;
	UPROPERTY()
		int32 ActiveWeaponIndex;

	UPROPERTY()
		class USpringArmComponent* CameraArmComp;
	UPROPERTY(EditAnywhere)
		class UHealth* HealthComponent;
	UPROPERTY()
		class AController* CharacterController;

	bool bHoldingWeaponTrigger = false;
	float BaseAimLevel = 0;

	
	void SetAimCamera(bool bZoomActive);
	void SwitchWeapon(float Value);
	void SwitchWeaponTo(int32 Index);

	void MoveForward(float Value);
	void MoveSideways(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void LookUpRate(float Value);
	void TurnRate(float Value);

	void SpawnWeapons();	
};
