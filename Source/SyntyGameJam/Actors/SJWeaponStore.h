// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SyntyGameJam/Actors/SJInteractableActor.h"
#include "SJWeaponStore.generated.h"

/**
 * 
 */
UCLASS()
class SYNTYGAMEJAM_API ASJWeaponStore : public ASJInteractableActor
{
	GENERATED_BODY()

public:
	virtual void Interact(ASJBaseCharacter* InteractingCharacter) override;

	virtual bool CanInteract(ASJBaseCharacter* InteractingCharacter) override;

	UPROPERTY(VisibleAnywhere)
	int32 Bullets = 5;

	UPROPERTY(VisibleAnywhere)
	int32 Cost = 5;
};
