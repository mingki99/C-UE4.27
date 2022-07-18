// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeDeukwooUe4.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEEDEUKWOOUE4_API UABCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadwrite, Category = Character)
	int CurrentIndex;

	UPROPERTY(EditAnyWhere, BlueprintReadwrite, Category = Character)
	int32 MaxIndex;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	UPROPERTY()
	class UButton* PrevButton;

	UPROPERTY()
	class UButton* NextButton;

	UPROPERTY()
	class UEditableTextBox* TextBox;

	UPROPERTY()
	class UButton* ConfirmButton;

private:
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();
	
};
