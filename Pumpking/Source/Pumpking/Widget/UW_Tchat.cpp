// Fill out your copyright notice in the Description page of Project Settings.

#include "UW_Tchat.h"
#include <Components/TextBlock.h>
#include <Components/ScrollBoxSlot.h>
#include "Pumpking/Player/PlayerCharacter.h"
#include "Pumpking/PlayerController/PlayerCharacterController.h"
#include <Macro.h>

void UUW_Tchat::AddText(const FText& _text, bool _isSelf)
{
	UTextBlock* _newWidget = NewObject<UTextBlock>(this);
	_newWidget->SetWrapTextAt(250.0f);
	_newWidget->SetAutoWrapText(true);
	_newWidget->SetText(_text);
	UPanelSlot* _slot = scrollBox->AddChild(_newWidget);
	if (CAST(UScrollBoxSlot, _scrollBoxSlot, _slot))
	{
		EHorizontalAlignment _alignment = _isSelf ? HAlign_Right : HAlign_Left;
		_scrollBoxSlot->SetHorizontalAlignment(_alignment);
	}
}

void UUW_Tchat::NativeConstruct()
{
	Super::NativeConstruct();
	if (!textField) return;
	textField->OnTextCommitted.AddDynamic(this, &UUW_Tchat::OnTextCommit);
}

void UUW_Tchat::OnTextCommit(const FText& _text, ETextCommit::Type _commitMethod)
{
	if (_text.IsEmpty()) return;
	if (_commitMethod != ETextCommit::OnEnter) return;
	if (TObjectPtr<APlayerController> _pc = GetWorld()->GetFirstPlayerController())
	{
		if (CAST(APlayerCharacterController, _playerController, _pc))
		{
			_playerController->SendTchatMessage(_text);
		}
	}
}

void UUW_Tchat::ToggleTchat()
{
	ESlateVisibility _visibility = isOpen ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	SetVisibility(_visibility);
	isOpen = !isOpen;
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(isOpen);
}