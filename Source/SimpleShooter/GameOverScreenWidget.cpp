// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreenWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameOverScreenWidget::SetDisplayText(FText Text)
{
	if (!DisplayText) return;
	DisplayText->Text = Text;
}

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
	//if (PlayerController) PlayerController->RestartLevel();
	UGameplayStatics::OpenLevel(PlayerController, FName(*GetWorld()->GetName()), false);
}
