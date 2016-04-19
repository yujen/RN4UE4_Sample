// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ping.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(RN4UE4Sample, Log, All);


UCLASS()
class RN4UE4_SAMPLE_API APing : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APing();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "RakNet")
	void StartServer(FString responseString);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RakNet")
	FString serverResponseString;

	UFUNCTION(BlueprintCallable, Category = "RakNet")
	void ClientPing();
	
};
