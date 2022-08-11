// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
//#include "MyPawn.h"
#include "MyCharacter.h"

//기본생성자
AMyGameModeBase::AMyGameModeBase()
{
	//DefaultPawnClass = AMyCharacter::StaticClass();

	//FClassFinder생성자(찾을클래스)
	static ConstructorHelpers::FClassFinder<ACharacter>
		BP_Char(TEXT("Blueprint'/Game/BluePrints/BP_MyCharacter.BP_MyCharacter_C'"));

	if (BP_Char.Succeeded())
		DefaultPawnClass = BP_Char.Class;


}
