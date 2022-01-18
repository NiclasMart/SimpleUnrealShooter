// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreenWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverScreenWidget::NativeConstruct()
{
	if (RestartBtn)
	{
		RestartBtn->OnClicked.AddDynamic(this, &UGameOverScreenWidget::RestartGame);
	}
}

void UGameOverScreenWidget::RestartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController) PlayerController->RestartLevel();
}
