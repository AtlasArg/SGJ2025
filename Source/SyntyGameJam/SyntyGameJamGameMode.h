// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SyntyGameJamGameMode.generated.h"

class ASJBaseEnemy;
class ASJSpawner;
class ASJBaseCharacter;
class ASJPickeableActor;

UCLASS(minimalapi)
class ASyntyGameJamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASyntyGameJamGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	TSubclassOf<ASJBaseEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	TSubclassOf<ASJPickeableActor> GoldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	TSubclassOf<ASJPickeableActor> BulletsClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	float SecondsToSpawnBullets = 11.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	float SecondsToSpawnGold = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawneables")
	float SecondsToSpawnEnemies = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameConditions")
	int32 ReputationToWin = 10;
	
	void OnCharacterKilled(ASJBaseCharacter* KilledCharacter);

	void OnCharacterGainedReputation(ASJBaseCharacter* ModifiedCharacter);

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SpawnEnemies();

	void ShowVictoryScreen();

	void ShowDefeatScreen();

	void TrackNewEnemyOnMinimap(ASJBaseEnemy* SpawnedEnemy);

	UFUNCTION()
	void SpawnBullets();

	UFUNCTION() 
	void SpawnGold();
	
	UFUNCTION()
	void SpawnEnemiesIfNeeded();

	TArray<ASJBaseEnemy*> SpawnedEnemies;

	TArray<ASJSpawner*> EnemySpawnersPositions;

	TArray<ASJSpawner*> ItemSpawnersPositions;

	FTimerHandle SpawnGoldTimerHandle;

	FTimerHandle SpawnBulletsTimerHandle;

	FTimerHandle SpawnEnemiesTimerHandle;

	void SendGameResultMessage(bool bVictory);
};



