// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRow.h"
#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USessionRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;

	RowButton->OnClicked.AddDynamic(this, &USessionRow::OnClicked);
}

void USessionRow::OnClicked()
{
	if (Parent == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[USessionRow::OnClicked] Index %i"), Index);

	//Parent->SelectIndexSessionList(Index);
}