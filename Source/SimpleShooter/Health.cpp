// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
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
	UE_LOG(LogTemp, Warning, TEXT("Take %f Damage, Remaining Health: %d"), Damage, CurrentHealth);
}

