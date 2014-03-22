// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "JetpackCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AJetpackCharacter : public AShooterCharacter
{
	GENERATED_UCLASS_BODY()

	
	UPROPERTY(Transient, Replicated)
	uint8 bIsUsingJetpack : 1;

	UFUNCTION(BlueprintCallable, Category=Jetpack)
	virtual void StartJetpack();
	
	UFUNCTION(BlueprintCallable, Category=Jetpack)
	virtual void StopJetpack();

	virtual void DisableJetpack();
	virtual void EnableJetpack();

 

	int WallJumpTraces;
	float WallJumpTraceDistance;

	UFUNCTION(BlueprintCallable, Category = Jetpack)
	void WallJump();
	bool bIsJetpackEnabled;

	/**This is the time the jetpack has to cool down, when you deplete it completely */ 
	UPROPERTY(Category="Jetpack", EditAnywhere, BlueprintReadWrite)
	float JetpackRecovery;

	UPROPERTY(Category="Jetpack", EditAnywhere, BlueprintReadWrite)
	float WalljumpHorizontalStrenght;
	UPROPERTY(Category="Jetpack", EditAnywhere, BlueprintReadWrite)
	float WalljumpUpwardsStrenght;

	UFUNCTION(reliable, server,WithValidation)
	virtual void ServerSetJetpack(bool bNewJetpack);	
	UFUNCTION(reliable, server, WithValidation)
	virtual void ServerAddForce(FVector force);

	virtual bool CanJump() const OVERRIDE;
};
