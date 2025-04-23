// Fill out your copyright notice in the Description page of Project Settings.


#include "SJSaloon.h"
#include "SyntyGameJam/SJBaseCharacter.h"

void ASJSaloon::Interact(ASJBaseCharacter* InteractingCharacter)
{
	InteractingCharacter->GrantReputation(Reputation);
	InteractingCharacter->GrantHealth(Health);

	InteractingCharacter->GainBullets(Bullets); // this was part of the saloon

	InteractingCharacter->RemoveGoldCoins(Cost);
}

bool ASJSaloon::CanInteract(ASJBaseCharacter* InteractingCharacter)
{
	return InteractingCharacter->GetGoldCoins() >= Cost;

}
