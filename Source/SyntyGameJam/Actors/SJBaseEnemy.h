// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SyntyGameJam/SJBaseCharacter.h"
#include "SJBaseEnemy.generated.h"

class UBehaviorTree;
class ASJAIController;
class UWidgetComponent;

UCLASS()
class SYNTYGAMEJAM_API ASJBaseEnemy : public ASJBaseCharacter
{
	GENERATED_BODY()

public:
	ASJBaseEnemy();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASJAIController> SJAIController;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget; 

protected:
	virtual void BeginPlay() override;

	virtual void CharacterDied() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> ReputationBar;

	
public:	
	virtual void Tick(float DeltaTime) override;
};
