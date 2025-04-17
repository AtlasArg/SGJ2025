// Fill out your copyright notice in the Description page of Project Settings.


#include "SJAttributeComponent.h"
//#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USJAttributeComponent::USJAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	HealthMax = 100.0f;
	Health = HealthMax;
}


// Called when the game starts
void USJAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

//void USJAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(USJAttributeComponent, Health);
//	DOREPLIFETIME(USJAttributeComponent, HealthMax);
//}

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

	//if (ActualDelta != 0.0f)
	//{
	//	MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	//}

	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		// TODO: handle in game mode

		/*ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}*/


		if (OnOwnerKilled.IsBound())
		{
			OnOwnerKilled.Broadcast();
		}
	}


	return ActualDelta != 0;
}

