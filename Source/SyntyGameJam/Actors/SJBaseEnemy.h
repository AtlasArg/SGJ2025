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
	// Sets default values for this character's properties
	ASJBaseEnemy();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASJAIController> SJAIController;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget; 

	/*UFUNCTION(BlueprintCallable)
	virtual void FireProjectile(FVector Location) override;*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

public:	
	virtual void Tick(float DeltaTime) override;
};
