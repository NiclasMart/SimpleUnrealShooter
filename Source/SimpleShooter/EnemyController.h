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

	void StopChasingTarget();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 200.f;

	AActor* Target;

	void ChaseTarget();
};
