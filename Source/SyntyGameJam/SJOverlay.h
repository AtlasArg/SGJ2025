// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SJOverlay.generated.h"

/**
 * 
 */
UCLASS()
class SYNTYGAMEJAM_API USJOverlay : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;
};
