##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release_NonHomeFolder_Win32
ProjectName            :=RCBot2Meta
ConfigurationName      :=Release_NonHomeFolder_Win32
WorkspacePath          :=/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta
ProjectPath            :=/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta
IntermediateDirectory  :=$(VS_Configuration)/
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=mint
Date                   :=04/11/17
CodeLitePath           :=/home/mint/.codelite
LinkerName             :=/usr/bin/i686-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/i686-linux-gnu-g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./Release_NonHomeFolder/HPB_bot2o.dll
Preprocessors          :=$(PreprocessorSwitch)WIN32 $(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)_WINDOWS 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="RCBot2Meta.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../../public/mathlib $(IncludeSwitch)../../public $(IncludeSwitch)../../public/engine $(IncludeSwitch)../../public/tier0 $(IncludeSwitch)../../public/tier1 $(IncludeSwitch)../../dlls $(IncludeSwitch)../../game_shared $(IncludeSwitch)../../game/shared $(IncludeSwitch)../../game/server $(IncludeSwitch)../../public/game/server 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)tier0 $(LibrarySwitch)tier1 $(LibrarySwitch)tier2 $(LibrarySwitch)tier3 $(LibrarySwitch)mathlib $(LibrarySwitch)vstdlib 
ArLibs                 :=  "tier0.a" "tier1.a" "tier2.a" "tier3.a" "mathlib.a" "vstdlib.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)..\..\lib\public $(LibraryPathSwitch)..\..\lib\common 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/i686-linux-gnu-ar rcu
CXX      := /usr/bin/i686-linux-gnu-g++
CC       := /usr/bin/i686-linux-gnu-gcc
CXXFLAGS :=  -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/i686-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
VS_Configuration:=Release_NonHomeFolder
VS_IntDir:=$(VS_Configuration)/
VS_OutDir:=$(VS_Configuration)/
VS_Platform:=Win32
VS_ProjectDir:=/home/desktop/Dropbox/src/rcbot2/utils/RCBot2_meta/
VS_ProjectName:=RCBot2Meta
VS_SolutionDir:=/home/desktop/Dropbox/src/rcbot2/utils/RCBot2_meta/
Objects0=$(IntermediateDirectory)/bot.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_accessclient.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_buttons.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_client.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_commands.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_configfile.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_coop.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_css_bot.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_dod_bot.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_dod_mod.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/bot_events.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_fortress.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_ga.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_ga_ind.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_getprop.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_globals.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_hl1dmsrc.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_hldm_bot.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_kv.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_menu.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/bot_mods.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_mtrand.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_navmesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_perceptron.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_profile.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_profiling.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_schedule.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_som.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_squads.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_strings.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/bot_task.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_tf2_mod.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_tf2_points.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_usercmd.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_utility.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_visibles.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_waypoint.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_waypoint_locations.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_waypoint_visibility.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_weapons.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/bot_wpt_dist.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_zombie.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_sigscan.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_cvars.cpp$(ObjectSuffix) $(IntermediateDirectory)/bot_plugin_meta.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)
	@$(MakeDirCommand) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/.build-release"
	@echo rebuilt > "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/.build-release/RCBot2Meta"

MakeIntermediateDirs:
	@test -d $(VS_Configuration)/ || $(MakeDirCommand) $(VS_Configuration)/


$(IntermediateDirectory)/.d:
	@test -d $(VS_Configuration)/ || $(MakeDirCommand) $(VS_Configuration)/

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/bot.cpp$(ObjectSuffix): bot.cpp $(IntermediateDirectory)/bot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot.cpp$(DependSuffix): bot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot.cpp$(DependSuffix) -MM bot.cpp

$(IntermediateDirectory)/bot.cpp$(PreprocessSuffix): bot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot.cpp$(PreprocessSuffix) bot.cpp

$(IntermediateDirectory)/bot_accessclient.cpp$(ObjectSuffix): bot_accessclient.cpp $(IntermediateDirectory)/bot_accessclient.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_accessclient.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_accessclient.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_accessclient.cpp$(DependSuffix): bot_accessclient.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_accessclient.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_accessclient.cpp$(DependSuffix) -MM bot_accessclient.cpp

$(IntermediateDirectory)/bot_accessclient.cpp$(PreprocessSuffix): bot_accessclient.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_accessclient.cpp$(PreprocessSuffix) bot_accessclient.cpp

$(IntermediateDirectory)/bot_buttons.cpp$(ObjectSuffix): bot_buttons.cpp $(IntermediateDirectory)/bot_buttons.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_buttons.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_buttons.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_buttons.cpp$(DependSuffix): bot_buttons.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_buttons.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_buttons.cpp$(DependSuffix) -MM bot_buttons.cpp

$(IntermediateDirectory)/bot_buttons.cpp$(PreprocessSuffix): bot_buttons.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_buttons.cpp$(PreprocessSuffix) bot_buttons.cpp

$(IntermediateDirectory)/bot_client.cpp$(ObjectSuffix): bot_client.cpp $(IntermediateDirectory)/bot_client.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_client.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_client.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_client.cpp$(DependSuffix): bot_client.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_client.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_client.cpp$(DependSuffix) -MM bot_client.cpp

$(IntermediateDirectory)/bot_client.cpp$(PreprocessSuffix): bot_client.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_client.cpp$(PreprocessSuffix) bot_client.cpp

$(IntermediateDirectory)/bot_commands.cpp$(ObjectSuffix): bot_commands.cpp $(IntermediateDirectory)/bot_commands.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_commands.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_commands.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_commands.cpp$(DependSuffix): bot_commands.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_commands.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_commands.cpp$(DependSuffix) -MM bot_commands.cpp

$(IntermediateDirectory)/bot_commands.cpp$(PreprocessSuffix): bot_commands.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_commands.cpp$(PreprocessSuffix) bot_commands.cpp

$(IntermediateDirectory)/bot_configfile.cpp$(ObjectSuffix): bot_configfile.cpp $(IntermediateDirectory)/bot_configfile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_configfile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_configfile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_configfile.cpp$(DependSuffix): bot_configfile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_configfile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_configfile.cpp$(DependSuffix) -MM bot_configfile.cpp

$(IntermediateDirectory)/bot_configfile.cpp$(PreprocessSuffix): bot_configfile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_configfile.cpp$(PreprocessSuffix) bot_configfile.cpp

$(IntermediateDirectory)/bot_coop.cpp$(ObjectSuffix): bot_coop.cpp $(IntermediateDirectory)/bot_coop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_coop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_coop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_coop.cpp$(DependSuffix): bot_coop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_coop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_coop.cpp$(DependSuffix) -MM bot_coop.cpp

$(IntermediateDirectory)/bot_coop.cpp$(PreprocessSuffix): bot_coop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_coop.cpp$(PreprocessSuffix) bot_coop.cpp

$(IntermediateDirectory)/bot_css_bot.cpp$(ObjectSuffix): bot_css_bot.cpp $(IntermediateDirectory)/bot_css_bot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_css_bot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_css_bot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_css_bot.cpp$(DependSuffix): bot_css_bot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_css_bot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_css_bot.cpp$(DependSuffix) -MM bot_css_bot.cpp

$(IntermediateDirectory)/bot_css_bot.cpp$(PreprocessSuffix): bot_css_bot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_css_bot.cpp$(PreprocessSuffix) bot_css_bot.cpp

$(IntermediateDirectory)/bot_dod_bot.cpp$(ObjectSuffix): bot_dod_bot.cpp $(IntermediateDirectory)/bot_dod_bot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_dod_bot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_dod_bot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_dod_bot.cpp$(DependSuffix): bot_dod_bot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_dod_bot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_dod_bot.cpp$(DependSuffix) -MM bot_dod_bot.cpp

$(IntermediateDirectory)/bot_dod_bot.cpp$(PreprocessSuffix): bot_dod_bot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_dod_bot.cpp$(PreprocessSuffix) bot_dod_bot.cpp

$(IntermediateDirectory)/bot_dod_mod.cpp$(ObjectSuffix): bot_dod_mod.cpp $(IntermediateDirectory)/bot_dod_mod.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_dod_mod.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_dod_mod.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_dod_mod.cpp$(DependSuffix): bot_dod_mod.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_dod_mod.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_dod_mod.cpp$(DependSuffix) -MM bot_dod_mod.cpp

$(IntermediateDirectory)/bot_dod_mod.cpp$(PreprocessSuffix): bot_dod_mod.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_dod_mod.cpp$(PreprocessSuffix) bot_dod_mod.cpp

$(IntermediateDirectory)/bot_events.cpp$(ObjectSuffix): bot_events.cpp $(IntermediateDirectory)/bot_events.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_events.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_events.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_events.cpp$(DependSuffix): bot_events.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_events.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_events.cpp$(DependSuffix) -MM bot_events.cpp

$(IntermediateDirectory)/bot_events.cpp$(PreprocessSuffix): bot_events.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_events.cpp$(PreprocessSuffix) bot_events.cpp

$(IntermediateDirectory)/bot_fortress.cpp$(ObjectSuffix): bot_fortress.cpp $(IntermediateDirectory)/bot_fortress.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_fortress.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_fortress.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_fortress.cpp$(DependSuffix): bot_fortress.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_fortress.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_fortress.cpp$(DependSuffix) -MM bot_fortress.cpp

$(IntermediateDirectory)/bot_fortress.cpp$(PreprocessSuffix): bot_fortress.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_fortress.cpp$(PreprocessSuffix) bot_fortress.cpp

$(IntermediateDirectory)/bot_ga.cpp$(ObjectSuffix): bot_ga.cpp $(IntermediateDirectory)/bot_ga.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_ga.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_ga.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_ga.cpp$(DependSuffix): bot_ga.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_ga.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_ga.cpp$(DependSuffix) -MM bot_ga.cpp

$(IntermediateDirectory)/bot_ga.cpp$(PreprocessSuffix): bot_ga.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_ga.cpp$(PreprocessSuffix) bot_ga.cpp

$(IntermediateDirectory)/bot_ga_ind.cpp$(ObjectSuffix): bot_ga_ind.cpp $(IntermediateDirectory)/bot_ga_ind.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_ga_ind.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_ga_ind.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_ga_ind.cpp$(DependSuffix): bot_ga_ind.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_ga_ind.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_ga_ind.cpp$(DependSuffix) -MM bot_ga_ind.cpp

$(IntermediateDirectory)/bot_ga_ind.cpp$(PreprocessSuffix): bot_ga_ind.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_ga_ind.cpp$(PreprocessSuffix) bot_ga_ind.cpp

$(IntermediateDirectory)/bot_getprop.cpp$(ObjectSuffix): bot_getprop.cpp $(IntermediateDirectory)/bot_getprop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_getprop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_getprop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_getprop.cpp$(DependSuffix): bot_getprop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_getprop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_getprop.cpp$(DependSuffix) -MM bot_getprop.cpp

$(IntermediateDirectory)/bot_getprop.cpp$(PreprocessSuffix): bot_getprop.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_getprop.cpp$(PreprocessSuffix) bot_getprop.cpp

$(IntermediateDirectory)/bot_globals.cpp$(ObjectSuffix): bot_globals.cpp $(IntermediateDirectory)/bot_globals.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_globals.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_globals.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_globals.cpp$(DependSuffix): bot_globals.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_globals.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_globals.cpp$(DependSuffix) -MM bot_globals.cpp

$(IntermediateDirectory)/bot_globals.cpp$(PreprocessSuffix): bot_globals.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_globals.cpp$(PreprocessSuffix) bot_globals.cpp

$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(ObjectSuffix): bot_hl1dmsrc.cpp $(IntermediateDirectory)/bot_hl1dmsrc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_hl1dmsrc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(DependSuffix): bot_hl1dmsrc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(DependSuffix) -MM bot_hl1dmsrc.cpp

$(IntermediateDirectory)/bot_hl1dmsrc.cpp$(PreprocessSuffix): bot_hl1dmsrc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_hl1dmsrc.cpp$(PreprocessSuffix) bot_hl1dmsrc.cpp

$(IntermediateDirectory)/bot_hldm_bot.cpp$(ObjectSuffix): bot_hldm_bot.cpp $(IntermediateDirectory)/bot_hldm_bot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_hldm_bot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_hldm_bot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_hldm_bot.cpp$(DependSuffix): bot_hldm_bot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_hldm_bot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_hldm_bot.cpp$(DependSuffix) -MM bot_hldm_bot.cpp

$(IntermediateDirectory)/bot_hldm_bot.cpp$(PreprocessSuffix): bot_hldm_bot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_hldm_bot.cpp$(PreprocessSuffix) bot_hldm_bot.cpp

$(IntermediateDirectory)/bot_kv.cpp$(ObjectSuffix): bot_kv.cpp $(IntermediateDirectory)/bot_kv.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_kv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_kv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_kv.cpp$(DependSuffix): bot_kv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_kv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_kv.cpp$(DependSuffix) -MM bot_kv.cpp

$(IntermediateDirectory)/bot_kv.cpp$(PreprocessSuffix): bot_kv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_kv.cpp$(PreprocessSuffix) bot_kv.cpp

$(IntermediateDirectory)/bot_menu.cpp$(ObjectSuffix): bot_menu.cpp $(IntermediateDirectory)/bot_menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_menu.cpp$(DependSuffix): bot_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_menu.cpp$(DependSuffix) -MM bot_menu.cpp

$(IntermediateDirectory)/bot_menu.cpp$(PreprocessSuffix): bot_menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_menu.cpp$(PreprocessSuffix) bot_menu.cpp

$(IntermediateDirectory)/bot_mods.cpp$(ObjectSuffix): bot_mods.cpp $(IntermediateDirectory)/bot_mods.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_mods.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_mods.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_mods.cpp$(DependSuffix): bot_mods.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_mods.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_mods.cpp$(DependSuffix) -MM bot_mods.cpp

$(IntermediateDirectory)/bot_mods.cpp$(PreprocessSuffix): bot_mods.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_mods.cpp$(PreprocessSuffix) bot_mods.cpp

$(IntermediateDirectory)/bot_mtrand.cpp$(ObjectSuffix): bot_mtrand.cpp $(IntermediateDirectory)/bot_mtrand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_mtrand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_mtrand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_mtrand.cpp$(DependSuffix): bot_mtrand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_mtrand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_mtrand.cpp$(DependSuffix) -MM bot_mtrand.cpp

$(IntermediateDirectory)/bot_mtrand.cpp$(PreprocessSuffix): bot_mtrand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_mtrand.cpp$(PreprocessSuffix) bot_mtrand.cpp

$(IntermediateDirectory)/bot_navmesh.cpp$(ObjectSuffix): bot_navmesh.cpp $(IntermediateDirectory)/bot_navmesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_navmesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_navmesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_navmesh.cpp$(DependSuffix): bot_navmesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_navmesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_navmesh.cpp$(DependSuffix) -MM bot_navmesh.cpp

$(IntermediateDirectory)/bot_navmesh.cpp$(PreprocessSuffix): bot_navmesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_navmesh.cpp$(PreprocessSuffix) bot_navmesh.cpp

$(IntermediateDirectory)/bot_perceptron.cpp$(ObjectSuffix): bot_perceptron.cpp $(IntermediateDirectory)/bot_perceptron.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_perceptron.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_perceptron.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_perceptron.cpp$(DependSuffix): bot_perceptron.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_perceptron.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_perceptron.cpp$(DependSuffix) -MM bot_perceptron.cpp

$(IntermediateDirectory)/bot_perceptron.cpp$(PreprocessSuffix): bot_perceptron.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_perceptron.cpp$(PreprocessSuffix) bot_perceptron.cpp

$(IntermediateDirectory)/bot_profile.cpp$(ObjectSuffix): bot_profile.cpp $(IntermediateDirectory)/bot_profile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_profile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_profile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_profile.cpp$(DependSuffix): bot_profile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_profile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_profile.cpp$(DependSuffix) -MM bot_profile.cpp

$(IntermediateDirectory)/bot_profile.cpp$(PreprocessSuffix): bot_profile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_profile.cpp$(PreprocessSuffix) bot_profile.cpp

$(IntermediateDirectory)/bot_profiling.cpp$(ObjectSuffix): bot_profiling.cpp $(IntermediateDirectory)/bot_profiling.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_profiling.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_profiling.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_profiling.cpp$(DependSuffix): bot_profiling.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_profiling.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_profiling.cpp$(DependSuffix) -MM bot_profiling.cpp

$(IntermediateDirectory)/bot_profiling.cpp$(PreprocessSuffix): bot_profiling.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_profiling.cpp$(PreprocessSuffix) bot_profiling.cpp

$(IntermediateDirectory)/bot_schedule.cpp$(ObjectSuffix): bot_schedule.cpp $(IntermediateDirectory)/bot_schedule.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_schedule.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_schedule.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_schedule.cpp$(DependSuffix): bot_schedule.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_schedule.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_schedule.cpp$(DependSuffix) -MM bot_schedule.cpp

$(IntermediateDirectory)/bot_schedule.cpp$(PreprocessSuffix): bot_schedule.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_schedule.cpp$(PreprocessSuffix) bot_schedule.cpp

$(IntermediateDirectory)/bot_som.cpp$(ObjectSuffix): bot_som.cpp $(IntermediateDirectory)/bot_som.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_som.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_som.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_som.cpp$(DependSuffix): bot_som.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_som.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_som.cpp$(DependSuffix) -MM bot_som.cpp

$(IntermediateDirectory)/bot_som.cpp$(PreprocessSuffix): bot_som.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_som.cpp$(PreprocessSuffix) bot_som.cpp

$(IntermediateDirectory)/bot_squads.cpp$(ObjectSuffix): bot_squads.cpp $(IntermediateDirectory)/bot_squads.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_squads.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_squads.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_squads.cpp$(DependSuffix): bot_squads.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_squads.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_squads.cpp$(DependSuffix) -MM bot_squads.cpp

$(IntermediateDirectory)/bot_squads.cpp$(PreprocessSuffix): bot_squads.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_squads.cpp$(PreprocessSuffix) bot_squads.cpp

$(IntermediateDirectory)/bot_strings.cpp$(ObjectSuffix): bot_strings.cpp $(IntermediateDirectory)/bot_strings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_strings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_strings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_strings.cpp$(DependSuffix): bot_strings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_strings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_strings.cpp$(DependSuffix) -MM bot_strings.cpp

$(IntermediateDirectory)/bot_strings.cpp$(PreprocessSuffix): bot_strings.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_strings.cpp$(PreprocessSuffix) bot_strings.cpp

$(IntermediateDirectory)/bot_task.cpp$(ObjectSuffix): bot_task.cpp $(IntermediateDirectory)/bot_task.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_task.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_task.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_task.cpp$(DependSuffix): bot_task.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_task.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_task.cpp$(DependSuffix) -MM bot_task.cpp

$(IntermediateDirectory)/bot_task.cpp$(PreprocessSuffix): bot_task.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_task.cpp$(PreprocessSuffix) bot_task.cpp

$(IntermediateDirectory)/bot_tf2_mod.cpp$(ObjectSuffix): bot_tf2_mod.cpp $(IntermediateDirectory)/bot_tf2_mod.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_tf2_mod.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_tf2_mod.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_tf2_mod.cpp$(DependSuffix): bot_tf2_mod.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_tf2_mod.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_tf2_mod.cpp$(DependSuffix) -MM bot_tf2_mod.cpp

$(IntermediateDirectory)/bot_tf2_mod.cpp$(PreprocessSuffix): bot_tf2_mod.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_tf2_mod.cpp$(PreprocessSuffix) bot_tf2_mod.cpp

$(IntermediateDirectory)/bot_tf2_points.cpp$(ObjectSuffix): bot_tf2_points.cpp $(IntermediateDirectory)/bot_tf2_points.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_tf2_points.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_tf2_points.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_tf2_points.cpp$(DependSuffix): bot_tf2_points.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_tf2_points.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_tf2_points.cpp$(DependSuffix) -MM bot_tf2_points.cpp

$(IntermediateDirectory)/bot_tf2_points.cpp$(PreprocessSuffix): bot_tf2_points.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_tf2_points.cpp$(PreprocessSuffix) bot_tf2_points.cpp

$(IntermediateDirectory)/bot_usercmd.cpp$(ObjectSuffix): bot_usercmd.cpp $(IntermediateDirectory)/bot_usercmd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_usercmd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_usercmd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_usercmd.cpp$(DependSuffix): bot_usercmd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_usercmd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_usercmd.cpp$(DependSuffix) -MM bot_usercmd.cpp

$(IntermediateDirectory)/bot_usercmd.cpp$(PreprocessSuffix): bot_usercmd.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_usercmd.cpp$(PreprocessSuffix) bot_usercmd.cpp

$(IntermediateDirectory)/bot_utility.cpp$(ObjectSuffix): bot_utility.cpp $(IntermediateDirectory)/bot_utility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_utility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_utility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_utility.cpp$(DependSuffix): bot_utility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_utility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_utility.cpp$(DependSuffix) -MM bot_utility.cpp

$(IntermediateDirectory)/bot_utility.cpp$(PreprocessSuffix): bot_utility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_utility.cpp$(PreprocessSuffix) bot_utility.cpp

$(IntermediateDirectory)/bot_visibles.cpp$(ObjectSuffix): bot_visibles.cpp $(IntermediateDirectory)/bot_visibles.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_visibles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_visibles.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_visibles.cpp$(DependSuffix): bot_visibles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_visibles.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_visibles.cpp$(DependSuffix) -MM bot_visibles.cpp

$(IntermediateDirectory)/bot_visibles.cpp$(PreprocessSuffix): bot_visibles.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_visibles.cpp$(PreprocessSuffix) bot_visibles.cpp

$(IntermediateDirectory)/bot_waypoint.cpp$(ObjectSuffix): bot_waypoint.cpp $(IntermediateDirectory)/bot_waypoint.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_waypoint.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_waypoint.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_waypoint.cpp$(DependSuffix): bot_waypoint.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_waypoint.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_waypoint.cpp$(DependSuffix) -MM bot_waypoint.cpp

$(IntermediateDirectory)/bot_waypoint.cpp$(PreprocessSuffix): bot_waypoint.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_waypoint.cpp$(PreprocessSuffix) bot_waypoint.cpp

$(IntermediateDirectory)/bot_waypoint_locations.cpp$(ObjectSuffix): bot_waypoint_locations.cpp $(IntermediateDirectory)/bot_waypoint_locations.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_waypoint_locations.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_waypoint_locations.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_waypoint_locations.cpp$(DependSuffix): bot_waypoint_locations.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_waypoint_locations.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_waypoint_locations.cpp$(DependSuffix) -MM bot_waypoint_locations.cpp

$(IntermediateDirectory)/bot_waypoint_locations.cpp$(PreprocessSuffix): bot_waypoint_locations.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_waypoint_locations.cpp$(PreprocessSuffix) bot_waypoint_locations.cpp

$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(ObjectSuffix): bot_waypoint_visibility.cpp $(IntermediateDirectory)/bot_waypoint_visibility.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_waypoint_visibility.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(DependSuffix): bot_waypoint_visibility.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(DependSuffix) -MM bot_waypoint_visibility.cpp

$(IntermediateDirectory)/bot_waypoint_visibility.cpp$(PreprocessSuffix): bot_waypoint_visibility.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_waypoint_visibility.cpp$(PreprocessSuffix) bot_waypoint_visibility.cpp

$(IntermediateDirectory)/bot_weapons.cpp$(ObjectSuffix): bot_weapons.cpp $(IntermediateDirectory)/bot_weapons.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_weapons.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_weapons.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_weapons.cpp$(DependSuffix): bot_weapons.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_weapons.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_weapons.cpp$(DependSuffix) -MM bot_weapons.cpp

$(IntermediateDirectory)/bot_weapons.cpp$(PreprocessSuffix): bot_weapons.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_weapons.cpp$(PreprocessSuffix) bot_weapons.cpp

$(IntermediateDirectory)/bot_wpt_dist.cpp$(ObjectSuffix): bot_wpt_dist.cpp $(IntermediateDirectory)/bot_wpt_dist.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_wpt_dist.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_wpt_dist.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_wpt_dist.cpp$(DependSuffix): bot_wpt_dist.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_wpt_dist.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_wpt_dist.cpp$(DependSuffix) -MM bot_wpt_dist.cpp

$(IntermediateDirectory)/bot_wpt_dist.cpp$(PreprocessSuffix): bot_wpt_dist.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_wpt_dist.cpp$(PreprocessSuffix) bot_wpt_dist.cpp

$(IntermediateDirectory)/bot_zombie.cpp$(ObjectSuffix): bot_zombie.cpp $(IntermediateDirectory)/bot_zombie.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_zombie.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_zombie.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_zombie.cpp$(DependSuffix): bot_zombie.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_zombie.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_zombie.cpp$(DependSuffix) -MM bot_zombie.cpp

$(IntermediateDirectory)/bot_zombie.cpp$(PreprocessSuffix): bot_zombie.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_zombie.cpp$(PreprocessSuffix) bot_zombie.cpp

$(IntermediateDirectory)/bot_sigscan.cpp$(ObjectSuffix): bot_sigscan.cpp $(IntermediateDirectory)/bot_sigscan.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_sigscan.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_sigscan.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_sigscan.cpp$(DependSuffix): bot_sigscan.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_sigscan.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_sigscan.cpp$(DependSuffix) -MM bot_sigscan.cpp

$(IntermediateDirectory)/bot_sigscan.cpp$(PreprocessSuffix): bot_sigscan.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_sigscan.cpp$(PreprocessSuffix) bot_sigscan.cpp

$(IntermediateDirectory)/bot_cvars.cpp$(ObjectSuffix): bot_cvars.cpp $(IntermediateDirectory)/bot_cvars.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_cvars.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_cvars.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_cvars.cpp$(DependSuffix): bot_cvars.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_cvars.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_cvars.cpp$(DependSuffix) -MM bot_cvars.cpp

$(IntermediateDirectory)/bot_cvars.cpp$(PreprocessSuffix): bot_cvars.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_cvars.cpp$(PreprocessSuffix) bot_cvars.cpp

$(IntermediateDirectory)/bot_plugin_meta.cpp$(ObjectSuffix): bot_plugin_meta.cpp $(IntermediateDirectory)/bot_plugin_meta.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/mint/Dropbox/src/rcbot2/utils/RCBot2_meta/bot_plugin_meta.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bot_plugin_meta.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bot_plugin_meta.cpp$(DependSuffix): bot_plugin_meta.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bot_plugin_meta.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/bot_plugin_meta.cpp$(DependSuffix) -MM bot_plugin_meta.cpp

$(IntermediateDirectory)/bot_plugin_meta.cpp$(PreprocessSuffix): bot_plugin_meta.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bot_plugin_meta.cpp$(PreprocessSuffix) bot_plugin_meta.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(VS_Configuration)/


