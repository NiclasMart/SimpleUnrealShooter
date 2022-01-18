// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UGameOverScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDisplayText(FText Text);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RestartBtn; // name of button must be exactly like button name in widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock*  DisplayText;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
		void RestartGame();
};
