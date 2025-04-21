// Fill out your copyright notice in the Description page of Project Settings.


#include "SJSaloon.h"
#include "SyntyGameJam/SJBaseCharacter.h"

void ASJSaloon::Interact(ASJBaseCharacter* InteractingCharacter)
{
	InteractingCharacter->GrantReputation(Reputation);
	InteractingCharacter->GrantHealth(Health);

	InteractingCharacter->RemoveGoldCoins(Cost);
}
