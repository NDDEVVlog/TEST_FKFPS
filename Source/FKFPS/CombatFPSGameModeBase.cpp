// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFPSGameModeBase.h"
#include "TeamClass.h"
#include "TeamData.h"
#include "Kismet/GameplayStatics.h"

void ACombatFPSGameModeBase::CountDown() {
	if (Seconds != 0) {
		Seconds--;

	}
	else {
		if (Minutes == 0) {

		}else{
			Minutes--;
			Seconds = 59;
		}
	}
}

void ACombatFPSGameModeBase::ActorDied(AActor* actorDied) {
	ATeamClass* Team = Cast<ATeamClass>(actorDied);
	if (Team == nullptr) return;
	
	switch (Team->Team)
	{
	case EDataTeam::Terrorist: {
		--TeamMember[0];
		CheckWinCondition(EDataTeam::Terrorist, TeamMember[0]);
		break;
	}
	case EDataTeam::CounterTerrorist: {
		--TeamMember[1];
		CheckWinCondition(EDataTeam::CounterTerrorist, TeamMember[1]);

		break;
	}
	default:
		break;
	}
}

void ACombatFPSGameModeBase::CheckWinCondition(EDataTeam Data, int32 MemberLeft) {

	FName RowName = FName(UEnum::GetDisplayValueAsText(Data).ToString());
	if (MemberLeft <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("%s won the game"), *RowName.ToString() );
	}
}

void ACombatFPSGameModeBase::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&ACombatFPSGameModeBase::CountDown,1.f, true,0.0);
	TArray<AActor*> Pawn;

	UGameplayStatics::GetAllActorsOfClass(this, ATeamClass::StaticClass(),Pawn);
	for (AActor* AssignedPawn : Pawn) {
		ATeamClass* TeamClassMember = Cast<ATeamClass>(AssignedPawn);
		if (!TeamClassMember) continue;
		switch (TeamClassMember->Team)
		{
		case EDataTeam::Terrorist: {
			TeamMember[0]++;
			break;
		}
		case EDataTeam::CounterTerrorist: {
			TeamMember[1]++;

			break;
		}
		default:
			break;
		}
	}
}
