#pragma once
#include "../ZeroDawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "ZeroDawnMainMenuWidget.generated.h"

class UZeroDawnSessionSubsystem;

/**
 * UZeroDawnMainMenuWidget
 *
 * Main menu user widget with Host Game and Find Game buttons bound
 * to the session subsystem's CreateSession / FindSessions functions.
 * Displays discovered sessions in a scrollable list with Join buttons.
 */
UCLASS()
class UZeroDawnMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// ============================================================
	// Bound Widgets (from UMG blueprint)
	// ============================================================

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> HostGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FindGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SessionResultsBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;

	// ============================================================
	// Session Configuration
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Session")
	int32 MaxPlayers = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Session")
	FString DefaultMapName = TEXT("Nuketown");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Session")
	FString DefaultGameMode = TEXT("TeamDeathmatch");

	// ============================================================
	// Button Handlers
	// ============================================================

	UFUNCTION()
	void OnHostGameClicked();

	UFUNCTION()
	void OnFindGameClicked();

	UFUNCTION()
	void OnJoinSessionClicked(int32 SessionIndex);

	// ============================================================
	// Session Subsystem Delegates
	// ============================================================

	UFUNCTION()
	void OnCreateSessionCompleteBP(bool bSuccessful);

	UFUNCTION()
	void OnFindSessionsCompleteBP(bool bSuccessful);

	// ============================================================
	// Helpers
	// ============================================================

	void PopulateSessionList();
	void SetStatus(const FString& Message);

private:
	/** Cached pointer to the session subsystem — retrieved once in NativeConstruct. */
	TWeakObjectPtr<UZeroDawnSessionSubsystem> SessionSubsystem;
};
