// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerPawn(this, 0);
	InitializeAIBehavior();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LineOfSightTo(Target))
	{
		BBComponent->SetValueAsVector(TEXT("PlayerLocation"), Target->GetActorLocation());
		BBComponent->SetValueAsVector(TEXT("PlayerLastLocation"), Target->GetActorLocation());
	}
	else BBComponent->ClearValue(TEXT("PlayerLocation"));
}

void AEnemyController::InitializeAIBehavior()
{
	if (AIBehavior) RunBehaviorTree(AIBehavior);

	BBComponent = GetBlackboardComponent();
	BBComponent->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	BBComponent->SetValueAsObject(TEXT("Target"), Target);
}


