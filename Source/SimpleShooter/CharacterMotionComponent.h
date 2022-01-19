// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMotionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UCharacterMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterMotionComponent();

	void SetupMotionInputBindings(ACharacter* Character, UInputComponent* PlayerInputComponent);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
		float ForwardMovementSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float LookAroundSpeedMouse = 1.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float LookAroundSpeedController = 1.f;

	ACharacter* BoundCharacter;

	void MoveForward(float Value);
	void MoveSideways(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void LookUpRate(float Value);
	void TurnRate(float Value);

		
};
