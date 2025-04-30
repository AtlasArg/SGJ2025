// Copyright Epic Games, Inc. All Rights Reserved.

#include "SyntyGameJamPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SyntyGameJamCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Actors/BaseProjectile.h"
#include "SyntyGameJam/SyntyGameJam.h"
#include "Actors/SJBaseEnemy.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASyntyGameJamPlayerController::ASyntyGameJamPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
}

void ASyntyGameJamPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASyntyGameJamPlayerController::ShowGameResult(bool bVictory)
{
	OnShowGameResultEvent.Broadcast(bVictory);
}

void ASyntyGameJamPlayerController::TrackNewEnemyOnMinimap(ASJBaseEnemy* SpawnedEnemy)
{
	OnNewEnemyCreated.Broadcast(SpawnedEnemy);
}

void ASyntyGameJamPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASyntyGameJamPlayerController::Move);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::FireProjectile);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InteractWithPlace);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASyntyGameJamPlayerController::Move(const FInputActionValue& InputActionValue)
{
	APawn* ControlledPawn = GetPawn<APawn>();
	ASyntyGameJamCharacter* PlayerCharacter = Cast<ASyntyGameJamCharacter>(ControlledPawn);
	if (!IsValid(PlayerCharacter) || !PlayerCharacter->CanMove())
	{
		return;
	}

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (IsValid(ControlledPawn))
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ASyntyGameJamPlayerController::FireProjectile()
{
	if (ASyntyGameJamCharacter* SyntyCharacter = Cast<ASyntyGameJamCharacter>(GetPawn()))
	{

		FVector WorldLocation, WorldDirection;

		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			// Altura a la que queremos disparar (altura del personaje)
			float CharacterZ = SyntyCharacter->GetActorLocation().Z;

			// Plane equation: (PointOnPlane - RayOrigin) . PlaneNormal = 0
			float t = (CharacterZ - WorldLocation.Z) / WorldDirection.Z;

			FVector IntersectionPoint = WorldLocation + WorldDirection * t;

			// Ahora disparás al punto correcto:
			SyntyCharacter->FireProjectile(IntersectionPoint);

		}
	}
}

void ASyntyGameJamPlayerController::InteractWithPlace()
{
	if (ASyntyGameJamCharacter* SyntyCharacter = Cast<ASyntyGameJamCharacter>(GetPawn()))
	{
		SyntyCharacter->InteractWithMapElement();
	}
}
