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
	UFUNCTION()
		void UpdateAmmoDisplay(int32 Current, int32 Total);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* LifeBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TotalAmmo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CurrentAmmo;
	
};
