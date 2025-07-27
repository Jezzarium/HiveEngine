#include "SurfaceFactory.h"
#include "GraphicDeviceVulkan.h"
#include <dlfcn.h>
#include <vulkan/vulkan.h>


//TODO error handling. Need to free the allocated resources if something fail
hive::gfx::DeviceHandle* hive::vk::vulkan_device_create(const gfx::DeviceDesc& create_info)
{
    HIVE_LOG_DEBUG("Creating graphic device");
    auto device = static_cast<gfx::DeviceHandle *>(HV_NEW(sizeof(gfx::DeviceHandle), HIVE_ALLOC_CAT_ENGINE));
    if (!device)
    {
        HIVE_LOG_ERROR("Failed to allocate GraphicDevice");
        return nullptr;
    }

    vkb::InstanceBuilder instance_builder;
    auto instance_ret = instance_builder
            .request_validation_layers()
            .use_default_debug_messenger() //TODO use own callback instead
            .build();

    if (!instance_ret)
    {
        auto error = instance_ret.error();
        HIVE_LOG_ERROR("Failed to create vulkan instance. Error code: %d, Message: %s",
                       static_cast<int>(error.value()), error.message().c_str());
        vk::vulkan_device_destroy(device); //To free the already allocated memory
        return nullptr;
    }

    device->m_vkb_instance = instance_ret.value();

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    hive::vk::vulkan_create_surface(create_info.display, device->m_vkb_instance, &surface);
    if (surface == VK_NULL_HANDLE)
    {
        HIVE_LOG_ERROR("Failed to create vulkan surface");
        vk::vulkan_device_destroy(device); //To free the already allocated memory
        return nullptr;
    }



    device->m_vk_surface = surface;

    vkb::PhysicalDeviceSelector device_selector(device->m_vkb_instance);

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    auto phys_ret = device_selector
            .set_surface(device->m_vk_surface)
            .require_present()
            .set_required_features(device_features)
            .select();

    if (!phys_ret)
    {
        HIVE_LOG_ERROR("Failed to select physical device");
        vk::vulkan_device_destroy(device); //To free the already allocated memory
        return nullptr;
    }

    device->m_vkb_phys_device = phys_ret.value();

    vkb::DeviceBuilder device_builder(device->m_vkb_phys_device);
    auto device_ret = device_builder.build();

    if (!device_ret)
    {
        HIVE_LOG_ERROR("Failed to create vulkan device");
        vk::vulkan_device_destroy(device); //To free the already allocated memory
        return nullptr;
    }

    device->m_vkb_device = device_ret.value();

    return device;
}

void hive::vk::vulkan_device_destroy(gfx::DeviceHandle* device)
{
    HIVE_LOG_DEBUG("Destroying graphic device");
    HIVE_ASSERT(device != nullptr, "device is null");

    if (device->m_vk_surface != VK_NULL_HANDLE)
    {
        vkb::destroy_surface(device->m_vkb_instance, device->m_vk_surface);
    }

    vkb::destroy_device(device->m_vkb_device);
    vkb::destroy_instance(device->m_vkb_instance);

    HV_FREE(device);
}

void hive::vk::vulkan_device_wait_idle(const gfx::DeviceHandle* device)
{
    vkDeviceWaitIdle(device->m_vkb_device);
}
