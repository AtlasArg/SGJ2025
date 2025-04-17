// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SJPickeableReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USJPickeableReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SYNTYGAMEJAM_API ISJPickeableReceiver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickeable")
	void ReceivePickeable(class ASJPickeableActor* Pickeable);
};
