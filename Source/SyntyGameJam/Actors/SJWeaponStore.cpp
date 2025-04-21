// Fill out your copyright notice in the Description page of Project Settings.


#include "SJWeaponStore.h"
#include "SyntyGameJam/SJBaseCharacter.h"

void ASJWeaponStore::Interact(ASJBaseCharacter* InteractingCharacter)
{
	InteractingCharacter->GainBullets(Bullets);
	InteractingCharacter->RemoveGoldCoins(Cost);
}
