// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMotionComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCharacterMotionComponent::UCharacterMotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UCharacterMotionComponent::SetupMotionInputBindings(ACharacter* Character, UInputComponent* PlayerInputComponent)
{
	BoundCharacter = Character;

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, Character, &ACharacter::Jump);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &UCharacterMotionComponent::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &UCharacterMotionComponent::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UCharacterMotionComponent::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UCharacterMotionComponent::Turn);

	//controller bindings
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &UCharacterMotionComponent::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &UCharacterMotionComponent::TurnRate);
}

// Called when the game starts
void UCharacterMotionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterMotionComponent::MoveForward(float Value)
{
	float Speed = Value * ForwardMovementSpeed;
	BoundCharacter->AddMovementInput(BoundCharacter->GetActorForwardVector() * Speed);
}

void UCharacterMotionComponent::MoveSideways(float Value)
{
	float Speed = Value * ForwardMovementSpeed;
	BoundCharacter->AddMovementInput(BoundCharacter->GetActorRightVector() * Speed);
}

void UCharacterMotionComponent::LookUp(float Value)
{
	BoundCharacter->AddControllerPitchInput(Value * LookAroundSpeedMouse);
}

void UCharacterMotionComponent::Turn(float Value)
{
	BoundCharacter->AddControllerYawInput(Value * LookAroundSpeedMouse);
}

void UCharacterMotionComponent::LookUpRate(float Value)
{
	BoundCharacter->AddControllerPitchInput(Value * LookAroundSpeedController * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void UCharacterMotionComponent::TurnRate(float Value)
{
	BoundCharacter->AddControllerYawInput(Value * LookAroundSpeedController * UGameplayStatics::GetWorldDeltaSeconds(this));
}

