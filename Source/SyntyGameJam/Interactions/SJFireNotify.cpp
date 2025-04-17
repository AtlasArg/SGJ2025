// Fill out your copyright notice in the Description page of Project Settings.


#include "SJFireNotify.h"
#include "SyntyGameJam/SJBaseCharacter.h"

void USJFireNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        if (ASJBaseCharacter* Character = Cast<ASJBaseCharacter>(Owner))
        {
            Character->FinishFiringProjectile();
        }
    }
}
