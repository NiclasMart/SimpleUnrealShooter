// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION()
		void UpdateLifeDisplay(float Value);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* LifeBar;
	
};
