// Fill out your copyright notice in the Description page of Project Settings.


#include "SJBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SyntyGameJam/SJAIController.h"
#include "Components/WidgetComponent.h"
#include "SyntyGameJam/Actors/BaseProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"

ASJBaseEnemy::ASJBaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: Check
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	ReputationBar = CreateDefaultSubobject<UWidgetComponent>("ReputationBar");
	ReputationBar->SetupAttachment(GetRootComponent());
}

void ASJBaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SJAIController = Cast<ASJAIController>(NewController);
	SJAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	SJAIController->RunBehaviorTree(BehaviorTree);
	SJAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
}

void ASJBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASJBaseEnemy::CharacterDied()
{
	SJAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::CharacterDied();
}

void ASJBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
