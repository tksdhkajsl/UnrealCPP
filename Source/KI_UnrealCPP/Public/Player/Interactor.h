// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KI_UNREALCPP_API IInteractor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	void AddInteractionTarget(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	void ClearInteractionTarget(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactor")
	void TryInteraction();
};
