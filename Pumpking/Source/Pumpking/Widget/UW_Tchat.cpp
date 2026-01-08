// Fill out your copyright notice in the Description page of Project Settings.

#include "UW_Tchat.h"
#include "Pumpking/Player/PlayerCharacter.h"
#include "Pumpking/PlayerController/PlayerCharacterController.h"
#include <Macro.h>

void UUW_Tchat::AddText(const FText& _text, bool _isSelf)
{
	UBorder* _border = CreateMessageBubble(_text, _isSelf);
	AddMessageToScrollBox(_border, _isSelf);
	scrollBox->ScrollToEnd();
	textField->SetText(FText::GetEmpty());
	textField->SetFocus();
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
	TObjectPtr<APlayerController> _pc = GetWorld()->GetFirstPlayerController();
	if (!_pc) return;

	ESlateVisibility _visibility = isOpen ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	SetVisibility(_visibility);
	isOpen = !isOpen;
	_pc->SetShowMouseCursor(isOpen);
	if (!isOpen) return;
	textField->SetFocus();
}

FSlateBrush UUW_Tchat::CreateMessageBrush(bool _isSelf)
{
	FSlateBrush _brush;
	_brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	_brush.TintColor = FSlateColor(
		_isSelf
		? FLinearColor(0.2f, 0.6f, 1.f, 0.2f)
		: FLinearColor(0.1f, 0.1f, 0.1f, 0.5f)
	);
	return _brush;
}

TObjectPtr<UTextBlock> UUW_Tchat::CreateMessageText(const FText& Text)
{
	TObjectPtr<UTextBlock> _textBlock = NewObject<UTextBlock>(this);
	_textBlock->SetText(Text);
	_textBlock->SetAutoWrapText(true);
	_textBlock->SetWrapTextAt(250.f);

	return _textBlock;
}

TObjectPtr<UBorder> UUW_Tchat::CreateMessageBubble(const FText& Text, bool _isSelf)
{
	TObjectPtr<UBorder> _border = NewObject<UBorder>(this);
	_border->SetPadding(FMargin(10.f));
	_border->SetBrush(CreateMessageBrush(_isSelf));

	_border->SetContent(CreateMessageText(Text));

	return _border;
}

void UUW_Tchat::AddMessageToScrollBox(TObjectPtr<UBorder> _border, bool _isSelf)
{
	if (TObjectPtr<UScrollBoxSlot> _slot = Cast<UScrollBoxSlot>(scrollBox->AddChild(_border)))
	{
		_slot->SetHorizontalAlignment(_isSelf ? HAlign_Right : HAlign_Left);
		_slot->SetPadding(FMargin(5.f));
	}
}