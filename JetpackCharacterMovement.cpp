// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"



UJetpackCharacterMovement::UJetpackCharacterMovement(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	JetpackStrenght = 3000;

	JetpackMaxFuel = 2;
	JetpackRefuelRate = 0.5;
	

	Jetpackfuel = 0;
}





void  UJetpackCharacterMovement::PerformMovement(float DeltaTime) 
{

	// get the owner
	 AJetpackCharacter* ShooterCharacterOwner = Cast<AJetpackCharacter>(PawnOwner);
	if (ShooterCharacterOwner)	
	{
		// using jetpack, so fly up
		if(ShooterCharacterOwner->bIsUsingJetpack)
		{
			// make fuel decrease, as you are flying
			Jetpackfuel -= DeltaTime;
			// jetpack is depleted, disable it and stop flying
			if(Jetpackfuel < 0) 
			{
				ShooterCharacterOwner->StopJetpack();
				ShooterCharacterOwner->DisableJetpack();
			}
			// Add some acceleration to the Upward velocity, so the character is propulsed upwards
			Velocity.Z += JetpackStrenght*DeltaTime	;
		}
		
		else if(ShooterCharacterOwner->bIsJetpackEnabled == true)
		{
			// only refuel the jetpack when you are not flying and the jetpack is enabled
			Jetpackfuel+= DeltaTime*JetpackRefuelRate;
			if(Jetpackfuel >= JetpackMaxFuel)
			{
				Jetpackfuel = JetpackMaxFuel;
			}
		}
	}	
	
	// do the CharacterMovement version of PerformMovement, this function is the one that does the normal movement calculations.
	Super::PerformMovement(DeltaTime);
}

