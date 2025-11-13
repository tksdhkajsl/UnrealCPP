// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudrWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TEST_API UMainHudrWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TWeakObjectPtr<class UResourceBarWidget> StaminaBar;
	
};
