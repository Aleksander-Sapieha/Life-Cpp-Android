
#include <jni.h>
#include <memory>
#include <string>

#include "game.h"

using namespace std;

static unique_ptr<Game> game;

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_sourceguy_life_1cpp_MainActivity_nativeCreateGame(
        JNIEnv* env, jobject, jstring name
) {
    const char* playerName = env->GetStringUTFChars(name, nullptr);

    game = make_unique<Game>(playerName);

    env->ReleaseStringUTFChars(name, playerName);

    return env->NewStringUTF("Game created!");
}

JNIEXPORT jstring JNICALL
Java_com_sourceguy_life_1cpp_MainActivity_nativeStartDay(
        JNIEnv* env, jobject
) {
    if (!game) return env->NewStringUTF("Start a game first.");

    string result = game->startDay();
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_com_sourceguy_life_1cpp_MainActivity_nativeChooseAction(
        JNIEnv* env, jobject, jint choice
) {
    if (!game) return env->NewStringUTF("Start a game first.");

    string result = game->chooseAction(choice);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_com_sourceguy_life_1cpp_MainActivity_nativeChooseJob(
        JNIEnv* env, jobject, jint choice
) {
    if (!game) return env->NewStringUTF("Start a game first.");

    string result = game->chooseJob(choice);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jboolean JNICALL
Java_com_sourceguy_life_1cpp_MainActivity_nativeIsFinished(
        JNIEnv*, jobject
) {
    return game && game->isFinished();
}

}
