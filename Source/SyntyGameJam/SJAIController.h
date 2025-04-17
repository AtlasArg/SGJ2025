// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SJAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class SYNTYGAMEJAM_API ASJAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASJAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
