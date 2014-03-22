// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"


AJetpackCharacter::AJetpackCharacter(const class FPostConstructInitializeProperties& PCIP)
	:Super(PCIP.SetDefaultSubobjectClass<UJetpackCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	JetpackRecovery =2;
	bIsUsingJetpack = false;
	bIsJetpackEnabled = true;

	WallJumpTraces=20;

	WalljumpHorizontalStrenght = 1200;
	WalljumpUpwardsStrenght = 1500;
	WallJumpTraceDistance=200;
}


//////////////////////////////////////////////////////////////////////////
// Replication
void AJetpackCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// everyone except local owner: flag change is locally instigated
	DOREPLIFETIME_CONDITION(AJetpackCharacter, bIsUsingJetpack, COND_SkipOwner);

}


void AJetpackCharacter::WallJump()
{
	AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed() && !CharacterMovement->IsMovingOnGround())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walljump"));
		// Do a ring of traces
		FVector TraceStart = GetActorLocation();
		FVector Front = GetActorRotation().Vector();
		FVector Side = FVector::CrossProduct(Front,FVector::UpVector);

		DrawDebugLine(GetWorld(),TraceStart,TraceStart+Front*WallJumpTraceDistance,FColor::Blue,true,3);
		DrawDebugLine(GetWorld(),TraceStart,TraceStart+Side*WallJumpTraceDistance,FColor::Red,true,3);

		float MinDistance = 9999999;
		FVector HitLocation = FVector::ZeroVector;
		FVector HitNormal;

		for(int i = 0; i< WallJumpTraces;i++)
		{
			float traceangle = 360/WallJumpTraces * i; // angle of the trace
			

			FVector TraceDir = Front*FMath::Sin(traceangle) + Side*FMath::Cos(traceangle);
			FVector TraceEnd = TraceStart+TraceDir*WallJumpTraceDistance;
			DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Black,true,3);


			static FName TraceTag = FName(TEXT("WeaponTrace"));
			// Perform trace to retrieve hit info
			FCollisionQueryParams TraceParams(TraceTag, true, Instigator);
			TraceParams.bTraceAsyncScene = true;
			TraceParams.bReturnPhysicalMaterial = true;
			FHitResult Hit(ForceInit);
			GetWorld()->LineTraceSingle(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, TraceParams);

			if(Hit.bBlockingHit)
			{
				if( (Hit.Location-TraceStart).Size() < MinDistance)
				{
					HitLocation = Hit.Location;
					HitNormal = Hit.Normal;
					MinDistance = (Hit.Location-TraceStart).Size();
				}
			}
		}
		if(HitLocation != FVector::ZeroVector)
		{
			DrawDebugSphere(GetWorld(),HitLocation,20,20,FColor::Yellow,true,5);

			if(Role < ROLE_Authority)
			{
				ServerAddForce(HitNormal * WalljumpHorizontalStrenght + FVector::UpVector * WalljumpUpwardsStrenght);		
			}
			LaunchCharacter(HitNormal * WalljumpHorizontalStrenght + FVector::UpVector * WalljumpUpwardsStrenght,false,true);
		}

	}
}

void AJetpackCharacter::ServerAddForce_Implementation(FVector force)
{
	LaunchCharacter(force,false,true);
}

void AJetpackCharacter::StopJetpack()	
{
	
	if (Role < ROLE_Authority) // in a client
	{
		ServerSetJetpack(false); // call server to change variable
	}
	bIsUsingJetpack = false;

}

void AJetpackCharacter::StartJetpack()	
{
	if(bIsJetpackEnabled) // check for if it is enabled, so you cant start flying again while the jetpack disabled due to the player depleting it
	{
		if (Role < ROLE_Authority) // we are in a client
		{
			ServerSetJetpack(true); // call server to change the variable
		}
		bIsUsingJetpack = true;
	}
}
void AJetpackCharacter::ServerSetJetpack_Implementation(bool bNewJetpack)
{
	// This is the server version, so the bIsUsingJetpack variable is replicated properly across the network
	bIsUsingJetpack = bNewJetpack;
}

void AJetpackCharacter::DisableJetpack()
{
	// Debug Message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("JetpackDisabled"));
	bIsJetpackEnabled = false;

	GetWorldTimerManager().SetTimer(this, &AJetpackCharacter::EnableJetpack, JetpackRecovery, false);
}
void AJetpackCharacter::EnableJetpack()
{
	// Debug Message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("JetpackEnabled"));
	bIsJetpackEnabled = true;
}


bool AJetpackCharacter::CanJump() const
{
	
	return !bIsCrouched && CharacterMovement && CharacterMovement->IsMovingOnGround() && CharacterMovement->CanEverJump() && !CharacterMovement->bWantsToCrouch;
}
bool AJetpackCharacter::ServerSetJetpack_Validate(bool bNewJetpack)
{
	return true;
}

bool AJetpackCharacter::ServerAddForce_Validate(FVector force)
{
	return true;
}