// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Health.h"
#include "Kismet/GameplayStatics.h"

bool UPlayerHUDWidget::Initialize()
{
	bool bInit = Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("Widged was initialized"));
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		UHealth* Health = Cast<UHealth>(PlayerPawn->GetComponentByClass(UHealth::StaticClass()));
		if (Health) Health->OnValueChanged.AddUObject(this, &UPlayerHUDWidget::UpdateLifeDisplay);
	}
	return bInit;
}

void UPlayerHUDWidget::UpdateLifeDisplay(float Value)
{
	UE_LOG(LogTemp, Error, TEXT("CallUpdateUI %f"), Value);
	LifeBar->SetPercent(Value);
}
