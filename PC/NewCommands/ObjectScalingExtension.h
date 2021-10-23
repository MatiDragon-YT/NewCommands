#pragma once
#include <extender/ObjectExtender.h>

class ObjectScalingExtension
{
public:
    struct ExtensionInfo
    {
        bool  m_bObjectScaled = false;
        float m_fObjectScaleX = 1.0f;
        float m_fObjectScaleY = 1.0f;
        float m_fObjectScaleZ = 1.0f;
        ExtensionInfo(CObject*) {}
    };
    static plugin::ObjectExtendedData<ExtensionInfo> extensionData;
    static float GetObjectScaleX(CObject* object);
    static float GetObjectScaleY(CObject* object);
    static float GetObjectScaleZ(CObject* object);
    static void SetObjectScale(CObject* object, float scaleX, float scaleY, float scaleZ);
    ObjectScalingExtension();
};
extern ObjectScalingExtension extensionInstance;