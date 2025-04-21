// Fill out your copyright notice in the Description page of Project Settings.


#include "SJOverlay.h"
#include "SyntyGameJamPlayerController.h"

void USJOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ASyntyGameJamPlayerController* SJPC = Cast<ASyntyGameJamPlayerController>(PC))
		{
		//	SJPC->OnShowGameResultEvent.AddDynamic(this, &UTuWidget::HandleBoolMessage);
		}
	}
}
