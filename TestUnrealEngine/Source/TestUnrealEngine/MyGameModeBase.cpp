// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
//#include "MyPawn.h"
#include "MyCharacter.h"

//�⺻������
AMyGameModeBase::AMyGameModeBase()
{
	//DefaultPawnClass = AMyCharacter::StaticClass();

	//FClassFinder������(ã��Ŭ����)
	static ConstructorHelpers::FClassFinder<ACharacter>
		BP_Char(TEXT("Blueprint'/Game/BluePrints/BP_MyCharater.BP_MyCharater_C'"));

	if (BP_Char.Succeeded())
		DefaultPawnClass = BP_Char.Class;


}
