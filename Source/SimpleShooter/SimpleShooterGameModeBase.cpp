// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"

#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASimpleShooterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player) UGameplayStatics::PlaySoundAtLocation(this, IntroSound, Player->GetActorLocation());
	

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AShooterCharacter::StaticClass(), Enemies);
	EnemyCount = Enemies.Num() - 1;
}

void ASimpleShooterGameModeBase::ActorDied(APawn* Actor)
{
	if (Actor == Player)
	{
		HandleGameEnd(true);
	}
	else
	{
		EnemyCount--;

		UE_LOG(LogTemp, Error, TEXT("Remaining Enemies %d"), EnemyCount);
		if (EnemyCount == 0)
		{
			HandleGameEnd(false);
			UE_LOG(LogTemp, Error, TEXT("Player won"));
		}
	}
}

void ASimpleShooterGameModeBase::HandleGameEnd(bool bGameOver)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) PlayerController->GameHasEnded(PlayerController->GetPawn(), !bGameOver);
}
