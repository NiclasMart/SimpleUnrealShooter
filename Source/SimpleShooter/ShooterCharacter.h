// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

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

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AWeapon> WeaponBlueprint;

	UPROPERTY()
		AWeapon* Weapon;
	UPROPERTY()
		class UHealth* HealthComp;
	UPROPERTY()
		class APlayerController* PlayerController;

	bool bHoldingWeaponTrigger = false;

	void MoveForward(float Value);
	void MoveSideways(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void LookUpRate(float Value);
	void TurnRate(float Value);

	void StartFireWeapon();
	void StopFireWeapon();
	void FireWeapon();
};
