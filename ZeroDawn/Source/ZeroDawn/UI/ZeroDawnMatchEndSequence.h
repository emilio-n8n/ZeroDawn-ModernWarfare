#pragma once
#include "../ZeroDawn.h"
#include "../GameTypes.h"
#include "ZeroDawnMatchEndSequence.generated.h"

UCLASS()
class AZeroDawnMatchEndSequence : public AActor
{
	GENERATED_BODY()

public:
	AZeroDawnMatchEndSequence();

	UFUNCTION(BlueprintCallable, Category = "Match End")
	void ShowMatchEnd(ETeamType WinningTeam, int32 AlphaScore, int32 BravoScore, const TArray<FPlayerScoreData>& Scoreboard);

	UFUNCTION(BlueprintImplementableEvent, Category = "Match End")
	void OnMatchEnd(ETeamType WinningTeam, int32 AlphaScore, int32 BravoScore, const TArray<FPlayerScoreData>& Scoreboard);

	UFUNCTION(BlueprintCallable, Category = "Match End")
	void ShowTopPlayers(const TArray<FPlayerScoreData>& TopPlayers);

	UFUNCTION(BlueprintImplementableEvent, Category = "Match End")
	void OnShowTopPlayers(const TArray<FPlayerScoreData>& TopPlayers);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match End")
	float MatchEndDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match End")
	TSubclassOf<UUserWidget> MatchEndWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match End")
	UUserWidget* MatchEndWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match End")
	TSubclassOf<UUserWidget> FinalScoreboardWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Match End")
	UUserWidget* FinalScoreboardWidget;

	FTimerHandle ReturnToLobbyTimerHandle;

	UFUNCTION(BlueprintImplementableEvent, Category = "Match End")
	void PlayVictoryMusic();

	UFUNCTION(BlueprintImplementableEvent, Category = "Match End")
	void PlayDefeatMusic();

	UFUNCTION(BlueprintCallable, Category = "Match End")
	void ReturnToLobby();

	void PlayMatchEndSound(ETeamType PlayerTeam, ETeamType WinningTeam);
};
