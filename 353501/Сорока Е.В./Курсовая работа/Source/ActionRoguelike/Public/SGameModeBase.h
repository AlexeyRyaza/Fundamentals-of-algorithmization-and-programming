// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class UDataTable;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;


/* DataTable Row for spawning monsters in game mode   */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FMonsterInfoRow()
    {
        Weight = 1.0;
        SpawnCost = 5.0f;
        KillReward = 20.f;
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FPrimaryAssetId MonsterId;

    //TSubclassOf<AActor> MonsterClass;

    /* Relative chance to pick this monster */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Weight;

    /* Points required by game mode to spawn this unit. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SpawnCost;

    /* Amount of credits awarded to killer of this unit.  */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float KillReward;
};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

protected:
    FString SlotName;
    
    UPROPERTY()
    USSaveGame* CurrentSaveGame;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UDataTable* MonsterTable;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TSubclassOf<AActor> MinionClass;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UEnvQuery* SpawnBotQuery;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UCurveFloat* DifficultyCurve;

    FTimerHandle TimerHandle_SpawnBots;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float SpawnTimerInterval;

    // Read/write access as we could change this as our difficulty increases via Blueprint
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    int32 CreditsPerKill;

    UPROPERTY(EditDefaultsOnly, Category = "Powerups")
    UEnvQuery* PowerupSpawnQuery;

    /* All power-up classes used to spawn with EQS at match start */
    UPROPERTY(EditDefaultsOnly, Category = "Powerups")
    TArray<TSubclassOf<AActor>> PowerupClasses;

    /* Distance required between power-up spawn locations */
    UPROPERTY(EditDefaultsOnly, Category = "Powerups")
    float RequiredPowerupDistance;

    /* Amount of powerups to spawn during match start */
    UPROPERTY(EditDefaultsOnly, Category = "Powerups")
    int32 DesiredPowerupCount;

    UFUNCTION()
    void SpawnBotTimerElapsed();

    UFUNCTION()
    void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    UFUNCTION()
    void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

    UFUNCTION()
    void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
        EEnvQueryStatus::Type QueryStatus);

    UFUNCTION()
    void RespawnPlayerElapsed(AController* Controller);

public:
    virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

    ASGameModeBase();

    void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    virtual void StartPlay() override;

    void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    UFUNCTION(Exec)
    void KillAll();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void WriteSaveGame();

    void LoadSaveGame();
};
