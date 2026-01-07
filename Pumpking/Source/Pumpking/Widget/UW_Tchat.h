// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "UW_Tchat.generated.h"

class APlayerCharacter;

UCLASS()
class PUMPKING_API UUW_Tchat : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))	TObjectPtr<UEditableTextBox> textField = nullptr;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))	TObjectPtr<UScrollBox> scrollBox = nullptr;
	UPROPERTY(VisibleAnywhere) bool isOpen = false;

	void NativeConstruct() override;
	UFUNCTION() void OnTextCommit(const FText& _text, ETextCommit::Type _commitMethod);

public:
	void AddText(const FText& _text, bool _isSelf);
	void ToggleTchat();
};
