// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"

#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASimpleShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AShooterCharacter::StaticClass(), Enemies);
	EnemyCount = Enemies.Num() - 1;
}

void ASimpleShooterGameModeBase::ActorDied(APawn* Actor)
{
	if (Actor == Player)
	{
		HandleGameEnd(Actor, true);
	}
	else
	{
		EnemyCount--;

		UE_LOG(LogTemp, Error, TEXT("Remaining Enemies %d"), EnemyCount);
		if (EnemyCount == 0)
		{
			HandleGameEnd(Actor, false);
			UE_LOG(LogTemp, Error, TEXT("Player won"));
		}
	}
}

void ASimpleShooterGameModeBase::HandleGameEnd(APawn* Actor, bool bGameOver)
{
	APlayerController* PlayerController = Cast<APlayerController>(Actor->GetController());
	if (PlayerController) PlayerController->GameHasEnded(nullptr, bGameOver);
}
