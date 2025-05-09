// Fill out your copyright notice in the Description page of Project Settings.


#include "SJBaseCharacter.h"
#include "Actors/BaseProjectile.h"
#include "Actors/SJAttributeComponent.h"
#include "Actors/SJPickeableActor.h"
#include "Actors/SJInteractableActor.h"
#include "SyntyGameJam/SyntyGameJam.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASJBaseCharacter::ASJBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	/*GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);*/

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	//TODO: Check, not sure about this...
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AttributeComponent = CreateDefaultSubobject<USJAttributeComponent>("AttributeComp");
}

void ASJBaseCharacter::ReceivePickeable_Implementation(ASJPickeableActor* Pickeable)
{
	switch (Pickeable->PickeableType)
	{
	case EPickeableType::Bullets:
		GainBullets(Pickeable->Amount);
		break;
	case EPickeableType::Coins:
		GainGoldCoins(Pickeable->Amount);
		break;
	case EPickeableType::Health:
		break;
	default:
		break;
	}

	Pickeable->Destroy();
}

void ASJBaseCharacter::FinishFiringProjectile()
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, FireSoundCue, GetActorLocation(), 1.0f, 1.0f, 0.0f, SoundAttenuation);

	FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
	FVector MuzzleLocation = MuzzleTransform.GetLocation();

	FVector ShootDirection = (FireTargetLocation - MuzzleLocation).GetSafeNormal();
	FRotator MuzzleRotation = ShootDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	Bullets--;

	if (OnFProjectileFired.IsBound())
	{
		OnFProjectileFired.Broadcast();
	}
}

void ASJBaseCharacter::SetFireTargetLocation(FVector FireLocation)
{
	FireTargetLocation = FireLocation;
}

bool ASJBaseCharacter::CanMove()
{
	return !bDead;
}

void ASJBaseCharacter::GrantReputation(int32 ReputationGranted)
{
	if (AttributeComponent)
	{
		AttributeComponent->GrantReputation(ReputationGranted);
		if (OnReputationChanged.IsBound())
		{
			OnReputationChanged.Broadcast(GetReputation());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute Component is null in ASJBaseCharacter, GrantReputation"));
	}

}

int32 ASJBaseCharacter::GetReputation()
{
	return AttributeComponent->GetReputation();
}

void ASJBaseCharacter::GrantHealth(float HealthGained)
{
	if (AttributeComponent)
	{
		AttributeComponent->ApplyHealthChange(this, HealthGained);
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(AttributeComponent->GetHealth());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute Component is null in ASJBaseCharacter, GrantHealth"));
	}
}

void ASJBaseCharacter::LooseHealth(AActor* ActorInstigator, float HealthLost)
{
	AttributeComponent->ApplyHealthChange(ActorInstigator, HealthLost);
	if (HurtSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, SoundAttenuation);
	}

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(AttributeComponent->GetHealth());
	}
}

void ASJBaseCharacter::SetCurrentInteractable(ASJInteractableActor* NewInteractable)
{
	CurrentInteractable = NewInteractable;
}

void ASJBaseCharacter::InteractWithMapElement()
{
	if (CurrentInteractable && CurrentInteractable->CanInteract(this))
	{
		CurrentInteractable->Interact(this);
	}
}

// Called when the game starts or when spawned
void ASJBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	AttributeComponent->OnOwnerKilled.AddDynamic(this, &ThisClass::CharacterDied);
}

void ASJBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(FireCooldownHandle);
}

// Called every frame
void ASJBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASJBaseCharacter::FireProjectile(FVector Location)
{
	if (!bCanFire) 
	{
		return;
	}

	if (Bullets <= 0)
	{
		FVector Location = GetActorLocation(); 
		UGameplayStatics::PlaySoundAtLocation(this, EmptyPistolCue, Location);

		return;
	}

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireCooldownHandle, this, &ThisClass::ResetFire, FireCooldownSeconds, false);

	if (!ProjectileClass)
	{
		return;
	}

	SetFireTargetLocation(Location);
	UpdateFacingTarget(Location);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (FireMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void ASJBaseCharacter::GainBullets(int32 GainedBullets)
{
	Bullets += GainedBullets;
	if (OnBulletsUpdated.IsBound())
	{
		OnBulletsUpdated.Broadcast(GoldCoins);
	}
}

void ASJBaseCharacter::GainGoldCoins(int32 GainedCoins)
{
	GoldCoins += GainedCoins;
	if (OnCoinsUpdated.IsBound())
	{
		OnCoinsUpdated.Broadcast(GoldCoins);
	}
}

void ASJBaseCharacter::RemoveGoldCoins(int32 RemovedCoins)
{
	GoldCoins -= RemovedCoins;
	if (OnCoinsUpdated.IsBound())
	{
		OnCoinsUpdated.Broadcast(GoldCoins);
	}
}

void ASJBaseCharacter::ResetFire()
{
	bCanFire = true;
}

void ASJBaseCharacter::CharacterDied()
{
	bDead = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (DeathMontage && AnimInstance)
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded);
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_SetBlendingOutDelegate(MontageEndedDelegate, DeathMontage);
	}
}

void ASJBaseCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// TODO: we can refactor this to one method that returns the pickeable and then do the assignation
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}

	int32 RandomOffsetInX = FMath::RandRange(50, 200);
	int32 RandomOffsetInY = FMath::RandRange(50, 200);
	FVector CharacterLocation = GetActorLocation();
	FVector LocationToSpawn = FVector(CharacterLocation.X + RandomOffsetInX, CharacterLocation.Y + RandomOffsetInY, CharacterLocation.Z);

	if (DropeableBulletsClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ASJPickeableActor* ItemToPick = GetWorld()->SpawnActor<ASJPickeableActor>(DropeableBulletsClass, LocationToSpawn, FRotator(), SpawnParams);
		if (IsValid(ItemToPick))
		{
			ItemToPick->Amount = GetBulletCount();
		}
	}

	if (DropeableCoinsClass)
	{
		RandomOffsetInX = FMath::RandRange(50, 200);
		RandomOffsetInY = FMath::RandRange(50, 200);
		LocationToSpawn = FVector(CharacterLocation.X + RandomOffsetInX, CharacterLocation.Y + RandomOffsetInY, CharacterLocation.Z);
		FActorSpawnParameters SpawnParamsCoins;
		SpawnParamsCoins.Owner = this;
		SpawnParamsCoins.Instigator = GetInstigator();
		ASJPickeableActor* ItemToPick = GetWorld()->SpawnActor<ASJPickeableActor>(DropeableCoinsClass, LocationToSpawn, FRotator(), SpawnParamsCoins);
		if (IsValid(ItemToPick))
		{
			ItemToPick->Amount = GetGoldCoins();
		}
	}

	Destroy();
}
