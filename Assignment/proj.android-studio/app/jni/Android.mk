LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external/lua/lua)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external/lua/tolua)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/scripting/lua-bindings)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/MK/Input/MKInputDefinition.cpp \
				   $(LOCAL_PATH)/../../../Classes/MK/Input/MKInputManager.cpp \
				   $(LOCAL_PATH)/../../../Classes/MK/Input/MKKeyboardHandler.cpp \
				   $(LOCAL_PATH)/../../../Classes/MK/Input/MKTouchHandler.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
					$(LOCAL_PATH)/../../../cocos2d/cocos/scripting/lua-bindings/manual \
					$(LOCAL_PATH)/../../../cocos2d/external/lua/lua \
					$(LOCAL_PATH)/../../../cocos2d/external/lua/tolua \

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
