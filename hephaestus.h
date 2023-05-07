#pragma once

#ifndef HEPHAESTUS
#define HEPHAESTUS

#ifndef debug
#define VK_NO_PROTOTYPES
#define VK_NO_DEBUG
#define VK_NO_FILESYSTEM
#endif

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <Volk/volk.h>
///need <vulkan/...>
///need <vk_video/...>
//#include <vma/vk_mem_alloc.h>

#include <shaderc/shaderc.h>

//

#define make_version( major, minor, patch ) VK_MAKE_API_VERSION(0, major, minor, patch)

//

typedef VkLayerProperties debuglayer_properties;

//

typedef VkApplicationInfo info_app;
#define make_app_info(in_next, in_app_name, in_app_version, in_engine_name, in_engine_version, in_api_version) \
{                                               \
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,\
	.pNext = in_next,                           \
	.pApplicationName = in_app_name,            \
	.applicationVersion = in_app_version,       \
	.pEngineName = in_engine_name,              \
	.engineVersion = in_engine_version,         \
	.apiVersion = in_api_version,               \
};												\

//

typedef VkInstance instance;
typedef VkInstanceCreateInfo info_instance;
#define make_info_instance( in_next, in_flags, in_app_info, in_layer_count, in_layer_names, in_extension_count, in_extension_names) \
{													\
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,\
	.pNext = in_next,								\
	.flags = in_flags,								\
	.pApplicationInfo = in_app_info,				\
	.enabledLayerCount = in_layer_count,			\
	.ppEnabledLayerNames = in_layer_names,			\
	.enabledExtensionCount = in_extension_count,	\
	.ppEnabledExtensionNames = in_extension_names,	\
};													\

instance new_instance( info_instance in_info )
{
	instance temp_instance;
	vkCreateInstance(&(in_info), NULL, &(temp_instance));
	volkLoadInstanceOnly(temp_instance);
	return temp_instance;
}

//

typedef VkPhysicalDevice physical_device;

typedef VkDevice device;
typedef VkDeviceCreateInfo info_device;
#define make_device_info(in_next, in_flags, in_queue_create_info_count, in_queue_create_infos, in_enabled_layer_count, in_enabled_layer_names, in_enabled_extension_count, in_enabled_extension_names, in_enabled_features) \
{                                                           \
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,         \
    .pNext = in_next,                                      \
    .flags = in_flags,                                     \
    .queueCreateInfoCount = in_queue_create_info_count,    \
    .pQueueCreateInfos = in_queue_create_infos,            \
    .enabledLayerCount = in_enabled_layer_count,           \
    .ppEnabledLayerNames = in_enabled_layer_names,         \
    .enabledExtensionCount = in_enabled_extension_count,   \
    .ppEnabledExtensionNames = in_enabled_extension_names, \
    .pEnabledFeatures = in_enabled_features,               \
};

device new_device(physical_device in_physical_device, info_device in_info)
{
	device temp_device;
	vkCreateDevice(in_physical_device, &in_info, NULL, &temp_device);
	return temp_device;
}

//

typedef VkQueue queue;

queue get_queue(device in_device, uint32_t family_index, uint32_t queue_index)
{
	queue temp_queue;
	vkGetDeviceQueue(in_device, family_index, queue_index, &temp_queue);
	return temp_queue;
}

#endif
