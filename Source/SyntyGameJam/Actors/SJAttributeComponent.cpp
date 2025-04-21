// Fill out your copyright notice in the Description page of Project Settings.


#include "SJAttributeComponent.h"
#include "SyntyGameJam/SyntyGameJamGameMode.h"
#include "SyntyGameJam/SJBaseCharacter.h"

USJAttributeComponent::USJAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	HealthMax = 100.0f;
	Health = HealthMax;

	Reputation = 1;
}

void USJAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USJAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USJAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USJAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USJAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}


float USJAttributeComponent::GetHealth() const
{
	return Health;
}


float USJAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

int32 USJAttributeComponent::GetReputation() const
{
	return Reputation;
}

void USJAttributeComponent::GrantReputation(int32 ReputationGranted)
{
	Reputation += ReputationGranted;

	ASyntyGameJamGameMode* GM = GetWorld()->GetAuthGameMode<ASyntyGameJamGameMode>();
	if (GM)
	{
		ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(GetOwner());
		GM->OnCharacterGainedReputation(BaseCharacter);
	}
}

bool USJAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	/*if (Delta < 0.0f)
	{
		const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}*/

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;
	Health = NewHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	}

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		ASyntyGameJamGameMode* GM = GetWorld()->GetAuthGameMode<ASyntyGameJamGameMode>();
		if (GM)
		{
			ASJBaseCharacter* BaseCharacter = Cast<ASJBaseCharacter>(GetOwner());
			GM->OnCharacterKilled(BaseCharacter);
		}

		if (OnOwnerKilled.IsBound())
		{
			OnOwnerKilled.Broadcast();
		}
	}


	return ActualDelta != 0;
}

