// Fill out your copyright notice in the Description page of Project Settings.
#include "..\Public\BaseVillager.h"
#include "GameFramework/Pawn.h"
#include "PlayerVillager.h"
#include "TimeManager.h"
#include <IPlatformFilePak.h>
#include "GameFramework/Controller.h"

// Sets default values
ABaseVillager::ABaseVillager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseVillager::BeginPlay()
{
	Super::BeginPlay();
	WorldManager = static_cast<ATimeManager*>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass()));
	NPCController = GetController();
	NPCAIController = Cast<AAIController>(NPCController);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Responses = Cast<UDialogBank>(GetDefaultSubobjectByName("Dialog"));
	if (Career != nullptr)
	{
		JobOvertimeEnd = Career->EndTime;
	}
	LoadInDialog();
	LoadInGift();
	LoadInPersonality();
}

void ABaseVillager::LoadInPersonality()
{
	if (UsePersonalityFile && Responses != nullptr)
	{
		int PersonalityIndex = 100;
		TArray<FString>  PersonalityArray;
	
		FString FinalPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir().Append(PersonalityFilePath));
		const TCHAR* CharFilePath = *FinalPath;

		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(PersonalityArray, CharFilePath);
			for (int i = 0; i < PersonalityArray.Num(); i++)
			{
				if (!FindPersonality(PersonalityArray[i], PersonalityIndex) && PersonalityIndex < 100)
				{
					if (PersonalityIndex < FloatVariables.Num())
					{
						*FloatVariables[PersonalityIndex] = FCString::Atof(*PersonalityArray[i]);
					}
					else if (PersonalityIndex < FloatVariables.Num() + IntVariables.Num())
					{
						*IntVariables[PersonalityIndex - FloatVariables.Num()] = FCString::Atoi(*PersonalityArray[i]);
					}
					else if (PersonalityIndex < FloatVariables.Num() + IntVariables.Num() + MoodVariables.Num())
					{
						for (int j = static_cast<int>(UMood::Happy); j < 5; j++)
						{
							FString CurrentMood = UEnum::GetDisplayValueAsText(static_cast<UMood>(j)).ToString();
							if (PersonalityArray[i].Equals(CurrentMood))
							{
								*MoodVariables[PersonalityIndex - FloatVariables.Num() - IntVariables.Num()] = static_cast<UMood>(j);
							}
						}
					}
					else if (PersonalityIndex < FloatVariables.Num() + IntVariables.Num() + MoodVariables.Num() + TimestampVariables.Num())
					{
						auto Index = PersonalityArray[i].Find(" ");
						if (Index < PersonalityArray[i].Len())
						{
							(TimestampVariables[PersonalityIndex - FloatVariables.Num() - IntVariables.Num() - MoodVariables.Num()])->Hour = FCString::Atoi(*PersonalityArray[i].Left(Index));
							(TimestampVariables[PersonalityIndex - FloatVariables.Num() - IntVariables.Num() - MoodVariables.Num()])->Minute = FCString::Atoi(*PersonalityArray[i].Right(PersonalityArray[i].Len() - Index));
						}
					}
				}
			}
		}
	}
}


bool ABaseVillager::FindPersonality(FString& StringValue, int& PersonalityIndex)
{
	if (StringValue.Equals("Energy", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 0;
		return true;
	}
	if (StringValue.Equals("PlayerAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 1;
		return true;
	}
	if (StringValue.Equals("Happiness", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 2;
		return true;
	}
	if (StringValue.Equals("Anger", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 3;
		return true;
	}
	if (StringValue.Equals("Fear", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 4;
		return true;
	}
	if (StringValue.Equals("Sadness", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 5;
		return true;
	}
	if (StringValue.Equals("Excitement", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 6;
		return true;
	}
	if (StringValue.Equals("PositiveMoodPercent", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 7;
		return true;
	}
	if (StringValue.Equals("NegativeMoodPercent", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 8;
		return true;
	}
	if (StringValue.Equals("HurtAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 9;
		return true;
	}
	if (StringValue.Equals("LikedGiftAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 10;
		return true;
	}
	if (StringValue.Equals("BaseGiftAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 11;
		return true;
	}
	if (StringValue.Equals("DislikedGiftAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 12;
		return true;
	}
	if (StringValue.Equals("TalkAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 13;
		return true;
	}
	if (StringValue.Equals("HobbyAffinity", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 14;
		return true;
	}
	if (StringValue.Equals("MoodGain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 15;
		return true;
	}
	if (StringValue.Equals("MoodDrain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 16;
		return true;
	}
	if (StringValue.Equals("RelaxEnergyGain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 17;
		return true;
	}
	if (StringValue.Equals("UnconsciousEnergyGain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 18;
		return true;
	}
	if (StringValue.Equals("SleepEnergyGain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 19;
		return true;
	}
	if (StringValue.Equals("JobEnergyDrain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 20;
		return true;
	}
	if (StringValue.Equals("BaseEnergyDrain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 21;
		return true;
	}
	if (StringValue.Equals("HobbyEnergyDrain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 22;
		return true;
	}
	if (StringValue.Equals("NegativeMoodEnergyDrain", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 23;
		return true;
	}
	if (StringValue.Equals("MeanderPercent", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 24;
		return true;
	}
	if (StringValue.Equals("HobbyPercent", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 25;
		return true;
	}
	if (StringValue.Equals("ConversePercent", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 26;
		return true;
	}
	if (StringValue.Equals("AffinityLevelMin", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 27;
		return true;
	}
	if (StringValue.Equals("AffinityLevelMax", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 28;
		return true;
	}
	if (StringValue.Equals("EnergyLevelMin", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 29;
		return true;
	}
	if (StringValue.Equals("EnergyLevelMax", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = 30;
		return true;
	}
	if (StringValue.Equals("PlayerHits", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num();
		return true;
	}
	if (StringValue.Equals("ConverseMinExecutions", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num()+1;
		return true;
	}
	if (StringValue.Equals("ConverseMaxExecutions", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num()+2;
		return true;
	}
	if (StringValue.Equals("PositivePreference", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num() + IntVariables.Num();
		return true;
	}
	if (StringValue.Equals("NegativePreference", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num() + IntVariables.Num()+1;
		return true;
	}
	if (StringValue.Equals("ConverseMinTime", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num() + IntVariables.Num() + MoodVariables.Num();
		return true;
	}
	if (StringValue.Equals("ConverseMaxTime", ESearchCase::IgnoreCase))
	{
		PersonalityIndex = FloatVariables.Num() + IntVariables.Num() + MoodVariables.Num()+1;
		return true;
	}
	return false;
}

void ABaseVillager::LoadInGift()
{
	if (UseGiftFile && Responses != nullptr)
	{
		int GiftIndex = 2;
		TArray<FString>  GiftArray;

		FString FinalPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir().Append(GiftFilePath));
		const TCHAR* CharFilePath = *FinalPath;
		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(GiftArray, CharFilePath);
			for (int i = 0; i < GiftArray.Num(); i++)
			{
				if (!FindGift(GiftArray[i], GiftIndex) && GiftIndex < 2)
				{
					Gifts[GiftIndex]->Add(GiftArray[i]);
				}
			}
		}
	}
}

bool ABaseVillager::FindGift(FString& StringValue, int& GiftIndex)
{
	if (StringValue.Equals("LikedGifts", ESearchCase::IgnoreCase))
	{
		GiftIndex = 0;
		return true;
	}
	if (StringValue.Equals("DislikedGifts", ESearchCase::IgnoreCase))
	{
		GiftIndex = 1;
		return true;
	}
	return false;
}

void ABaseVillager::LoadInDialog()
{
	if (UseDialogueFile && Responses != nullptr)
	{
		TArray<FString>  Dialogue;

		FString FinalPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectContentDir().Append(DialogueFilePath));
		const TCHAR* CharFilePath = *FinalPath;

		IPlatformFile* PlatformFile = FPlatformFileManager::Get().FindPlatformFile(CharFilePath);

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(CharFilePath))
		{
			FFileHelper::LoadFileToStringArray(Dialogue, CharFilePath);
			for (int i = 0; i < Dialogue.Num(); i++)
			{
				if(!FindSubBank(Dialogue[i]) && DialogBanks.Num() >= 1)
				{
					DialogBanks[DialogBanks.Num()-1]->Add(Dialogue[i]);
				}
			}
		}
	}
}

bool ABaseVillager::FindSubBank(FString& StringValue)
{
	if (StringValue.Equals("HighAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HighAffinity);
		return true;
	}
	if (StringValue.Equals("MidAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MidAffinity);
		return true;
	}
	if (StringValue.Equals("LowAffinity", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LowAffinity);
		return true;
	}
	if (StringValue.Equals("HighEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HighEnergy);
		return true;
	}
	if (StringValue.Equals("MidEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MidEnergy);
		return true;
	}
	if (StringValue.Equals("LowEnergy", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LowEnergy);
		return true;
	}
	if (StringValue.Equals("HappyMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HappyMood);
		return true;
	}
	if (StringValue.Equals("SadMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->SadMood);
		return true;
	}
	if (StringValue.Equals("AngryMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->AngryMood);
		return true;
	}
	if (StringValue.Equals("ExcitedMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->ExcitedMood);
		return true;
	}
	if (StringValue.Equals("ScaredMood", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->ScaredMood);
		return true;
	}
	if (StringValue.Equals("BusyState", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->BusyState);
		return true;
	}
	if (StringValue.Equals("HurtReactions", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HurtReactions);
		return true;
	}
	if (StringValue.Equals("LikedGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->LikedGifts);
		return true;
	}
	if (StringValue.Equals("MiddleGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->MiddleGifts);
		return true;
	}
	if (StringValue.Equals("DislikedGifts", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->DislikedGifts);
		return true;
	}
	if (StringValue.Equals("Coworker", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->Coworker);
		return true;
	}
	if (StringValue.Equals("HurtCoworker", ESearchCase::IgnoreCase))
	{
		DialogBanks.Add(&Responses->HurtCoworker);
		return true;
	}
	return false;
}

// Called every frame
void ABaseVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseVillager::UpdateMood()
{
	for (int i = 0; i < MoodValues.Num(); i++)
	{
		if (*MoodValues[i] < 0)
		{
			*MoodValues[i] = 0;
		}
		else if (*MoodValues[i] > 100)
		{
			*MoodValues[i] = 100;
		}
	}

	int j = 0;
	float LargestValue = 0;
	for (int i = 0; i < MoodValues.Num(); i++)
	{
		if (LargestValue < *MoodValues[i])
		{
			LargestValue = *MoodValues[i];
			j = i;
		}
	}
	Mood = static_cast<UMood>(j);
}

void ABaseVillager::UpdateAffinity()
{
	if (PlayerAffinity < 0)
	{
		PlayerAffinity = 0;
	}
	else if (PlayerAffinity > 100)
	{
		PlayerAffinity = 100;
	}
}

void ABaseVillager::UpdateEnergy(bool UpdateLevels)
{
	if (UpdateLevels)
	{
		Energy += *EnergyChange[static_cast<uint8>(State)];
		if (State != UState::Relaxing && State != UState::Unconscious && State!= UState::Asleep && (Mood == UMood::Angry || Mood == UMood::Sad || Mood == UMood::Scared))
		{
			Energy -= NegativeMoodEnergyDrain;
		}
	}

	if (Energy < 0)
	{
		Energy = 0;
	}
	else if (Energy > 100)
	{
		Energy = 100;
	}
}

void ABaseVillager::NegativeMoodHit(float MoodChange)
{
	auto NewValue = FMath::RandRange(0, 100);
	if (NewValue <= NegativeMoodPercent)
	{
		*MoodValues[static_cast<uint8>(NegativePreference)] += MoodChange;
	}
	//Negative moods use the integer values 1 2 and 3.
	else
	{
		int RandomMood = FMath::RandRange(1, 3);
		while (RandomMood == static_cast<uint8>(NegativePreference))
		{
			RandomMood = FMath::RandRange(1, 3);
		}
		*MoodValues[RandomMood] += MoodChange;
	}
	UpdateMood();
}

void ABaseVillager::PositiveMoodHit(float MoodChange)
{
	auto NewValue = FMath::RandRange(0, 100);
	if (NewValue <= PositiveMoodPercent)
	{
		*MoodValues[static_cast<uint8>(PositivePreference)]+=MoodChange;
	}
	else
	{
		if (PositivePreference == UMood::Happy)
		{
			Excitement += MoodChange;
		}
		else
		{
			Happiness += MoodChange;
		}
	}
	UpdateMood();
}

void ABaseVillager::ConverseTransition()
{
	if (ConversationPartner != nullptr && CurrentIdleBehavior == 2)
	{
		if (IsOverlappingActor(ConversationPartner))
		{
			if (static_cast<APlayerVillager*>(ConversationPartner) != nullptr && !static_cast<APlayerVillager*>(ConversationPartner)->ConversationTarget && !static_cast<APlayerVillager*>(ConversationPartner)->Interacting && static_cast<APlayerVillager*>(ConversationPartner)->InteractedNPC == nullptr)
			{
				Talk();
				static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget = true;
				PlayerConversationInitiated = true;
			}
			else if(static_cast<ABaseVillager*>(ConversationPartner) != nullptr && CanTalk() && static_cast<ABaseVillager*>(ConversationPartner)->CanTalk())
			{
				auto ConverseOptions = IdleOptions[CurrentIdleBehavior];

				CurrentTime.GenerateRandomTime(ConverseOptions->MinTime, ConverseOptions->MaxTime, ConverseOptions->StartTime, ConverseOptions->LastTime);

				if (static_cast<ABaseVillager*>(ConversationPartner) != nullptr)
				{
					static_cast<ABaseVillager*>(ConversationPartner)->IdleOptions[2]->StartTime = ConverseOptions->StartTime;
					static_cast<ABaseVillager*>(ConversationPartner)->IdleOptions[2]->LastTime = ConverseOptions->LastTime;

					Talk();
					static_cast<ABaseVillager*>(ConversationPartner)->Talk();
				}
			}
		}
	}
}

void ABaseVillager::HobbyTransition()
{
	if (TargetHobby >= Hobbies.Num())
	{
		return;
	}
	auto& InvokedHobby = Hobbies[TargetHobby];
	if (Hobbies[TargetHobby] != nullptr && Hobbies[TargetHobby]->AssociatedObject != nullptr && IsOverlappingActor(Hobbies[TargetHobby]->AssociatedObject))
	{
		Hobbies[TargetHobby]->AssociatedObject->AddHobbyPerformer(this);
		TravelingToHobby = false;
		CurrentHobby = InvokedHobby->Name;

		CurrentTime.GenerateRandomTime(InvokedHobby->MinHobbyTime, InvokedHobby->MaxHobbyTime, InvokedHobby->LastHobbyStartTime, InvokedHobby->ActiveHobbyEndTime);
		State = UState::Hobby;
	}
}

void ABaseVillager::Talk()
{
	PreviousState = State;
	State = UState::Talking;
	NPCAIController->StopMovement();

	if (ConversationPartner == nullptr)
	{
		SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	}
	else
	{
		SetActorRotation((ConversationPartner->GetActorLocation() - GetActorLocation()).Rotation());
	}
}

bool ABaseVillager::RecoverEnergy()
{
	if (Energy < 10)
	{
		if (State == UState::Hobby && TargetHobby < Hobbies.Num())
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
		}
		NPCAIController->StopMovement();
		State = UState::Unconscious;
		return true;
	}
	else if (Energy < EnergyLevels[0] && !AtWork && !Commuting && !GoingToBed)
	{
		if (State == UState::Hobby && TargetHobby < Hobbies.Num())
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
		}

		NPCAIController->StopMovement();
		State = UState::Relaxing;
		return true;
	}
	else if (Energy >= EnergyLevels[1] && (State == UState::Relaxing || State == UState::Unconscious))
	{
		if (AtWork)
		{
			State = UState::Working;
		}
		else
		{
			State = UState::Idle;
		}
		return false;
	}
	else if (State == UState::Relaxing || State == UState::Asleep || State == UState::Unconscious)
	{
		return true;
	}
	return false;
}

void ABaseVillager::RelaxingMoodImprovement()
{
	if (static_cast<uint8>(Mood) != 0)
	{
		*MoodValues[static_cast<uint8>(Mood)] -= MoodDrain/10;
	}
	else
	{
		NegativeMoodHit(-MoodDrain / 10);
	}
	PositiveMoodHit(MoodGain/10);
	UpdateMood();
}

void ABaseVillager::WorkingBehaviors()
{
	TravelingToHobby = false;
	CurrentIdleBehavior = 4;
}

void ABaseVillager::WalkingBehaviors()
{
	if (GetVelocity() == FVector{ 0, 0, 0 })
	{
		State = UState::Idle;
	}

	if (GoingToBed && Bed != nullptr)
	{
		NPCAIController->MoveToLocation(Bed->GetActorLocation());
		if (IsOverlappingActor(Bed))
		{
			State = UState::Asleep;
		}
	}
	else if (CurrentIdleBehavior == 1)
	{
		HobbyTransition();
	}
	else if (CurrentIdleBehavior == 2)
	{
		ConverseTransition();
	}
	else if (Commuting)
	{
		TravelingToHobby = false;
		CurrentIdleBehavior = 4;
		return;
	}
}

void ABaseVillager::TalkingBehaviors()
{
	if (CurrentIdleBehavior == 2)
	{
		if (ConversationPartner != nullptr)
		{
			SetActorRotation((ConversationPartner->GetActorLocation() - GetActorLocation()).Rotation());
			if (PlayerConversationInitiated && static_cast<APlayerVillager*>(ConversationPartner) != nullptr)
			{
				if (!CanTalk() || IdleOptions[CurrentIdleBehavior]->CurrentExecutions <= 0)
				{
					EndInitiatedDialog();
				}
			}
			else if(static_cast<ABaseVillager*>(ConversationPartner) != nullptr)
			{
				if (IdleOptions[CurrentIdleBehavior]->LastTime < CurrentTime || !CanTalk())
				{
					static_cast<ABaseVillager*>(ConversationPartner)->EndInitiatedDialog();
					EndInitiatedDialog();
				}
			}
		}
		else
		{
			EndDialog();
		}
	}
	else
	{
		SetActorRotation((PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation());
	}
}
bool ABaseVillager::CanTalk()
{
	int BadMood = static_cast<uint8>(Mood);
	if (AtWork || Commuting || (!(EnergyLevels.Num() <= 0) && Energy <= EnergyLevels[0]) || (BadMood != 0 && BadMood != 4) || GoingToBed || CurrentIdleBehavior == 1)
	{
		return false;
	}
	return true;
}

bool ABaseVillager::CanStartConversation()
{
	if (CanTalk() && !PlayerConversationInitiated && State != UState::Talking && State != UState::Relaxing && State != UState::Unconscious && State != UState::Asleep)
	{
		return true;
	}
	return false;
}

void ABaseVillager::IdleBehaviors()
{
	CurrentIdleBehavior = 4;
	PlayerConversationInitiated = false;
	ConversationPartner = nullptr;
	TargetHobby = Hobbies.Num();

	if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
	{
		int randomChance = FMath::RandRange(0, 100);

		if (CurrentTime.Minute % 10 == 0)
		{
			if (randomChance <= IdleOptions[0]->Percent)
			{
				CurrentIdleBehavior = 0;
				IdleLoafing();
			}
			else if (randomChance <= IdleOptions[0]->Percent + IdleOptions[1]->Percent)
			{
				CurrentIdleBehavior = 1;
				if (AttemptHobbySelection())
				{
					State = UState::Walking;
					return;
				}
			}
			else
			{
				if (CanTalk())
				{
					CurrentIdleBehavior = 2;
					InitiateConversation();
				}
				else
				{
					CurrentIdleBehavior = 0;
					IdleLoafing();
				}
			}
		}

		if (GetVelocity() != FVector{ 0, 0, 0 })
		{
			State = UState::Walking;
		}
	}
	else if (Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name)
	{
		SetActorRotation(((PlayerPawn->GetActorLocation() - GetActorLocation()) * FVector(1, 1, 0)).Rotation());
	}
}

void ABaseVillager::DecrementConversations()
{
	IdleOptions[2]->CurrentExecutions -= 1;
}

void ABaseVillager::InitiateConversation()
{
	ConversationPartner = ChoosePartner();
	if (ConversationPartner != nullptr)
	{
		NPCAIController->MoveToActor(ConversationPartner, 60, true);
		State = UState::Walking;
	}
}

AActor* ABaseVillager::ChoosePartner()
{
	auto ChosenTarget = FMath::RandRange(0, 100);
	
	if (PlayerAffinity > AffinityLevels[0] && ChosenTarget < PlayerAffinity && !static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget && !static_cast<APlayerVillager*>(PlayerPawn)->Interacting)
	{
		auto NumberOfTimes = FMath::RandRange(IdleOptions[2]->MinExecutions, IdleOptions[2]->MaxExecutions);
		IdleOptions[2]->CurrentExecutions = NumberOfTimes;
		return PlayerPawn;
	}
	else
	{
		int VillagerNumber = WorldManager->Villagers.Num();
		for (int i = 0; i < 6; i++)
		{
			int RandomSelection = FMath::RandRange(0, VillagerNumber-1);
			auto ChosenVillager = static_cast<ABaseVillager*>(WorldManager->Villagers[RandomSelection]);

			if (ChosenVillager != nullptr && this != ChosenVillager  && CanStartConversation() && ChosenVillager->CanStartConversation() && ChosenVillager->CurrentIdleBehavior != 2)
			{
				auto VillagerCasted = static_cast<ABaseVillager*>(ChosenVillager);
		
				VillagerCasted->ConversationPartner = this;
				VillagerCasted->CurrentIdleBehavior = 2;
				VillagerCasted->NPCAIController->MoveToActor(this, 60, true);
				VillagerCasted->State = UState::Walking;

				return ChosenVillager;
			}
		}
	}
	return nullptr;
}

void ABaseVillager::HobbyBehaviors()
{
	if (TargetHobby < Hobbies.Num())
	{
		auto& InvokedHobby = Hobbies[TargetHobby];
		if (CurrentTime.BetweenTwoTimes(InvokedHobby->LastHobbyStartTime, InvokedHobby->ActiveHobbyEndTime))
		{
			PositiveMoodHit(MoodGain / 10);
			NegativeMoodHit(-MoodDrain / 10);
			if (InvokedHobby->AssociatedObject->PlayerPerforming)
			{
				PlayerAffinity += HobbyAffinity;
			}
		}
		else
		{
			Hobbies[TargetHobby]->AssociatedObject->RemoveHobbyPerformer(this);
			State = UState::Idle;
		}
	}
}
void ABaseVillager::RelaxingBehaviors()
{
	if (TargetHobby != Hobbies.Num())
	{
		Hobbies[TargetHobby]->ActiveHobbyEndTime = CurrentTime;
		TargetHobby = Hobbies.Num();
	}
}
void ABaseVillager::UnconsciousBehaviors()
{

}
void ABaseVillager::AsleepBehaviors()
{
	if (!CheckToSleep())
	{
		State = UState::Idle;
	}
}

void ABaseVillager::EndInitiatedDialog()
{
	CurrentIdleBehavior = 4;
	TargetHobby = Hobbies.Num();

	if(static_cast<APlayerVillager*>(ConversationPartner) && PlayerConversationInitiated)
	{
		static_cast<APlayerVillager*>(PlayerPawn)->InteractedNPC = nullptr;
		static_cast<APlayerVillager*>(PlayerPawn)->ConversationTarget = false;
		static_cast<APlayerVillager*>(PlayerPawn)->ConversationCleanup = true;
	}
	else
	{
		ConversationPartner = nullptr;
	}

	if (!Commuting && !AtWork && State != UState::Relaxing)
	{
		State = UState::Idle;
	}
	else if (Commuting)
	{
		State = UState::Walking;
	}
	else if (AtWork && Career != nullptr)
	{
		State = UState::Working;
		SetActorRotation(((Career->Workstation->GetActorLocation() - GetActorLocation()) * FVector(1, 1, 0)).Rotation());
	}
	else if (Energy < EnergyLevels[0])
	{
		State = UState::Relaxing;
	}
	return;
}

void ABaseVillager::UpdateStatus()
{
	if (GetWorldTimerManager().IsTimerActive(IdleTimer) && State != UState::Idle)
	{
		GetWorldTimerManager().ClearTimer(IdleTimer);
	}
	CheckToSleep();
	if (GoingToBed && Bed != nullptr)
	{
		TravelingToHobby = false;
		if (State != UState::Asleep && State != UState::Unconscious && State != UState::Talking)
		{
			NPCAIController->MoveToLocation(Bed->GetActorLocation());
			State = UState::Walking;
			if (IsOverlappingActor(Bed))
			{
				State = UState::Asleep;
			}
		}
	}
	if (State != UState::Hobby && State != UState::Talking && CurrentHobby != "")
	{
		if (!TravelingToHobby || Commuting || AtWork)
		{
			TargetHobby = Hobbies.Num();
			CurrentHobby = "";
		}
		else
		{
			State = UState::Walking;
		}
	}
	if (State != UState::Talking && RecoverEnergy())
	{
		RelaxingMoodImprovement();
	}
	(this->*(Behaviors[static_cast<uint8>(State)]))();
	UpdateEnergy(true);
}

void ABaseVillager::IdleLoafing()
{
	if (!(Cast<APlayerVillager>(PlayerPawn) && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC != nullptr && Cast<APlayerVillager>(PlayerPawn)->InteractedNPC->Name == Name))
	{
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation Point;
		bool success = NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), IdleRadius, Point);
		if (success)
		{
			NPCAIController->MoveToLocation(Point);
			State = UState::Walking;
		}
	}
}

bool ABaseVillager::CheckToSleep()
{
	if (Career != nullptr)
	{
		GoingToBed = CurrentTime.BetweenTwoTimes(Career->SleepTime, Career->WakeTime);
		return GoingToBed;
	}
	return false;
}

bool ABaseVillager::AttemptHobbySelection()
{
	if (Hobbies.Num() > 0)
	{
		auto HobbyIndex = FMath::RandRange(0, Hobbies.Num() - 1);
		if (Hobbies[HobbyIndex] != nullptr && Hobbies[HobbyIndex]->AssociatedObject != nullptr && Energy > 40)
		{
			if (Hobbies[HobbyIndex]->CanInvoke(CurrentTime))
			{
				TravelingToHobby = true;
				TargetHobby = HobbyIndex;
				NPCAIController->MoveToLocation(Hobbies[HobbyIndex]->AssociatedObject->GetActorLocation());
				State = UState::Walking;
				return true;
			}
		}
	}
	return false;
}

void ABaseVillager::EndDialog()
{
	if (State != UState::Unconscious && State != UState::Asleep)
	{
		State = PreviousState;

		if (State == UState::Working)
		{
			SetActorRotation(((Career->Workstation->GetActorLocation() - GetActorLocation()) * FVector(1, 1, 0)).Rotation());
		}
	}
}

void ABaseVillager::CheckOnJob(const UWeekday& CurrentDay)
{
	if (!DoneWork && Career != nullptr)
	{
		if (State != UState::Talking || CurrentIdleBehavior < 3)
		{
			if (CurrentTime.BetweenTwoTimes(Career->CommuteTime, JobOvertimeEnd) && !Commuting && !AtWork && Career->Days.Contains(CurrentDay))
			{
				Commuting = true;
				NPCAIController->MoveToActor(Career->Workstation, 0, true);
				if (State == UState::Talking)
				{
					if (!PlayerConversationInitiated && static_cast<ABaseVillager*>(ConversationPartner) != nullptr)
					{
						static_cast<ABaseVillager*>(ConversationPartner)->EndInitiatedDialog();
					}
					EndInitiatedDialog();
				}
				State = UState::Walking;
			}
			else if (!CurrentTime.BetweenTwoTimes(Career->CommuteTime, JobOvertimeEnd) && AtWork)
			{
				AtWork = false;
				DoneWork = true;
				OvertimeMinutes = 0;
				State = UState::Idle;
				JobOvertimeEnd = Career->EndTime;
			}
			else if (Commuting && IsOverlappingActor(Career->Workstation))
			{
				AtWork = true;
				Commuting = false;
				State = UState::Working;
				NPCAIController->StopMovement();
			}
			else if (Commuting && State != UState::Talking && GetVelocity().GetAbsMax() < 1)
			{
				NPCAIController->MoveToActor(Career->Workstation, 0, true);
			}
		}
		if (CurrentTime.BetweenTwoTimes(Career->StartTime, JobOvertimeEnd) && State != UState::Working)
		{
			FTimestamp NewMinute;
			NewMinute.Hour = 0;
			NewMinute.Minute = 1;
			JobOvertimeEnd = JobOvertimeEnd + NewMinute;
		}
		if (CurrentTime.BetweenTwoTimes(Career->EndTime, JobOvertimeEnd))
		{
			NegativeMoodHit(MoodDrain / 10);
			PositiveMoodHit(-MoodGain / 10);
		}
	}
}

// Called to bind functionality to input
void ABaseVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}