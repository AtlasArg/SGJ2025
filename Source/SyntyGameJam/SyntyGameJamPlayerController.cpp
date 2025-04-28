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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASyntyGameJamPlayerController::ASyntyGameJamPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	//FollowTime = 0.f;
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

void ASyntyGameJamPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		/*EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ASyntyGameJamPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ASyntyGameJamPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ASyntyGameJamPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ASyntyGameJamPlayerController::OnSetDestinationReleased);*/

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASyntyGameJamPlayerController::Move);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::FireProjectile);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::InteractWithPlace);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASyntyGameJamPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ASyntyGameJamPlayerController::OnSetDestinationTriggered()
{

	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ASyntyGameJamPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	//if (FollowTime <= ShortPressThreshold)
	//{
	//	// We move there and spawn some particles
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	//}

	//FollowTime = 0.f;
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
