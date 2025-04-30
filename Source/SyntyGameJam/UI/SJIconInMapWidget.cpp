// Fill out your copyright notice in the Description page of Project Settings.


#include "SJIconInMapWidget.h"
#include "Components/Image.h"

void USJIconInMapWidget::SetupWidget(EMapIconType IconType, AActor* RelatedActor)
{
	MapIconType = IconType;
	RelatedWidgetActor = RelatedActor;

	switch (MapIconType)
	{
	case EMapIconType::PLAYER:
		IconImage->SetBrushFromTexture(PlayerIcon);
		break;

	case EMapIconType::ENEMY:
		IconImage->SetBrushFromTexture(EnemyIcon);
		break;

	case EMapIconType::POINT_OF_INTEREST:
		IconImage->SetBrushFromTexture(PointsOfInterestIcon);
		break;
	}
}

