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

void ASJBaseEnemy::ReceivePickeable_Implementation(ASJPickeableActor* Pickeable)
{
	Super::ReceivePickeable_Implementation(Pickeable);
	SJAIController->GetBlackboardComponent()->SetValueAsObject(FName("ClosestPickeable"), nullptr);
}

void ASJBaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	// TODO: ideally, this should have been an interaction with the saloon, not like this.
	GetWorldTimerManager().SetTimer(SpendCoinsTimerHandle, this, &ThisClass::SpendCoinsIfPossible, 25.f, true);
	GetWorldTimerManager().SetTimer(GenerateCoinsAndBulletsTimerHandle, this, &ThisClass::GenerateCoinsAndBullets, 4.f, true);
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

void ASJBaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpendCoinsTimerHandle);
	GetWorldTimerManager().ClearTimer(GenerateCoinsAndBulletsTimerHandle);
}

void ASJBaseEnemy::SpendCoinsIfPossible()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	if (!IsValid(this) || IsPendingKill())
	{
		return;
	}

	// TODO: change! magic numbers!!!
	if (GetGoldCoins() >= 5)
	{
		GrantReputation(2);
		GrantHealth(20);
		GainBullets(5);

		RemoveGoldCoins(5);
	}
}

void ASJBaseEnemy::GenerateCoinsAndBullets()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown) 
	{
		return;
	}

	if (!IsValid(this) || IsPendingKill())
	{
		return;
	}

	// TODO: change! magic numbers!!!
	if (GetBulletCount() <= 2)
	{
		GainBullets(5);
	}

	if (GetGoldCoins() < 5)
	{
		GainGoldCoins(4);
	}
}