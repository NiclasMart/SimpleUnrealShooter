// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "ShooterCharacter.h"

UHealth::UHealth()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealth::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingLife;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealth::ApplyDamage);
}

void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealth::ApplyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Max(CurrentHealth - Damage, 0.f);
	if (IsDead())
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
		Character->HandleDeath();
	}
	UE_LOG(LogTemp, Warning, TEXT("Take %f Damage, Remaining Health: %d"), Damage, CurrentHealth);
}

