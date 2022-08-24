// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFPSGameModeBase.h"
#include "TeamBlue.h"
#include "TeamRed.h"
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
	if (ATeamBlue* BlueMem = Cast<ATeamBlue>(actorDied)) {
		--BlueMembers;
		if (BlueMembers == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Red Winnnnnnnnnnn"));
		}
	}
	else if(ATeamRed* RedMem = Cast<ATeamRed>(actorDied) ){
		--RedMembers;
		if (RedMembers == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Blue Winnnnnnnnnnn"));

		}
	}
}

void ACombatFPSGameModeBase::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&ACombatFPSGameModeBase::CountDown,1.f, true,0.0);
	RedMembers = GetRedMemberNum();

	BlueMembers = GetBlueMemberNum();


}

int32 ACombatFPSGameModeBase::GetRedMemberNum() {
	TArray<AActor*> Red;
	UGameplayStatics::GetAllActorsOfClass(this, ATeamRed::StaticClass(), Red);
	return Red.Num();

}

int32 ACombatFPSGameModeBase::GetBlueMemberNum() {
	TArray<AActor*> Blue;
	UGameplayStatics::GetAllActorsOfClass(this, ATeamBlue::StaticClass(), Blue);
	return Blue.Num();


}