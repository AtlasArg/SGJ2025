// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "SyntyGameJamPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASJBaseEnemy;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowGameResultEvent, bool, bValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewEnemyCreated, ASJBaseEnemy*, NewEnemy);

UCLASS()
class ASyntyGameJamPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASyntyGameJamPlayerController();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnShowGameResultEvent OnShowGameResultEvent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNewEnemyCreated OnNewEnemyCreated;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	void ShowGameResult(bool bVictory);

	void TrackNewEnemyOnMinimap(ASJBaseEnemy* SpawnedEnemy);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay();
	void OnInputStarted();

	void Move(const FInputActionValue& InputActionValue);

private:
	void FireProjectile();

	void InteractWithPlace();
	FVector CachedDestination;
};


