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

		//// Setup touch input events
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ASyntyGameJamPlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ASyntyGameJamPlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ASyntyGameJamPlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ASyntyGameJamPlayerController::OnTouchReleased);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASyntyGameJamPlayerController::Move);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::FireProjectile);
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
	// We flag that the input is being pressed
	//FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	//if (bIsTouch)
	//{
	//	bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	//}
	//else
	//{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
//	}

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

// Triggered every frame when the input is held down
void ASyntyGameJamPlayerController::OnTouchTriggered()
{
	//bIsTouch = true;
	OnSetDestinationTriggered();
}

void ASyntyGameJamPlayerController::OnTouchReleased()
{
//	bIsTouch = false;
	OnSetDestinationReleased();
}

void ASyntyGameJamPlayerController::Move(const FInputActionValue& InputActionValue)
{
	/*if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}*/

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
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
			FHitResult HitResult;
			FVector TraceStart = WorldLocation;
			FVector TraceEnd = TraceStart + WorldDirection * 10000.f; // long range trace

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this); // Ignore self

			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
			{
				SyntyCharacter->FireProjectile(HitResult.ImpactPoint);
				/*FVector TargetLocation = HitResult.ImpactPoint;

				UpdateFacingTarget(TargetLocation);

				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (FireMontage && AnimInstance)
				{
					AnimInstance->Montage_Play(FireMontage);
				}*/

				/*	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
					FVector MuzzleLocation = MuzzleTransform.GetLocation();

					FVector ShootDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();
					FRotator MuzzleRotation = ShootDirection.Rotation();

					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = GetInstigator();

					GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					Bullets--;

					if (OnFProjectileFired.IsBound())
					{
						OnFProjectileFired.Broadcast();
					}*/
			}
		}

	}
}
