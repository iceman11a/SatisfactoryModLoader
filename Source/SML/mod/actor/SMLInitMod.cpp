#include "SMLInitMod.h"
#include "FGSchematicManager.h"
#include "SML/util/Logging.h"
#include "FGResearchManager.h"

void ASMLInitMod::Init_Implementation() {
}

void ASMLInitMod::PostInit_Implementation() {
}

void ASMLInitMod::PreInit_Implementation() {
}

void ASMLInitMod::PreLoadModContent() {
	//Register subsystem holders
	for (const TSubclassOf<UModSubsystemHolder> SubsystemHolder : mModSubsystems) {
		FSubsystemInfoHolder::RegisterSubsystemHolder(SubsystemHolder);
	}
}

void ASMLInitMod::LoadModContent() {
	AFGSchematicManager* schematicManager = AFGSchematicManager::Get(this);
	//No need to register AvailableSchematics on client side, they are replicated
	if (schematicManager->HasAuthority()) {
		for (TSubclassOf<UFGSchematic> schematic : mSchematics) {
			TArray<TSubclassOf<UFGSchematic>> availableSchematics;
			schematicManager->GetAvailableSchematics(availableSchematics);
			SML::Logging::debug("Loading schematic ", *UFGSchematic::GetSchematicDisplayName(schematic).ToString(), " of mod ", *this->GetClass()->GetPathName());
			if (!schematicManager->IsSchematicPurchased(schematic) && !availableSchematics.Contains(schematic)) {
				SML::Logging::debug("Adding schematic ", *UFGSchematic::GetSchematicDisplayName(schematic).ToString());
				schematicManager->AddAvailableSchematic(schematic);
			}
		}
	}
	//Add research trees
	AFGResearchManager* ResearchManager = AFGResearchManager::Get(this);
	for (TSubclassOf<UFGResearchTree> ResearchTree : mResearchTrees) {
		SML::Logging::debug("Loading research tree ", *UFGResearchTree::GetDisplayName(ResearchTree).ToString(), " of mod ", *this->GetClass()->GetPathName());
		if (!ResearchManager->mAllResearchTrees.Contains(ResearchTree)) {
			SML::Logging::debug("Adding research tree ", *UFGResearchTree::GetDisplayName(ResearchTree).ToString());
			ResearchManager->mAllResearchTrees.Add(ResearchTree);
		}
	}
	//Update unlocked trees because new research trees might have been added
	ResearchManager->UpdateUnlockedResearchTrees();
	AChatCommandSubsystem* ChatCommandSubsystem = AChatCommandSubsystem::Get(this);
	if (ChatCommandSubsystem != nullptr && ChatCommandSubsystem->HasAuthority()) {
		//Register chat commands on server side only
		for (const TSubclassOf<AChatCommandInstance>& RegistrarEntry : mChatCommands) {
			SML::Logging::info(TEXT("Registering chat command "), *RegistrarEntry->GetPathName());
			ChatCommandSubsystem->RegisterCommand(RegistrarEntry);
		}
	}
}

void ASMLInitMod::PlayerJoined_Implementation(AFGPlayerController* Player) {
}
