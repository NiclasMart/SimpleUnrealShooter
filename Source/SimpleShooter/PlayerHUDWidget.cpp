// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Health.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "Weapon.h"

bool UPlayerHUDWidget::Initialize()
{
	bool bInit = Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("Initialize HUD"));

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		UHealth* Health = Cast<UHealth>(PlayerPawn->GetComponentByClass(UHealth::StaticClass()));
		if (Health) Health->OnValueChanged.AddUObject(this, &UPlayerHUDWidget::UpdateLifeDisplay);

		AShooterCharacter* Character = Cast<AShooterCharacter>(PlayerPawn);
		if (Character)
		{
			Character->OnMunitionChanged.AddUObject(this, &UPlayerHUDWidget::UpdateAmmoDisplay);
		}
	}
	return bInit;
}

void UPlayerHUDWidget::UpdateLifeDisplay(float Value)
{
	LifeBar->SetPercent(Value);
}

void UPlayerHUDWidget::UpdateAmmoDisplay(int32 Current, int32 Total)
{	
	TotalAmmo->SetText(FText::FromString(FString::FromInt(Total)));
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(Current)));
}
