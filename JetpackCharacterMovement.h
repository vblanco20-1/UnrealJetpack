// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "JetpackCharacterMovement.generated.h"

/**
 * 
 */
UCLASS(HeaderGroup=Component)
class UJetpackCharacterMovement : public UShooterCharacterMovement
{
	GENERATED_UCLASS_BODY()

	
	/**Override the Perform Movement function so we can add the jetpack logic there */
	virtual void PerformMovement(float DeltaTime) OVERRIDE;

	/** Upwards Strenght of the jetpack, the more it is, the bigger is the acceleration for the jetpack, if its too low, the gravity has more power and you dont even fly */
	UPROPERTY(Category="Character Movement", EditAnywhere, BlueprintReadWrite)
	float JetpackStrenght;

	/** maximum fuel for the jetpack, this goes in seconds, and its depleted with simple time, so if its 2, you can only fly for 2 seconds */
	UPROPERTY(Category="Character Movement", EditAnywhere, BlueprintReadWrite)
	float JetpackMaxFuel;

	/** Multiplier for the jetpack fuel regeneration, uses the time, if its 0.5, and the JetpackMaxFuel is 2 seconds, that means that it will take 4 seconds to be back at 100% */
	UPROPERTY(Category="Character Movement", EditAnywhere, BlueprintReadWrite)
	float JetpackRefuelRate;


	/** Holds the current fuel amount */
	float Jetpackfuel;
	
};
