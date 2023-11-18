#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"
#include "Unity/Unity.h"
#include "Unity/il2cppClass.h"
#include <And64InlineHook/And64InlineHook.hpp>

#define HOOK(offset, ptr, orig) A64HookFunction((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB(lib, offset, ptr, orig) A64HookFunction((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

#define HOOK_NO_ORIG(offset, ptr) A64HookFunction((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) A64HookFunction((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

#define HOOKSYM(sym, ptr, org) A64HookFunction(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)
#define HOOKSYM_LIB(lib, sym, ptr, org) A64HookFunction(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)

#define HOOKSYM_NO_ORIG(sym, ptr) A64HookFunction(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, NULL)
#define HOOKSYM_LIB_NO_ORIG(lib, sym, ptr) A64HookFunction(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, NULL)

//Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>

#define HOOK(offset, ptr, orig) MSHookFunction((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB(lib, offset, ptr, orig) MSHookFunction((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

#define HOOK_NO_ORIG(offset, ptr) MSHookFunction((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) MSHookFunction((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)

#define HOOKSYM(sym, ptr, org) MSHookFunction(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)
#define HOOKSYM_LIB(lib, sym, ptr, org) MSHookFunction(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)

#define HOOKSYM_NO_ORIG(sym, ptr)  MSHookFunction(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, NULL)
#define HOOKSYM_LIB_NO_ORIG(lib, sym, ptr)  MSHookFunction(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, NULL)



//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

#include "Includes/Macros.h"

// fancy struct for patches for kittyMemory
struct My_Patches {

    MemoryPatch SliderExample, Map1, Map2, Map3, Map4, Show, Cam, Showrank, Showname, Fps1, Fps2, Fps3, Aim, God,Onehit, Canattack1, Canattack2, Cd30, Autowin, Nocd, Skip55, Fixlag, Cansell, NoSkillCD, LichSuDau ;
    
} hexPatches;

bool feature1, feature2, featureHookToggle, Health;
bool mod1;
bool mod2;
bool mod3;
bool mod4;
bool mod5;
bool mod6;
bool mod7;
bool mod14;
bool mod24;
bool ShowSkillState = false;
bool Hackmap = false;
bool isPlayerName = false;
int sliderValue = 1, level = 0;
void *instanceBtn;

// Hooking examples. Assuming you know how to write hook
void (*AddMoneyExample)(void *instance, int amount);




void (*old_SetPlayerName)(void *instance, monoString *just_create, void *prefixName, bool *isGuideLevel);
void SetPlayerName(void *instance, monoString *just_create, void *prefixName, bool *isGuideLevel) {
 if (instance != NULL && isPlayerName) {
  just_create = CreateIl2cppString("Github: github.com/ImTienNguyenZ");
 }
 old_SetPlayerName(instance, just_create, prefixName, isGuideLevel);
}




//Show Info

void (*_ShowHeroInfo)(void *instance, int actor, bool bShow);
void ShowHeroInfo(void *instance, int actor, bool bShow) {
    if (instance != NULL&&ShowSkillState) {
      actor = 1;
      bShow = true;
    } 
    _ShowHeroInfo(instance, actor, bShow);
}

void (*_ShowSkillStateInfo)(void *instance, bool bShow);
void ShowSkillStateInfo(void *instance, bool bShow) {
    if (instance != NULL&&ShowSkillState) {
      bShow = true;
    }
    _ShowSkillStateInfo(instance, bShow);
}

void (*_ShowHeroHpInfo)(void *instance, bool bShow);
void ShowHeroHpInfo(void *instance, bool bShow) {
    if (instance != NULL&&ShowSkillState) {
      bShow = true;
    }
    _ShowHeroHpInfo(instance, bShow);
}





void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    //Anti-lib rename
    /*
    do {
        sleep(1);
    } while (!isLibraryLoaded("libYOURNAME.so"));*/

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

HOOK_LIB("libil2cpp.so", "0xEB8398", SetPlayerName, old_SetPlayerName);

hexPatches.Map1 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x23A73CC")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
hexPatches.Map2 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x23F14FC")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
hexPatches.Map3 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x2585B10")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));





MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", 0x1C09F5C), (void *) ShowHeroInfo, (void **) &_ShowHeroInfo);
MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", 0x20020C4), (void *) ShowSkillStateInfo, (void **) &_ShowSkillStateInfo);
MSHookFunction((void *) getAbsoluteAddress("libil2cpp.so", 0x20001D4), (void *) ShowHeroHpInfo, (void **) &_ShowHeroHpInfo);



hexPatches.Cam = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x1DA8438")),OBFUSCATE("81 01 A0 E3 1E FF 2F E1"));
hexPatches.Fps1 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x14B7394")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
hexPatches.Fps2 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x14A8FF0")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
hexPatches.Fps3 = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x14A8EFC")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
hexPatches.NoSkillCD = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x258AEF0")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));


//Aimskill


hexPatches.Aim = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x2212D54")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));

// Antiban
MemoryPatch::createWithHex("libil2cpp.so", 0x1F25F7C, "00 F0 20 E3 1E FF 2F E1").Modify();

MemoryPatch::createWithHex("libil2cpp.so", 0x1F25E48, "00 F0 20 E3 1E FF 2F E1").Modify();


// Show Lich Su Dau
hexPatches.LichSuDau = MemoryPatch::createWithHex(targetLibName,string2Offset(OBFUSCATE("0x16B3DC4")),OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
/*
MemoryPatch::createWithHex("libanogs.so", 0x3AC, "1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x11090, "00 46 70 47").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x124E0, "00 46 70 47").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1C44C, "1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1C454, "00 00 00 00").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1C45C, "1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1C474, "1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1E1550, "00 00 70 47").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1EB150, "70 47 70 47").Modify();
MemoryPatch::createWithHex("libanogs.so", 0x1EB154, "70 47 9A 42").Modify();   
MemoryPatch::createWithHex("libanogs.so", 0x00FC42D8 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x00FC4EB4 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x00FC4FA8 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x00FD3138 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x00FD45C4 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x01EFBE34 , "00000000").Modify(); 
MemoryPatch::createWithHex("libanogs.so", 0x01EFD378 , "00000000").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0x10D50E8, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0x18BCB9C, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0x18BCCD0, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0x1EAB148, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xDEFA00, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xDF00C0, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xDF0E48, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0xDF5028, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0xDFD228, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xE01378, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0xE02968, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0xE029F8, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xE0ED58, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xE0F5C8, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0xE81888, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0x10D29F4, "00 00 A0 E3 1E FF 2F E1").Modify(); 
MemoryPatch::createWithHex("libil2cpp.so", 0x10DF260, "00 00 A0 E3 1E FF 2F E1").Modify();
MemoryPatch::createWithHex("libil2cpp.so", 0x10D50E8, "00 00 A0 E3 1E FF 2F E1").Modify();
*/
   LOGI(OBFUSCATE("Done"));

    return NULL;
}

//JNI calls
extern "C" {

// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_Check Box
// To learn HTML, go to this page: https://www.w3schools.com/

JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    //Toasts added here so it's harder to remove it
    MakeToast(env, context, OBFUSCATE("Share By: ImTienNguyenZ"), Toast::LENGTH_LONG);

    const char *features[] = {
            OBFUSCATE("Collapse_Game Hack"),
            OBFUSCATE("1_CollapseAdd_ButtonOnOff_Hack Map"),
            OBFUSCATE("3_CollapseAdd_ButtonOnOff_High Cam"),
            OBFUSCATE("5_CollapseAdd_ButtonOnOff_Aim Skill"),
            OBFUSCATE("CollapseAdd_RichTextView_"
			"<font color='yellow'>Notes : Bật Hack Map khi gần vào trận tắt trước khi trụ vỡ."),
            OBFUSCATE("Collapse_Show Info"),
            OBFUSCATE("2_CollapseAdd_ButtonOnOff_Show Info Hero"),
            OBFUSCATE("6_CollapseAdd_ButtonOnOff_Show Battle History"),
            OBFUSCATE("Collapse_Unlock FPS"),
			OBFUSCATE("4_CollapseAdd_CheckBox_Unlock 60 FPS"),
			OBFUSCATE("14_CollapseAdd_CheckBox_Unlock 90 FPS"),
			OBFUSCATE("24_CollapseAdd_CheckBox_Unlock 120 FPS"),
            OBFUSCATE("Collapse_Menu Bot"),
            OBFUSCATE("7_CollapseAdd_ButtonOnOff_No Skill CD ( Bot )"),
            OBFUSCATE("Collapse_Set Name"),
            OBFUSCATE("8_CollapseAdd_ButtonOnOff_Hide Name"),
			OBFUSCATE("ButtonLink_Download & Update_https://github.com/ImTienNguyenZ/CheatAovSoucre"), //Not counted
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 1:
            mod1 = boolean;
            if (mod1) {
                hexPatches.Map1.Modify();
				hexPatches.Map2.Modify();
                hexPatches.Map3.Modify();
            } else {
                hexPatches.Map1.Restore();
				hexPatches.Map2.Restore();
                hexPatches.Map3.Restore();
            }
            break;
		case 2:
            ShowSkillState = boolean;
            break;
		case 3:
            mod3 = boolean;
            if (mod3) {
                hexPatches.Cam.Modify();
            } else {
                hexPatches.Cam.Restore();
            }
            break;
		case 4:
            mod4 = boolean;
            if (mod4) {
                hexPatches.Fps1.Modify();
            } else {
				hexPatches.Fps1.Restore();
            }
            break;
		case 14:
            mod14 = boolean;
            if (mod14) {
                hexPatches.Fps2.Modify();
            } else {
                hexPatches.Fps2.Restore();
            }
            break;
		case 24:
            mod24 = boolean;
            if (mod24) {
                hexPatches.Fps3.Modify();
            } else {
                hexPatches.Fps3.Restore();
            }
            break;
		case 5:
            mod5 = boolean;
            if (mod5) {
                hexPatches.Aim.Modify();
            } else {
                hexPatches.Aim.Restore();
            }
            break;
       case 6:
            mod6 = boolean;
            if (mod6) {
                hexPatches.LichSuDau.Modify();
            } else {
                hexPatches.LichSuDau.Restore();
            }
            break;
       case 7:
            mod7 = boolean;
            if (mod7) {
                hexPatches.NoSkillCD.Modify();
            } else {
                hexPatches.NoSkillCD.Restore();
            }
            break;
		case 8:
            isPlayerName = boolean;
            break;
    }
}
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}
 */
