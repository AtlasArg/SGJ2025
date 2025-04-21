// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SyntyGameJam/Actors/SJInteractableActor.h"
#include "SJSaloon.generated.h"

/**
 * 
 */
UCLASS()
class SYNTYGAMEJAM_API ASJSaloon : public ASJInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void Interact(ASJBaseCharacter* InteractingCharacter) override;

	UPROPERTY(VisibleAnywhere)
	int32 Reputation = 2;

	UPROPERTY(VisibleAnywhere)
	int32 Health = 20;

	UPROPERTY(VisibleAnywhere)
	int32 Cost = 5;
};
