#include "ObjectScalingExtension.h"

float ObjectScalingExtension::GetObjectScaleX(CObject* object)
{
    return extensionData.Get(object).m_fObjectScaleX;
}

float ObjectScalingExtension::GetObjectScaleY(CObject* object)
{
    return extensionData.Get(object).m_fObjectScaleY;
}

float ObjectScalingExtension::GetObjectScaleZ(CObject* object)
{
    return extensionData.Get(object).m_fObjectScaleZ;
}

void ObjectScalingExtension::SetObjectScale(CObject* object, float scaleX, float scaleY, float scaleZ)
{
    extensionData.Get(object).m_fObjectScaleX = scaleX;
    extensionData.Get(object).m_fObjectScaleY = scaleY;
    extensionData.Get(object).m_fObjectScaleZ = scaleZ;
}

ObjectScalingExtension::ObjectScalingExtension()
{
    plugin::Events::objectRenderEvent.before += [](CObject* object)
    {
        auto& data = extensionData.Get(object);
        if (data.m_fObjectScaleX != 1.0f || data.m_fObjectScaleY != 1.0f || data.m_fObjectScaleZ != 1.0f || data.m_bObjectScaled)
            if (object->m_pRwObject) {
                object->m_placement.UpdateRW();
                RwFrame* frame = reinterpret_cast<RwFrame*>(object->m_pRwObject->parent);
                RwV3d scale = { data.m_fObjectScaleX, data.m_fObjectScaleY, data.m_fObjectScaleZ };
                RwMatrixScale(&frame->modelling, &scale, rwCOMBINEPRECONCAT);
                //RwFrameScale(frame, &scale, rwCOMBINEPRECONCAT);
                RwFrameUpdateObjects(frame);
                if (data.m_fObjectScaleX != 1.0f || data.m_fObjectScaleY != 1.0f || data.m_fObjectScaleZ != 1.0f)
                    data.m_bObjectScaled = true;
                else
                    data.m_bObjectScaled = false;
            }
    };
}
ObjectScalingExtension extensionInstance;
plugin::ObjectExtendedData<ObjectScalingExtension::ExtensionInfo> ObjectScalingExtension::extensionData;