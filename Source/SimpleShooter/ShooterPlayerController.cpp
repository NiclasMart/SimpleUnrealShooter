// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ShooterCharacter.h"
#include "GameOverScreenWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());

	HUD = CreateWidget(this, HUDClass);
	if (HUD) HUD->AddToViewport();
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	AShooterCharacter* PlayerChar = Cast<AShooterCharacter>(GetPawn());
	if (PlayerChar)
	{
		PlayerChar->DisableInput(this);
		PlayerChar->StopFireWeapon();
	}

	ShowEndscreen(bIsWinner);

}

void AShooterPlayerController::ShowEndscreen(bool bIsWinner)
{
	HUD->RemoveFromViewport();

	UGameOverScreenWidget* LoseScreen = Cast<UGameOverScreenWidget>(CreateWidget(this, LoseScreenClass));
	if (!LoseScreen) return;

	FText DisplayText = bIsWinner ? FText::FromString(TEXT("You have Won!")) : FText::FromString(TEXT("Game Over!"));
	LoseScreen->SetDisplayText(DisplayText);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
	LoseScreen->AddToViewport();
	

	
}
