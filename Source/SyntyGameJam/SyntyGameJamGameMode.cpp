// Copyright Epic Games, Inc. All Rights Reserved.

#include "SyntyGameJamGameMode.h"
#include "SyntyGameJamPlayerController.h"
#include "SyntyGameJamCharacter.h"
#include "SyntyGameJam/Actors/SJBaseEnemy.h"
#include "SyntyGameJam/Actors/SJSpawner.h"
#include "SyntyGameJam/Actors/SJPickeableActor.h"
#include "SyntyGameJam/SyntyGameJamCharacter.h"
#include "SyntyGameJam/SJBaseCharacter.h"
#include "SyntyGameJam/Interactions/SJMessages.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagContainer.h"
//#include "GameFramework/GameplayMessageSubsystem.h"
//#include "GameplayMessageSubsystem.h"
//#include "Subsystems/GameplayMessageSubsystem.h"

ASyntyGameJamGameMode::ASyntyGameJamGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASyntyGameJamPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ASyntyGameJamGameMode::OnCharacterKilled(ASJBaseCharacter* KilledCharacter)
{
	ASyntyGameJamCharacter* PlayerCharacter = Cast<ASyntyGameJamCharacter>(KilledCharacter);
	if (IsValid(PlayerCharacter))
	{
		ShowDefeatScreen();
	}
	else 
	{
		ASJBaseEnemy* EnemyCharacter = Cast<ASJBaseEnemy>(KilledCharacter);
		if (IsValid(EnemyCharacter))
		{
			SpawnedEnemies.Remove(EnemyCharacter);
			/*if (SpawnedEnemies.Num() == 0)
			{
				ShowVictoryScreen();
			}*/
		}
	}
}

void ASyntyGameJamGameMode::OnCharacterGainedReputation(ASJBaseCharacter* ModifiedCharacter)
{
	if (ModifiedCharacter->GetReputation() >= ReputationToWin)
	{
		ASyntyGameJamCharacter* PlayerCharacter = Cast<ASyntyGameJamCharacter>(ModifiedCharacter);
		if (IsValid(PlayerCharacter))
		{
			ShowVictoryScreen();
			
		}
		else
		{
			ASJBaseEnemy* EnemyCharacter = Cast<ASJBaseEnemy>(ModifiedCharacter);
			if (IsValid(EnemyCharacter))
			{
				ShowDefeatScreen();
			}
		}
	}
}

void ASyntyGameJamGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASJSpawner::StaticClass(), FoundActors);

	for (AActor* ActorFound: FoundActors)
	{
		ASJSpawner* Spawner = Cast<ASJSpawner>(ActorFound);
		if (IsValid(Spawner))
		{
			if (Spawner->ActorHasTag(FName("EnemySpawner")))
			{
				EnemySpawnersPositions.Add(Spawner);
			}
			if (Spawner->ActorHasTag(FName("PickupSpawner")))
			{
				ItemSpawnersPositions.Add(Spawner);
			}
		}
	}

	SpawnEnemies();
	
	GetWorldTimerManager().SetTimer(SpawnGoldTimerHandle, this, &ThisClass::SpawnGold, SecondsToSpawnGold, true);
	GetWorldTimerManager().SetTimer(SpawnBulletsTimerHandle, this, &ThisClass::SpawnBullets, SecondsToSpawnBullets, true);
	GetWorldTimerManager().SetTimer(SpawnEnemiesTimerHandle, this, &ThisClass::SpawnEnemiesIfNeeded, SecondsToSpawnEnemies, true);
}

void ASyntyGameJamGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear your timers to avoid callbacks after world is tearing down
	GetWorldTimerManager().ClearTimer(SpawnGoldTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnBulletsTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnEnemiesTimerHandle);
}

void ASyntyGameJamGameMode::SpawnEnemies()
{
	if (!EnemyClass)
	{
		return;
	}
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	TArray<ASJSpawner*> AvailableSpawners = EnemySpawnersPositions;
	while (AvailableSpawners.Num() > 0)
	{
		int32 SpawnIndex = FMath::RandRange(0, AvailableSpawners.Num() - 1);
		if (AvailableSpawners.Num() > 0)
		{
			ASJSpawner* ChosenSpawner = AvailableSpawners[SpawnIndex];

			if (IsValid(ChosenSpawner))
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				ASJBaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ASJBaseEnemy>(EnemyClass, ChosenSpawner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

				if (SpawnedEnemy)
				{
					SpawnedEnemies.Add(SpawnedEnemy);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy at: %s"), *ChosenSpawner->GetActorLocation().ToString());
				}
			}

			AvailableSpawners.RemoveAt(SpawnIndex);
		}
	}
}

void ASyntyGameJamGameMode::ShowVictoryScreen()
{
	SendGameResultMessage(true);
}

void ASyntyGameJamGameMode::ShowDefeatScreen()
{
	SendGameResultMessage(false);
}

void ASyntyGameJamGameMode::SpawnBullets()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	if (ItemSpawnersPositions.Num() > 0)
	{
		int32 SpawnIndex = FMath::RandRange(0, ItemSpawnersPositions.Num() - 1);
		ASJSpawner* ChosenSpawner = ItemSpawnersPositions[SpawnIndex];

		if (IsValid(ChosenSpawner))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ASJPickeableActor* SpawnedBullets = GetWorld()->SpawnActor<ASJPickeableActor>(BulletsClass, ChosenSpawner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
			//TODO: por ahi estaria bueno dejar una constancia de que cosas hay spawneadas en el mapa 
			// y no estar spawneando cada dos por tres si hay mucho ya spawneado.
		}
	}
}

void ASyntyGameJamGameMode::SpawnGold()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	// TODO: refactor, son la misma funcion pero entra una clase nomas de diferencia.
	if (ItemSpawnersPositions.Num() > 0)
	{
		int32 SpawnIndex = FMath::RandRange(0, ItemSpawnersPositions.Num() - 1);
		ASJSpawner* ChosenSpawner = ItemSpawnersPositions[SpawnIndex];

		if (IsValid(ChosenSpawner))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ASJPickeableActor* SpawnedBullets = GetWorld()->SpawnActor<ASJPickeableActor>(GoldClass, ChosenSpawner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void ASyntyGameJamGameMode::SpawnEnemiesIfNeeded()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	if (SpawnedEnemies.Num() <= 3)
	{
		int32 SpawnIndex = FMath::RandRange(0, EnemySpawnersPositions.Num() - 1);
		ASJSpawner* ChosenSpawner = EnemySpawnersPositions[SpawnIndex];

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ASJBaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ASJBaseEnemy>(EnemyClass, ChosenSpawner->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (IsValid(SpawnedEnemy))
		{
			int32 RandomReputation = FMath::RandRange(0, 3);
			SpawnedEnemy->GrantReputation(RandomReputation);

			SpawnedEnemies.Add(SpawnedEnemy);
		}
	}
}

void ASyntyGameJamGameMode::SendGameResultMessage(bool bVictory)
{
	// TODO: another refactor to do.
	//FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("UI.ShowGameResult"));
	//FSJGameResultMessage Payload;
	//Payload.bVictory = bVictory;

	//UGameplayMessageSubsystem* MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld()); // "this" must be a WorldContext
	//if (MessageSubsystem)
	//{
	//	MessageSubsystem->BroadcastMessage(Tag, Payload);
	//}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());

		ASyntyGameJamPlayerController* GameJamPC = Cast<ASyntyGameJamPlayerController>(PC);
		GameJamPC->ShowGameResult(bVictory);

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}
