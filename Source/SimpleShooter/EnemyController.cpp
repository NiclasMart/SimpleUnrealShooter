// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "Kismet/GameplayStatics.h"

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LineOfSightTo(Target)) ChaseTarget();
	else StopChasingTarget();

}

void AEnemyController::StopChasingTarget()
{
	StopMovement();
	ClearFocus(EAIFocusPriority::Gameplay);
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AEnemyController::ChaseTarget()
{
	MoveToActor(Target, AcceptanceRadius);
	SetFocus(Target);
}
