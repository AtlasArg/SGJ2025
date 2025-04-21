// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SJInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USJInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class SYNTYGAMEJAM_API ISJInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(ACharacter* InteractingCharacter);
};
