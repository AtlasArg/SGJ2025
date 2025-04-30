// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapUserWidget.h"
#include "SyntyGameJam/SyntyGameJamCharacter.h"
#include "SyntyGameJam/SyntyGameJamPlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "SyntyGameJam/Actors/SJBaseEnemy.h"
#include "SyntyGameJam/Actors/SJSaloon.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Kismet/GameplayStatics.h"

USJIconInMapWidget* UMinimapUserWidget::AddIconToMap(EMapIconType MapIcon, AActor* RelatedActor)
{
	ensure(DefaultIconWidgetClass);
	UUserWidget* NewItemWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultIconWidgetClass);
	if (USJIconInMapWidget* ItemWidget = Cast<USJIconInMapWidget>(NewItemWidget))
	{
		UOverlaySlot* OverlaySlot = MapOverlay->AddChildToOverlay(ItemWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

		ItemWidget->SetupWidget(MapIcon, RelatedActor);
		return ItemWidget;
	}

	return nullptr;
}

void UMinimapUserWidget::AddIconsToMap()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASJBaseEnemy::StaticClass(), FoundActors);

	ASyntyGameJamPlayerController* SyntyGameJamPlayerController = Cast<ASyntyGameJamPlayerController>(GetOwningPlayer());
	ASyntyGameJamCharacter* PlayerCharacter = Cast<ASyntyGameJamCharacter>(SyntyGameJamPlayerController->GetPawn());
	PlayerIcon = AddIconToMap(EMapIconType::PLAYER, PlayerCharacter);

	for (AActor* foundActor : FoundActors)
	{
		ASJBaseEnemy* BaseEnemy = Cast<ASJBaseEnemy>(foundActor);
		if (IsValid(BaseEnemy))
		{
			USJIconInMapWidget* IconMapWidet = AddIconToMap(EMapIconType::ENEMY, BaseEnemy);
			EnemyIcons.Add(IconMapWidet);
		}
	}

	TArray<AActor*> FoundSaloons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASJSaloon::StaticClass(), FoundSaloons);
	for (AActor* foundActor : FoundSaloons)
	{
		ASJSaloon* Saloon = Cast<ASJSaloon>(foundActor);
		if (IsValid(Saloon))
		{
			USJIconInMapWidget* IconMapWidet = AddIconToMap(EMapIconType::POINT_OF_INTEREST, Saloon);
			PointsOfInterestIcons.Add(IconMapWidet);
		}
	}
}

void UMinimapUserWidget::AddNewEnemyIconToMap(ASJBaseEnemy* NewEnemy)
{
	if (IsValid(NewEnemy))
	{
		USJIconInMapWidget* IconMapWidet = AddIconToMap(EMapIconType::ENEMY, NewEnemy);
		EnemyIcons.Add(IconMapWidet);
	}
}

void UMinimapUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HalfUISizeInX = MapSizeBoxContainerSize / 2;
	HalfUISizeInY = MapSizeBoxContainerSize / 2;

	//AddIconsToMap();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UMinimapUserWidget::AddIconsToMap);
}

void UMinimapUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateEnemyIcons();
	UpdatePointsOfInterestIcons();
}

void UMinimapUserWidget::UpdateIconRotation(USJIconInMapWidget* IconToRotate)
{
	if (IsValid(IconToRotate) && IsValid(IconToRotate->RelatedWidgetActor))
	{
		FRotator Rotation = IconToRotate->RelatedWidgetActor->GetActorRotation();
		IconToRotate->SetRenderTransformAngle(Rotation.Yaw);
	}
}

void UMinimapUserWidget::UpdateEnemyIcons()
{
	if (EnemyIcons.Num() > 0)
	{
		UpdateIcons(EnemyIcons, true, false);
	}
}

void UMinimapUserWidget::UpdatePointsOfInterestIcons()
{
	if (PointsOfInterestIcons.Num() > 0)
	{
		UpdateIcons(PointsOfInterestIcons, false, true);
	}
}

FVector2D UMinimapUserWidget::GetActorScreenOffset(FVector ActorLocation, FVector CameraLocation, FRotator CameraRotation)
{
	// Proyecto posiciones al plano XY (ya que el minimapa es una vista desde arriba)
	const FVector2D ActorXY(ActorLocation.X, ActorLocation.Y);
	const FVector2D CameraXY(CameraLocation.X, CameraLocation.Y);

	// Diferencia en mundo
	const FVector2D Delta = ActorXY - CameraXY;

	// Posición relativa en el rango [-1, 1]
	const float RelativeX = FMath::Clamp(Delta.Y / HalfCameraWidth, -1.f, 1.f);
	const float RelativeY = FMath::Clamp(Delta.X / HalfCameraWidth, -1.f, 1.f);

	// Convertimos a coordenadas de pantalla (widget)
	FVector2D ScreenOffset;
	ScreenOffset.X = RelativeX * HalfUISizeInX;
	ScreenOffset.Y = -RelativeY * HalfUISizeInY; // Invertido si la Y de UI crece hacia abajo

	return ScreenOffset;
}

void UMinimapUserWidget::UpdateIcons(TArray<USJIconInMapWidget*>& IconsInMap, bool bHideIfHavingTag, bool bUpdateColor)
{
	ASyntyGameJamPlayerController* SyntyGameJamPlayerController = Cast<ASyntyGameJamPlayerController>(GetOwningPlayer());
	if (!IsValid(SyntyGameJamPlayerController))
	{
		return;
	}

	APawn* ControlledPawn = SyntyGameJamPlayerController->GetPawn();

	if (!IsValid(ControlledPawn))
	{
		return;
	}
	ASyntyGameJamCharacter* PlayerCharacter = Cast<ASyntyGameJamCharacter>(SyntyGameJamPlayerController->GetPawn());
	USceneCaptureComponent2D* MinimapCamera = PlayerCharacter->GetMinimapCamera();
	HalfCameraWidth = MinimapCamera->OrthoWidth / 2;
	FVector ActorLocation = ControlledPawn->GetActorLocation();


	TArray <USJIconInMapWidget*> MarkedToRemove;
	for (USJIconInMapWidget* IconInMap : IconsInMap)
	{
		if (!IsValid(IconInMap->RelatedWidgetActor))
		{
			MarkedToRemove.Add(IconInMap);
			continue;
		}

		if (!IsValid(IconInMap->RelatedWidgetActor))
		{
			IconInMap->RelatedWidgetActor = IconInMap->RelatedWidgetActor;
		}

		AActor* TeamMemberPawn = IconInMap->RelatedWidgetActor;

		if (IsValid(TeamMemberPawn))
		{
			FVector TeamMemberLocation = TeamMemberPawn->GetActorLocation();

			FVector2D NewRelativePosition = GetActorScreenOffset(TeamMemberLocation, MinimapCamera->GetComponentLocation(), MinimapCamera->GetComponentRotation());
			IconInMap->SetRenderTranslation(NewRelativePosition);

			UpdateIconRotation(IconInMap);
		}
	}

	for (USJIconInMapWidget* IconInMap : MarkedToRemove)
	{
		if (IsValid(IconInMap))
		{
			IconsInMap.Remove(IconInMap);
			MapOverlay->RemoveChild(IconInMap);
		}
	}
}
