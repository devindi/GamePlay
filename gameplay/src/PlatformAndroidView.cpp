#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "gameplay.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//asset manager should be global
AAssetManager* __assetManager;

static gameplay::Game* _game;
static int __width;
static int __height;
static struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
static bool __vsync = WINDOW_VSYNC;
static bool __multiSampling = false;
static bool __multiTouch = false;

// OpenGL VAO functions.
static const char* __glExtensions;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;
PFNGLMAPBUFFEROESPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFEROESPROC glUnmapBuffer = NULL;

namespace gameplay
{

extern int strcmpnocase(const char* s1, const char* s2)
{
    return strcasecmp(s1, s2);
}

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, format, argptr);
    va_end(argptr);
}

std::string Platform::displayFileDialog(size_t mode, const char* title, const char* filterDescription, const char* filterExtensions, const char* initialDirectory)
{
    return "";
}

static double timespec2millis(struct timespec *a)
{
    GP_ASSERT(a);
    return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
}

void Platform::displayKeyboard(bool display)
{
    //not
}

void Platform::shutdownInternal()
{
    Game::getInstance()->shutdown();
}

unsigned int Platform::getDisplayWidth()
{
    return __width;
}

unsigned int Platform::getDisplayHeight()
{
    return __height;
}

double Platform::getAbsoluteTime()
{
    clock_gettime(CLOCK_REALTIME, &__timespec);
    double now = timespec2millis(&__timespec);
    __timeAbsolute = now - __timeStart;

    return __timeAbsolute;
}

void Platform::setAbsoluteTime(double time)
{
    __timeAbsolute = time;
}

bool Platform::isVsync()
{
    return __vsync;
}

void Platform::setVsync(bool enable)
{
    eglSwapInterval(EGL_NO_DISPLAY, enable ? 1 : 0);
    __vsync = enable;
}


void Platform::swapBuffers()
{

}

void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

void Platform::setMultiSampling(bool enabled)
{
    if (enabled == __multiSampling)
    {
        return;
    }
    __multiSampling = enabled;
}

bool Platform::isMultiSampling()
{
    return __multiSampling;
}

void Platform::setMultiTouch(bool enabled)
{
    __multiTouch = enabled;
}

bool Platform::isMultiTouch()
{
    return __multiTouch;
}

bool Platform::hasAccelerometer()
{
    return false;
}

void Platform::signalShutdown()
{
    // nothing to do
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
}

bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
}

void Platform::getArguments(int* argc, char*** argv)
{
    if (argc)
        *argc = 0;
    if (argv)
        *argv = 0;
}

bool Platform::hasMouse()
{
    // not
    return false;
}

void Platform::setMouseCaptured(bool captured)
{
    // not
}

bool Platform::isMouseCaptured()
{
    // not
    return false;
}

void Platform::setCursorVisible(bool visible)
{
    // not
}

bool Platform::isCursorVisible()
{
    // not
    return false;
}

bool Platform::canExit()
{
    return true;
}

void Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    if (accelX)
    {
        *accelX = 0;
    }

    if (accelY)
    {
        *accelY = 0;
    }

    if (accelZ)
    {
        *accelZ = 0;
    }

    if (gyroX)
    {
        *gyroX = 0;
    }

    if (gyroY)
    {
        *gyroY = 0;
    }

    if (gyroZ)
    {
        *gyroZ = 0;
    }
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = 0;
    *roll = 0;
}

bool Platform::launchURL(const char* url)
{
    return false;
}

Platform::~Platform()
{
}

int Platform::enterMessagePump()
{
    return 0;
}







static void printGLString(const char *name, GLenum s) {
    LOGI("glGetString");
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    LOGI("glGetError");
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
    LOGI("glGetError - END");
}





}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager)
{

    __width = width;
    __height = height;
    __assetManager = AAssetManager_fromJava(env, assetManager);
    gameplay::printGLString("Version", GL_VERSION);
    gameplay::printGLString("Vendor", GL_VENDOR);
    gameplay::printGLString("Renderer", GL_RENDERER);
    gameplay::printGLString("Extensions", GL_EXTENSIONS);
    __glExtensions = (const char*)glGetString(GL_EXTENSIONS);

    if (strstr(__glExtensions, "GL_OES_vertex_array_object") || strstr(__glExtensions, "GL_ARB_vertex_array_object"))
    {
        glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
        glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
        glMapBuffer = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
        glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
    }
    _game = gameplay::Game::getInstance();
    _game->run();

}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    _game->frame();
}
