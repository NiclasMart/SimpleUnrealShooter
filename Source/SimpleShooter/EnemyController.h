// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void InitializeAIBehavior();

private:
	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehavior;
	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 200.f;

	UPROPERTY()
		class UBlackboardComponent* BBComponent;
	AActor* Target;

};
