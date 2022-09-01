// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamClass.h"
#include "Engine/World.h"


void ATeamClass::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent) {
	Super::PostEditChangeProperty(PropertyChangeEvent);
	if (Team == EDataTeam::None) return;

	if (TeamDataTable) {
		const FName RowName = FName(UEnum::GetDisplayValueAsText(Team).ToString());
		FTeamInforTable* Row = TeamDataTable->FindRow<FTeamInforTable>(RowName, "Team Details");

		if (Row) {
			TeamName = Row->TeamName;
			Material = Row->Material;
			GetMesh()->SetMaterial(0,Material);
			UE_LOG(LogTemp, Warning, TEXT("Changed"));
		}
	}

}