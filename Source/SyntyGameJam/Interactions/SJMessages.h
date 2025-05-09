// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SJMessages.generated.h"

USTRUCT(BlueprintType)
struct FSJGameResultMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    bool bVictory;
};
