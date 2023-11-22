// // // // // // //
// > Hephaestus _
// -------
// c7h16-conforming Vulkan abstraction
// requires: Volk
// @ENDESGA 2023
// // // // // // //

#pragma once
#ifndef Hephaestus_included
#define Hephaestus_included

#include <Volk/volk.c>

/*
//

typedef VkNAME H_NAME;
typedef VkNAMECreateInfo H_struct_NAME;

H_NAME H_new_NAME
(
	const u32 in_param,
)
{
	H_NAME temp_NAME = NULL;
	H_struct_NAME temp_struct_NAME = {VK_STRUCTURE_TYPE_NAME_CREATE_INFO};

	temp_struct_NAME.param = in_param;

	H_CHECK( vkCreateNAME( &temp_struct_NAME, NULL, &temp_NAME ) );
	return temp_NAME;
}

//
*/

#define H_create_version( major, minor, patch ) VK_MAKE_API_VERSION( 0, major, minor, patch )

#define H_result VkResult

const char* H_result_to_string(const H_result in_error) {
    switch (in_error) {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        default: return "UNKNOWN_ERROR";
    }
}

#define H_CHECK( _ )                                \
		do                                                \
		{                                                 \
			H_result error = _;                             \
			if( error )                                     \
			{                                               \
				printf( "ERROR IN: %s\n", #_ );               \
				printf( "%s\n", H_result_to_string( error ) ); \
				while( 1 ) {};                                \
			}                                               \
		}                                                 \
		while( 0 )

//

typedef VkApplicationInfo H_struct_app;
#define H_create_struct_app(            \
    in_app_name,                          \
    in_app_version,                       \
    in_engine_name,                       \
    in_engine_version,                    \
    in_api_version                        \
)                                       \
		( H_struct_app )                      \
		{                                     \
			VK_STRUCTURE_TYPE_APPLICATION_INFO, \
				NULL,                             \
				in_app_name,                      \
				in_app_version,                   \
				in_engine_name,                   \
				in_engine_version,                \
				in_api_version                    \
		}

typedef VkExtensionProperties H_extension_properties;

unsigned int H_get_instance_extensions_count() {
	unsigned int extension_count = 0;
	vkEnumerateInstanceExtensionProperties( NULL, &extension_count, NULL );
	return extension_count;
}

void H_get_instance_extensions(unsigned int in_count, VkExtensionProperties* out_extensions) {
	H_CHECK(vkEnumerateInstanceExtensionProperties(NULL, &in_count, out_extensions));
}

typedef VkLayerProperties H_debug_layer_properties;

unsigned int H_get_instance_layer_count() {
	unsigned int debug_layer_count = 0;
	H_CHECK(vkEnumerateInstanceLayerProperties( &debug_layer_count, NULL ));
	return debug_layer_count;
}

void H_get_instance_layers(unsigned int in_count, H_debug_layer_properties* out_layers) {
    H_CHECK(vkEnumerateInstanceLayerProperties( &in_count, out_layers ));
}

typedef VkInstance H_instance;
typedef VkInstanceCreateInfo H_struct_instance;

H_instance H_new_instance
(
    const H_struct_app* in_app_info,
    const u32 in_layer_count,
    const char* const* in_layer_names,
    const u32 in_extension_count,
    const char* const* in_extension_names
) {
    H_instance temp_instance = NULL;
    H_struct_instance temp_struct_instance = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};

    temp_struct_instance.pApplicationInfo = in_app_info;
    temp_struct_instance.enabledLayerCount = in_layer_count;
    temp_struct_instance.ppEnabledLayerNames = in_layer_names;
    temp_struct_instance.enabledExtensionCount = in_extension_count;
    temp_struct_instance.ppEnabledExtensionNames = in_extension_names;

    H_CHECK(vkCreateInstance( &temp_struct_instance, NULL, &temp_instance ));
    volkLoadInstanceOnly(temp_instance);
    return temp_instance;
}

#define H_delete_instance( INSTANCE ) vkDestroyInstance(INSTANCE, NULL)

#define H_gpu_type_discrete VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
#define H_gpu_type_integrated VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU

typedef VkPhysicalDevice H_gpu;

unsigned int H_get_gpu_count(const H_instance in_instance) {
	unsigned int physical_device_count = 0;
	H_CHECK(vkEnumeratePhysicalDevices( in_instance, &physical_device_count, NULL ));
	return physical_device_count;
}

void H_get_gpus(const H_instance in_instance, unsigned int in_count, H_gpu* out_gpu_ptr) {
    H_CHECK(vkEnumeratePhysicalDevices( in_instance, &in_count, out_gpu_ptr ));
}

typedef VkPhysicalDeviceFeatures H_gpu_features;

H_gpu_features H_get_gpu_features(const H_gpu in_gpu) {
    H_gpu_features features;
    vkGetPhysicalDeviceFeatures(in_gpu, &features);
    return features;
}

typedef VkPhysicalDeviceProperties H_gpu_properties;

H_gpu_properties H_get_gpu_properties(const H_gpu in_gpu) {
    H_gpu_properties properties;
    vkGetPhysicalDeviceProperties(in_gpu, &properties);
    return properties;
}

typedef VkQueueFamilyProperties H_gpu_queue;

unsigned int H_get_gpu_queue_count(const H_gpu in_gpu) {
	unsigned int queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(in_gpu, &queue_family_count, NULL);
	return queue_family_count;
}

void H_get_gpu_queues(const H_gpu in_gpu, unsigned int in_count, H_gpu_queue* out_gpu_queue) {
    vkGetPhysicalDeviceQueueFamilyProperties(in_gpu, &in_count, out_gpu_queue);
}

#define H_gpu_queue_render VK_QUEUE_GRAPHICS_BIT
#define H_gpu_queue_compute VK_QUEUE_COMPUTE_BIT
#define H_gpu_queue_transfer VK_QUEUE_TRANSFER_BIT
#define H_check_gpu_queue_render( QUEUE ) ( QUEUE.queueFlags & H_gpu_queue_render )
#define H_check_gpu_queue_compute( QUEUE ) ( QUEUE.queueFlags & H_gpu_queue_compute )
#define H_check_gpu_queue_transfer( QUEUE ) ( QUEUE.queueFlags & H_gpu_queue_transfer )

typedef VkPhysicalDeviceMemoryProperties H_gpu_memory_properties;

H_gpu_memory_properties H_get_gpu_memory_properties(const H_gpu in_gpu) {
    H_gpu_memory_properties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(in_gpu, &memory_properties);
    return memory_properties;
}

typedef VkSurfaceKHR H_surface;

unsigned int H_get_gpu_supports_present(const H_gpu in_gpu, const unsigned int in_queue_index,
                                               const H_surface in_window_surface) {
    unsigned int supports_present = 0;
    H_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR( in_gpu, in_queue_index, in_window_surface, &supports_present ));
    return supports_present;
}

typedef VkSurfaceFormatKHR H_surface_format;

unsigned int H_get_surface_format_count(const H_gpu in_gpu, const H_surface in_surface) {
	unsigned int formats_count = 0;
	H_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR( in_gpu, in_surface, &formats_count, NULL ));
	return formats_count;
}

void H_get_surface_formats(const H_gpu in_gpu, const H_surface in_surface,
                                          unsigned int formats_count, H_surface_format* out_surface_formats) {
    H_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR( in_gpu, in_surface, &formats_count, out_surface_formats ));
}

#define H_present_mode_vsync_off VK_PRESENT_MODE_IMMEDIATE_KHR
#define H_present_mode_vsync_on VK_PRESENT_MODE_MAILBOX_KHR
#define H_present_mode_vsync_relaxed VK_PRESENT_MODE_FIFO_RELAXED_KHR

typedef VkPresentModeKHR H_present_mode;

unsigned int H_get_present_mode_count(const H_gpu in_gpu, const H_surface in_surface) {
	unsigned int modes_count = 0;
	H_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR( in_gpu, in_surface, &modes_count, NULL ));
	return modes_count;
}

void H_get_present_modes(const H_gpu in_gpu, const H_surface in_surface,
                                        unsigned int modes_count, H_present_mode* out_present_modes) {
    H_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR( in_gpu, in_surface, &modes_count, out_present_modes ));
}

typedef VkSurfaceCapabilitiesKHR H_surface_capabilities;

H_surface_capabilities H_get_surface_capabilities(const H_gpu in_gpu, const H_surface in_surface) {
    H_surface_capabilities surface_capabilities;
    H_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR( in_gpu, in_surface, &surface_capabilities ));
    return surface_capabilities;
}

typedef VkDeviceQueueCreateInfo H_struct_device_queue;
#define H_create_struct_device_queue(           \
    in_queue_family_index,                        \
    in_queue_count,                               \
    in_queue_priorities                           \
)                                               \
		( H_struct_device_queue )                     \
		{                                             \
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, \
				NULL,                                     \
				0,                                        \
				in_queue_family_index,                    \
				in_queue_count,                           \
				in_queue_priorities                       \
		}

typedef VkDevice H_device;
typedef VkDeviceCreateInfo H_struct_device;

H_device H_new_device
(
    const H_gpu in_gpu,
    const u32 in_queue_create_struct_count,
    const H_struct_device_queue* in_queue_create_infos,
    const u32 in_enabled_extension_count,
    const char* const* in_enabled_extension_names,
    const H_gpu_features* in_enabled_features
) {
    H_device temp_device = NULL;
    H_struct_device temp_struct_device = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};

    temp_struct_device.queueCreateInfoCount = in_queue_create_struct_count;
    temp_struct_device.pQueueCreateInfos = in_queue_create_infos;
    temp_struct_device.enabledExtensionCount = in_enabled_extension_count;
    temp_struct_device.ppEnabledExtensionNames = in_enabled_extension_names;
    temp_struct_device.pEnabledFeatures = in_enabled_features;

    H_CHECK(vkCreateDevice( in_gpu, &temp_struct_device, NULL, &temp_device ));
    volkLoadDevice(temp_device);
    return temp_device;
}

#define delete_device( DEVICE ) vkDestroyDevice( DEVICE, NULL );

static H_device H_current_device = NULL;

typedef VkQueue H_queue;

H_queue H_get_queue(uint32_t family_index, uint32_t queue_index) {
    H_queue temp_queue;
    vkGetDeviceQueue(H_current_device, family_index, queue_index, &temp_queue);
    return temp_queue;
}

typedef VkCommandPool H_command_pool;
typedef VkCommandPoolCreateInfo H_struct_command_pool;

H_command_pool H_new_command_pool
(
	const u32 in_queue_family_index
)
{
	H_command_pool temp_command_pool = NULL;
	H_struct_command_pool temp_struct_command_pool = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};

	temp_struct_command_pool.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	temp_struct_command_pool.queueFamilyIndex = in_queue_family_index;

	H_CHECK(vkCreateCommandPool( H_current_device, &temp_struct_command_pool, NULL, &temp_command_pool ));
	return temp_command_pool;
}

#define delete_command_pool( COMMAND_POOL ) vkDestroyCommandPool( H_current_device, COMMAND_POOL, NULL )

typedef VkCommandBufferAllocateInfo H_struct_command_buffer;
#define H_create_struct_command_buffer(             \
    in_command_pool,                                  \
    in_level,                                         \
    in_command_buffer_count                           \
)                                                   \
		( H_struct_command_buffer )                       \
		{                                                 \
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, \
				NULL,                                         \
				in_command_pool,                              \
				in_level,                                     \
				in_command_buffer_count                       \
		}
typedef VkCommandBuffer H_command_buffer;

void _H_new_command_buffers(H_struct_command_buffer* in_info,
                            H_command_buffer* out_command_buffers) {
    H_CHECK(vkAllocateCommandBuffers( H_current_device, in_info, out_command_buffers ));
}

#define H_new_command_buffers(  STRUCT_COMMAND_BUFFER, PTR_COMMAND_BUFFERS ) _H_new_command_buffers(  &STRUCT_COMMAND_BUFFER, PTR_COMMAND_BUFFERS )

#define H_command_buffer_level_primary VK_COMMAND_BUFFER_LEVEL_PRIMARY
#define H_command_buffer_level_secondary VK_COMMAND_BUFFER_LEVEL_SECONDARY

typedef VkCommandBufferBeginInfo H_struct_command_buffer_start;
#define H_create_struct_command_buffer_start(    \
    in_inheritance_info                            \
)                                                \
		( H_struct_command_buffer_start )              \
		{                                              \
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, \
				NULL,                                      \
				0,                                         \
				in_inheritance_info                        \
		}

void _H_start_command_buffer(H_command_buffer in_command_buffer,
                             H_struct_command_buffer_start* in_struct_command_buffer_start) {
    H_CHECK(vkBeginCommandBuffer( in_command_buffer, in_struct_command_buffer_start ));
}

#define H_start_command_buffer( COMMAND_BUFFER, STRUCT_COMMAND_BUFFER_START ) _H_start_command_buffer( COMMAND_BUFFER, &STRUCT_COMMAND_BUFFER_START )

void H_end_command_buffer(H_command_buffer in_command_buffer) {
    H_CHECK(vkEndCommandBuffer( in_command_buffer ));
}

typedef VkSharingMode H_sharing_mode;

#define H_sharing_mode_exclusive VK_SHARING_MODE_EXCLUSIVE
#define H_sharing_mode_concurrent VK_SHARING_MODE_CONCURRENT

typedef VkMemoryRequirements H_memory_requirements;
typedef VkMemoryPropertyFlags H_memory_properties;

#define H_memory_property_host_visible VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
#define H_memory_property_host_coherent VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
#define H_memory_property_host_cached VK_MEMORY_PROPERTY_HOST_CACHED_BIT
#define H_memory_property_device_local VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
#define H_memory_property_protected VK_MEMORY_PROPERTY_PROTECTED_BIT
#define H_memory_property_lazily_allocated VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT

unsigned int H_find_mem(H_gpu_memory_properties in_memory_properties, unsigned int in_type_filter,
                        H_memory_properties in_properties) {
    for (unsigned int i = 0; i < in_memory_properties.memoryTypeCount; i++) {
        if ((in_type_filter & (1 << i)) && (in_memory_properties.memoryTypes[i].propertyFlags & in_properties) ==
            in_properties) {
            return i;
        }
    }
    return 0;
}

//

#define H_allocate_memory( INFO, OUT_MEMORY ) H_CHECK(vkAllocateMemory( INFO, NULL, &OUT_MEMORY ));

//

typedef VkBufferCreateInfo H_struct_buffer;
#define H_create_struct_buffer(           \
    in_size,                                \
    in_usage,                               \
    in_sharing_mode,                        \
    in_queue_family_index_count,            \
    in_queue_family_indices                 \
)                                         \
		( H_struct_buffer )                     \
		{                                       \
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, \
				NULL,                               \
				0,                                  \
				in_size,                            \
				in_usage,                           \
				in_sharing_mode,                    \
				in_queue_family_index_count,        \
				in_queue_family_indices             \
		}
typedef VkBuffer H_buffer;

H_buffer _H_new_buffer(H_struct_buffer* in_info) {
    H_buffer temp_buffer = NULL;
    H_CHECK(vkCreateBuffer( H_current_device, in_info, NULL, &temp_buffer ));
    return temp_buffer;
}

#define H_new_buffer( STRUCT_BUFFER ) _H_new_buffer( &STRUCT_BUFFER )

typedef VkBufferUsageFlags H_buffer_usage;
#define H_buffer_usage_vertex VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
#define H_buffer_usage_index VK_BUFFER_USAGE_INDEX_BUFFER_BIT
#define H_buffer_usage_storage VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
#define H_buffer_usage_indirect VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
#define H_buffer_usage_uniform VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
#define H_buffer_usage_transfer_dst VK_BUFFER_USAGE_TRANSFER_DST_BIT

H_memory_requirements H_get_memory_requirements_buffer( H_buffer in_buffer) {
    H_memory_requirements temp_requirements;
    vkGetBufferMemoryRequirements(H_current_device, in_buffer, &temp_requirements);
    return temp_requirements;
}

typedef VkMemoryAllocateInfo H_struct_memory;
#define H_create_struct_memory(             \
    in_allocation_size,                       \
    in_memory_type_index                      \
)                                           \
		( H_struct_memory )                       \
		{                                         \
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, \
				NULL,                                 \
				in_allocation_size,                   \
				in_memory_type_index                  \
		}
typedef VkDeviceMemory H_memory;

H_memory _H_new_memory_buffer( H_struct_memory* in_info, H_buffer in_buffer) {
    H_memory temp_memory = NULL;
    H_CHECK(vkAllocateMemory( H_current_device, in_info, NULL, &temp_memory ));
    H_CHECK(vkBindBufferMemory( H_current_device, in_buffer, temp_memory, 0 ));
    return temp_memory;
}

#define H_new_memory_buffer( STRUCT_MEMORY, BUFFER ) _H_new_memory_buffer( &STRUCT_MEMORY, BUFFER )

typedef VkFormat H_format;

H_format H_find_compatible_format(H_gpu in_gpu, const H_format* candidates, size_t candidate_count, VkFormatFeatureFlags features) {
	for (size_t i = 0; i < candidate_count; i++) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(in_gpu, candidates[i], &props);

		if ((props.optimalTilingFeatures & features) == features) {
			return candidates[i];
		}
	}
	return VK_FORMAT_UNDEFINED;
}

H_format H_find_depth_format(H_gpu in_gpu) {
	H_format depth_formats[] = {
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D16_UNORM
	};
	return H_find_compatible_format(in_gpu, depth_formats, sizeof(depth_formats) / sizeof(depth_formats[0]), VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

H_format H_find_depth_stencil_format(H_gpu in_gpu) {
	H_format depth_stencil_formats[] = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT
	};
	return H_find_compatible_format(in_gpu, depth_stencil_formats, sizeof(depth_stencil_formats) / sizeof(depth_stencil_formats[0]), VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

#define H_format_r_f32 VK_FORMAT_R32_SFLOAT
#define H_format_rg_f32 VK_FORMAT_R32G32_SFLOAT
#define H_format_rgb_f32 VK_FORMAT_R32G32B32_SFLOAT
#define H_format_rgba_f32 VK_FORMAT_R32G32B32A32_SFLOAT

#define H_format_r_u8_to_f32 VK_FORMAT_R8_SCALED
#define H_format_rg_u8_to_f32 VK_FORMAT_R8G8_SCALED
#define H_format_rgb_u8_to_f32 VK_FORMAT_R8G8B8_SCALED
#define H_format_bgr_u8_to_f32 VK_FORMAT_B8G8R8_SCALED
#define H_format_rgba_u8_to_f32 VK_FORMAT_R8G8B8A8_SCALED
#define H_format_bgra_u8_to_f32 VK_FORMAT_B8G8R8A8_SCALED
#define H_format_abgr_u8_to_f32 VK_FORMAT_A8B8G8R8_SCALED_PACK32
#define H_format_r_u16_to_f32 VK_FORMAT_R16_SCALED
#define H_format_rg_u16_to_f32 VK_FORMAT_R16G16_SCALED
#define H_format_rgb_u16_to_f32 VK_FORMAT_R16G16B16_SCALED
#define H_format_rgba_u16_to_f32 VK_FORMAT_R16G16B16A16_SCALED

#define H_format_r_f16 VK_FORMAT_R16_SFLOAT
#define H_format_rg_f16 VK_FORMAT_R16G16_SFLOAT
#define H_format_rgb_f16 VK_FORMAT_R16G16B16_SFLOAT
#define H_format_rgba_f16 VK_FORMAT_R16G16B16A16_SFLOAT

#define H_format_r_u8_to_norm_f32 VK_FORMAT_R8_UNORM
#define H_format_rg_u8_to_norm_f32 VK_FORMAT_R8G8_UNORM
#define H_format_rgb_u8_to_norm_f32 VK_FORMAT_R8G8B8_UNORM
#define H_format_bgr_u8_to_norm_f32 VK_FORMAT_B8G8R8_UNORM
#define H_format_rgba_u8_to_norm_f32 VK_FORMAT_R8G8B8A8_UNORM
#define H_format_bgra_u8_to_norm_f32 VK_FORMAT_B8G8R8A8_UNORM
#define H_format_abgr_u8_to_norm_f32 VK_FORMAT_A8B8G8R8_UNORM_PACK32

#define H_format_r_s8 VK_FORMAT_R8_SINT
#define H_format_r_u8 VK_FORMAT_R8_UINT
#define H_format_rg_s8 VK_FORMAT_R8G8_SINT
#define H_format_rg_u8 VK_FORMAT_R8G8_UINT
#define H_format_rgb_s8 VK_FORMAT_R8G8B8_SINT
#define H_format_rgb_u8 VK_FORMAT_R8G8B8_UINT
#define H_format_bgr_s8 VK_FORMAT_B8G8R8_SINT
#define H_format_bgr_u8 VK_FORMAT_B8G8R8_UINT
#define H_format_rgba_s8 VK_FORMAT_R8G8B8A8_SINT
#define H_format_rgba_u8 VK_FORMAT_R8G8B8A8_UINT
#define H_format_bgra_s8 VK_FORMAT_B8G8R8A8_SINT
#define H_format_bgra_u8 VK_FORMAT_B8G8R8A8_UINT
#define H_format_abgr_s8 VK_FORMAT_A8B8G8R8_SINT_PACK32
#define H_format_abgr_u8 VK_FORMAT_A8B8G8R8_UINT_PACK32

//

#define H_colorspace_srgb_nonlinear VK_COLOR_SPACE_SRGB_NONLINEAR_KHR

//

typedef VkExtent2D H_extent_2d;
typedef VkExtent3D H_extent_3d;

typedef VkOffset2D H_offset_2d;
typedef VkOffset3D H_offset_3d;

typedef VkRect2D H_rect_2d;

typedef VkRect2D H_scissor;

typedef VkComponentMapping H_component_mapping;

//


/*#define H_create_struct_image(           \
    in_image_type,                         \
    in_format,                             \
    in_extent,                             \
    in_mip_levels,                         \
    in_array_layers,                       \
    in_samples,                            \
    in_tiling,                             \
    in_usage,                              \
    in_sharing_mode,                       \
    in_queue_family_index_count,           \
    in_queue_family_indices,               \
    in_initial_layout                      \
)                                        \
		( H_struct_image )                     \
		{                                      \
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, \
				NULL,                              \
				0,                                 \
				in_image_type,                     \
				in_format,                         \
				in_extent,                         \
				in_mip_levels,                     \
				in_array_layers,                   \
				in_samples,                        \
				in_tiling,                         \
				in_usage,                          \
				in_sharing_mode,                   \
				in_queue_family_index_count,       \
				in_queue_family_indices,           \
				in_initial_layout                  \
		}*/

typedef VkImageType H_image_type;
typedef VkImageTiling H_image_tiling;

#define H_image_tiling_optimal VK_IMAGE_TILING_OPTIMAL
#define H_image_tiling_linear VK_IMAGE_TILING_LINEAR

typedef VkSampleCountFlagBits H_sample_count_flag;
typedef VkImageUsageFlags H_image_usage_flags;
typedef VkImageLayout H_image_layout;

typedef VkImage H_image;
typedef VkImageCreateInfo H_struct_image;

H_image H_new_image(
	const H_image_type in_image_type,
	const H_format in_format,
	const H_extent_3d in_extent,
	const unsigned int in_mip_levels,
	const unsigned int in_array_layers,
	const H_sample_count_flag in_samples,
	const H_image_tiling in_tiling,
	const H_image_usage_flags in_usage,
	const H_sharing_mode in_sharing_mode,
	const unsigned int in_queue_family_index_count,
	const uint32_t* in_queue_family_indices,
	const H_image_layout in_initial_layout
)
{
    H_image temp_image = NULL;
	H_struct_image temp_struct_image = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};

	temp_struct_image.imageType = in_image_type;
	temp_struct_image.format = in_format;
	temp_struct_image.extent = in_extent;
	temp_struct_image.mipLevels = in_mip_levels;
	temp_struct_image.arrayLayers = in_array_layers;
	temp_struct_image.samples = in_samples;
	temp_struct_image.tiling = in_tiling;
	temp_struct_image.usage = in_usage;
	temp_struct_image.sharingMode = in_sharing_mode;
	temp_struct_image.queueFamilyIndexCount = in_queue_family_index_count;
	temp_struct_image.pQueueFamilyIndices = in_queue_family_indices;
	temp_struct_image.initialLayout = in_initial_layout;

	vkCreateImage(H_current_device,&temp_struct_image,NULL, &temp_image);
    return temp_image;
}

#define H_image_type_1d VK_IMAGE_TYPE_1D
#define H_image_type_2d VK_IMAGE_TYPE_2D
#define H_image_type_3d VK_IMAGE_TYPE_3D

#define H_image_view_type_1d VK_IMAGE_VIEW_TYPE_1D
#define H_image_view_type_2d VK_IMAGE_VIEW_TYPE_2D
#define H_image_view_type_3d VK_IMAGE_VIEW_TYPE_3D
#define H_image_view_type_cube VK_IMAGE_VIEW_TYPE_CUBE
#define H_image_view_type_1d_array VK_IMAGE_VIEW_TYPE_1D_ARRAY
#define H_image_view_type_2d_array VK_IMAGE_VIEW_TYPE_2D_ARRAY
#define H_image_view_type_3d_array VK_IMAGE_VIEW_TYPE_3D_ARRAY
#define H_image_view_type_cube_array VK_IMAGE_VIEW_TYPE_CUBE_ARRAY

H_memory_requirements H_get_memory_requirements_image( H_image in_image) {
    H_memory_requirements temp_requirements;
    vkGetImageMemoryRequirements(H_current_device, in_image, &temp_requirements);
    return temp_requirements;
}

H_memory _H_new_memory_image( H_struct_memory* in_info, H_image in_image) {
    H_memory temp_memory = NULL;
    H_CHECK(vkBindImageMemory( H_current_device, in_image, temp_memory, 0 ));
    return temp_memory;
}

#define H_new_memory_image( STRUCT_MEMORY, IMAGE ) _H_new_memory_image( &STRUCT_MEMORY, IMAGE )

typedef VkImageSubresourceRange H_image_subresource_range;
typedef VkImageLayout H_image_layout;
typedef VkImageTiling H_image_tiling;

#define H_image_layout_undefined VK_IMAGE_LAYOUT_UNDEFINED
#define H_image_layout_general VK_IMAGE_LAYOUT_GENERAL
#define H_image_layout_color_attachment_optimal VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
#define H_image_layout_depth_stencil_attachment_optimal VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
#define H_image_layout_depth_stencil_read_only_optimal VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
#define H_image_layout_shader_read_only_optimal VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
#define H_image_layout_transfer_src_optimal VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
#define H_image_layout_transfer_dst_optimal VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
#define H_image_layout_preinitialized VK_IMAGE_LAYOUT_PREINITIALIZED
#define H_image_layout_depth_read_only_stencil_attachment_optimal VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
#define H_image_layout_depth_attachment_stencil_read_only_optimal VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
#define H_image_layout_present VK_IMAGE_LAYOUT_PRESENT_SRC_KHR

#define H_image_usage_transfer_src VK_IMAGE_USAGE_TRANSFER_SRC_BIT
#define H_image_usage_transfer_dst VK_IMAGE_USAGE_TRANSFER_DST_BIT
#define H_image_usage_sampled VK_IMAGE_USAGE_SAMPLED_BIT
#define H_image_usage_storage VK_IMAGE_USAGE_STORAGE_BIT
#define H_image_usage_color_attachment VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
#define H_image_usage_depth_stencil_attachment VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
#define H_image_usage_transient_attachment VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT
#define H_image_usage_input_attachment VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT

typedef VkImageMemoryBarrier H_image_barrier;
#define H_create_image_barrier(             \
    in_srcAccessMask,                         \
    in_dstAccessMask,                         \
    in_oldLayout,                             \
    in_newLayout,                             \
    in_image,                                 \
    in_aspectMask,                            \
    in_baseMipLevel,                          \
    in_levelCount,                            \
    in_baseArrayLayer,                        \
    in_layerCount                             \
)                                           \
		( H_image_barrier )                       \
		{                                         \
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, \
				NULL,                                 \
				in_srcAccessMask,                     \
				in_dstAccessMask,                     \
				in_oldLayout,                         \
				in_newLayout,                         \
				VK_QUEUE_FAMILY_IGNORED,              \
				VK_QUEUE_FAMILY_IGNORED,              \
				in_image,                             \
			{                                       \
				in_aspectMask,                        \
					in_baseMipLevel,                    \
					in_levelCount,                      \
					in_baseArrayLayer,                  \
					in_layerCount                       \
			}                                       \
		}

typedef VkImageViewCreateInfo H_struct_image_view;
#define H_create_struct_image_view(           \
    in_image,                                   \
    in_view_type,                               \
    in_format,                                  \
    in_components,                              \
    in_subresource_range                        \
)                                             \
		( H_struct_image_view )                     \
		{                                           \
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, \
				NULL,                                   \
				0,                                      \
				in_image,                               \
				in_view_type,                           \
				in_format,                              \
				in_components,                          \
				in_subresource_range                    \
		}
typedef VkImageView H_image_view;

H_image_view _H_new_image_view( H_struct_image_view* in_info) {
    H_image_view temp_image_view = NULL;
    H_CHECK(vkCreateImageView( H_current_device, in_info, NULL, &temp_image_view ));
    return temp_image_view;
}

#define H_new_image_view(  STRUCT_IMAGE_VIEW ) _H_new_image_view(  &STRUCT_IMAGE_VIEW )

#define H_image_sample_1 VK_SAMPLE_COUNT_1_BIT
#define H_image_sample_2 VK_SAMPLE_COUNT_2_BIT
#define H_image_sample_4 VK_SAMPLE_COUNT_4_BIT
#define H_image_sample_8 VK_SAMPLE_COUNT_8_BIT
#define H_image_sample_16 VK_SAMPLE_COUNT_16_BIT
#define H_image_sample_32 VK_SAMPLE_COUNT_32_BIT
#define H_image_sample_64 VK_SAMPLE_COUNT_64_BIT

typedef VkPrimitiveTopology H_topology;
#define H_topology_tri VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
#define H_topology_line VK_PRIMITIVE_TOPOLOGY_LINE_LIST
#define H_topology_strip VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
#define H_topology_pixel VK_PRIMITIVE_TOPOLOGY_POINT_LIST

#define H_create_viewport( \
    in_x,                    \
    in_y,                    \
    in_width,                \
    in_height,               \
    in_deptH_min,            \
    in_deptH_max             \
)                          \
		( H_viewport )           \
		{                        \
			in_x,                  \
				in_y,                \
				in_width,            \
				in_height,           \
				in_deptH_min,        \
				in_deptH_max,        \
		}
typedef VkViewport H_viewport;

typedef VkFilter H_filter;
typedef VkSamplerMipmapMode H_mipmap_mode;
typedef VkSamplerAddressMode H_address_mode;
typedef VkCompareOp H_compare_op;
typedef VkBorderColor H_border_col;

typedef VkSampler H_sampler;
typedef VkSamplerCreateInfo H_struct_sampler;

H_sampler H_new_sampler
(
	const H_filter in_mag_filter,
	const H_filter in_min_filter,
	const H_mipmap_mode in_mipmap_mode,
	const H_address_mode in_address_mode_u,
	const H_address_mode in_address_mode_v,
	const H_address_mode in_address_mode_w,
	const float in_mip_lod_bias,
	const VkBool32 in_anisotropy_enable,
	const float in_max_anisotropy,
	const VkBool32 in_compare_enable,
	const H_compare_op in_compare_op,
	const float in_min_lod,
	const float in_max_lod,
	const H_border_col in_border_color,
	const VkBool32 in_unnormalized_coordinates
)
{
	H_sampler temp_sampler = NULL;
	H_struct_sampler temp_struct_sampler = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};

	temp_struct_sampler.magFilter = in_mag_filter;
	temp_struct_sampler.minFilter = in_min_filter;
	temp_struct_sampler.mipmapMode = in_mipmap_mode;
	temp_struct_sampler.addressModeU = in_address_mode_u;
	temp_struct_sampler.addressModeV = in_address_mode_v;
	temp_struct_sampler.addressModeW = in_address_mode_w;
	temp_struct_sampler.mipLodBias = in_mip_lod_bias;
	temp_struct_sampler.anisotropyEnable = in_anisotropy_enable;
	temp_struct_sampler.maxAnisotropy = in_max_anisotropy;
	temp_struct_sampler.compareEnable = in_compare_enable;
	temp_struct_sampler.compareOp = in_compare_op;
	temp_struct_sampler.minLod = in_min_lod;
	temp_struct_sampler.maxLod = in_max_lod;
	temp_struct_sampler.borderColor = in_border_color;
	temp_struct_sampler.unnormalizedCoordinates = in_unnormalized_coordinates;

	H_CHECK(vkCreateSampler( H_current_device, &temp_struct_sampler, NULL, &temp_sampler ));
	return temp_sampler;
}

typedef VkShaderModuleCreateInfo H_struct_shader_module;
#define H_create_struct_shader_module(           \
    in_code_size,                                  \
    in_code                                        \
)                                                \
		( H_struct_shader_module )                     \
		{                                              \
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, \
				NULL,                                      \
				0,                                         \
				in_code_size,                              \
				in_code                                    \
		}
typedef VkShaderModule H_shader_module;

H_shader_module _H_new_shader_module( H_struct_shader_module* in_info) {
    H_shader_module temp_image_view;
    H_CHECK(vkCreateShaderModule( H_current_device, in_info, NULL, &temp_image_view ));
    return temp_image_view;
}

#define H_new_shader_module( STRUCT_SHADER_MODULE ) _H_new_shader_module( &STRUCT_SHADER_MODULE )

typedef VkShaderStageFlagBits H_shader_stage;
#define H_shader_stage_vertex VK_SHADER_STAGE_VERTEX_BIT
#define H_shader_stage_geometry VK_SHADER_STAGE_GEOMETRY_BIT
#define H_shader_stage_fragment VK_SHADER_STAGE_FRAGMENT_BIT
#define H_shader_stage_compute VK_SHADER_STAGE_COMPUTE_BIT

#define H_create_vertex_binding_per_vertex(  \
    in_binding,                                \
    in_form_size                               \
)                                            \
		( H_vertex_binding )                       \
		{                                          \
			.binding = in_binding,                   \
			.stride = in_form_size,                  \
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX \
		}
#define H_create_vertex_binding_per_instance(  \
    in_binding,                                  \
    in_form_size                                 \
)                                              \
		( H_vertex_binding )                         \
		{                                            \
			.binding = in_binding,                     \
			.stride = in_form_size,                    \
			.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE \
		}
typedef VkVertexInputBindingDescription H_vertex_binding;

typedef VkVertexInputAttributeDescription H_vertex_attribute;

typedef VkPipelineCacheCreateInfo H_struct_pipeline_cache;
#define H_create_struct_pipeline_cache(           \
    in_initial_data_size,                           \
    in_initial_data                                 \
)                                                 \
		( H_struct_pipeline_cache )                     \
		{                                               \
			VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO, \
				NULL,                                       \
				0,                                          \
				in_initial_data_size,                       \
				in_initial_data                             \
		}
typedef VkPipelineCache H_pipeline_cache;

typedef VkPipelineShaderStageCreateInfo H_struct_pipeline_shader_stage;
#define H_create_struct_pipeline_shader_stage(           \
    in_stage,                                              \
    in_module,                                             \
    in_name,                                               \
    in_specialization_info                                 \
)                                                        \
		( H_struct_pipeline_shader_stage )                     \
		{                                                      \
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, \
				NULL,                                              \
				0,                                                 \
				in_stage,                                          \
				in_module,                                         \
				in_name,                                           \
				in_specialization_info                             \
		}

typedef VkPipelineVertexInputStateCreateInfo H_struct_pipeline_vertex;
#define H_create_struct_pipeline_vertex(                       \
    in_vertex_binding_description_count,                         \
    in_vertex_binding_descriptions,                              \
    in_vertex_attribute_description_count,                       \
    in_vertex_attribute_descriptions                             \
)                                                              \
		( H_struct_pipeline_vertex )                                 \
		{                                                            \
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, \
				NULL,                                                    \
				0,                                                       \
				in_vertex_binding_description_count,                     \
				in_vertex_binding_descriptions,                          \
				in_vertex_attribute_description_count,                   \
				in_vertex_attribute_descriptions                         \
		}

typedef VkPipelineInputAssemblyStateCreateInfo H_struct_pipeline_assembly;
#define H_create_struct_pipeline_assembly(                       \
    in_topology,                                                   \
    in_primitive_restart_enable                                    \
)                                                                \
		( H_struct_pipeline_assembly )                                 \
		{                                                              \
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, \
				NULL,                                                      \
				0,                                                         \
				in_topology,                                               \
				in_primitive_restart_enable                                \
		}

typedef VkPipelineTessellationStateCreateInfo H_struct_pipeline_tessellation;
#define H_create_struct_pipeline_tessellation(                 \
    in_patch_control_points                                      \
)                                                              \
		( H_struct_pipeline_tessellation )                           \
		{                                                            \
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO, \
				NULL,                                                    \
				0,                                                       \
				in_patch_control_points                                  \
		}

typedef VkPipelineViewportStateCreateInfo H_struct_pipeline_viewport;
#define H_create_struct_pipeline_viewport(                 \
    in_viewport_count,                                       \
    in_viewports,                                            \
    in_scissor_count,                                        \
    in_scissors                                              \
)                                                          \
		( H_struct_pipeline_viewport )                           \
		{                                                        \
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, \
				NULL,                                                \
				0,                                                   \
				in_viewport_count,                                   \
				in_viewports,                                        \
				in_scissor_count,                                    \
				in_scissors                                          \
		}

typedef VkPipelineRasterizationStateCreateInfo H_struct_pipeline_raster;
#define H_create_struct_pipeline_raster(                        \
    in_depth_clamp_enable,                                        \
    in_rasterizer_discard_enable,                                 \
    in_polygon_mode,                                              \
    in_cull_mode,                                                 \
    in_front_face,                                                \
    in_depth_bias_enable,                                         \
    in_depth_bias_constant_factor,                                \
    in_depth_bias_clamp,                                          \
    in_depth_bias_slope_factor,                                   \
    in_line_width                                                 \
)                                                               \
		( H_struct_pipeline_raster )                                  \
		{                                                             \
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, \
				NULL,                                                     \
				0,                                                        \
				in_depth_clamp_enable,                                    \
				in_rasterizer_discard_enable,                             \
				in_polygon_mode,                                          \
				in_cull_mode,                                             \
				in_front_face,                                            \
				in_depth_bias_enable,                                     \
				in_depth_bias_constant_factor,                            \
				in_depth_bias_clamp,                                      \
				in_depth_bias_slope_factor,                               \
				in_line_width                                             \
		}

typedef VkPipelineMultisampleStateCreateInfo H_struct_pipeline_multisample;
#define H_create_struct_pipeline_multisample(                 \
    in_rasterization_samples,                                   \
    in_sample_shading_enable,                                   \
    in_min_sample_shading,                                      \
    in_sample_mask,                                             \
    in_alpha_to_coverage_enable,                                \
    in_alpha_to_one_enable                                      \
)                                                             \
		( H_struct_pipeline_multisample )                           \
		{                                                           \
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, \
				NULL,                                                   \
				0,                                                      \
				in_rasterization_samples,                               \
				in_sample_shading_enable,                               \
				in_min_sample_shading,                                  \
				in_sample_mask,                                         \
				in_alpha_to_coverage_enable,                            \
				in_alpha_to_one_enable                                  \
		}

typedef VkPipelineDepthStencilStateCreateInfo H_struct_pipeline_depth_stencil;
#define H_create_struct_pipeline_depth_stencil(                 \
    in_depth_test_enable,                                         \
    in_depth_write_enable,                                        \
    in_depth_compare_op,                                          \
    in_depth_bounds_test_enable,                                  \
    in_stencil_test_enable,                                       \
    in_front,                                                     \
    in_back,                                                      \
    in_min_depth_bounds,                                          \
    in_max_depth_bounds                                           \
)                                                               \
		( H_struct_pipeline_depth_stencil )                           \
		{                                                             \
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, \
				NULL,                                                     \
				0,                                                        \
				in_depth_test_enable,                                     \
				in_depth_write_enable,                                    \
				in_depth_compare_op,                                      \
				in_depth_bounds_test_enable,                              \
				in_stencil_test_enable,                                   \
				in_front,                                                 \
				in_back,                                                  \
				in_min_depth_bounds,                                      \
				in_max_depth_bounds                                       \
		}

typedef VkPipelineColorBlendAttachmentState H_struct_pipeline_blend_mode;
#define H_create_struct_pipeline_blend_mode( \
    in_blend_enable,                           \
    in_src_color_blend_factor,                 \
    in_dst_color_blend_factor,                 \
    in_color_blend_op,                         \
    in_src_alpha_blend_factor,                 \
    in_dst_alpha_blend_factor,                 \
    in_alpha_blend_op,                         \
    in_color_write_mask                        \
)                                            \
		( H_struct_pipeline_blend_mode )           \
		{                                          \
			in_blend_enable,                         \
				in_src_color_blend_factor,             \
				in_dst_color_blend_factor,             \
				in_color_blend_op,                     \
				in_src_alpha_blend_factor,             \
				in_dst_alpha_blend_factor,             \
				in_alpha_blend_op,                     \
				in_color_write_mask,                   \
		}

H_struct_pipeline_blend_mode H_blend_mode_none;
H_struct_pipeline_blend_mode H_blend_mode_normal;
H_struct_pipeline_blend_mode H_blend_mode_constant;
H_struct_pipeline_blend_mode H_blend_mode_add;
H_struct_pipeline_blend_mode H_blend_mode_subtract;
H_struct_pipeline_blend_mode H_blend_mode_multiply;
H_struct_pipeline_blend_mode H_blend_mode_screen;
H_struct_pipeline_blend_mode H_blend_mode_overlay;

typedef VkPipelineColorBlendStateCreateInfo H_struct_pipeline_blend;
#define H_create_struct_pipeline_blend(                                                  \
    in_logic_op_enable,                                                                    \
    in_logic_op,                                                                           \
    in_attachment_count,                                                                   \
    in_attachments,                                                                        \
    in_blend_constant_r,                                                                   \
    in_blend_constant_g,                                                                   \
    in_blend_constant_b,                                                                   \
    in_blend_constant_a                                                                    \
)                                                                                        \
		( H_struct_pipeline_blend )                                                            \
		{                                                                                      \
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,                            \
				NULL,                                                                              \
				0,                                                                                 \
				in_logic_op_enable,                                                                \
				in_logic_op,                                                                       \
				in_attachment_count,                                                               \
				in_attachments,                                                                    \
			{                                                                                    \
				in_blend_constant_r, in_blend_constant_g, in_blend_constant_b, in_blend_constant_a \
			}                                                                                    \
		}

H_struct_pipeline_blend H_default_blend_none;
H_struct_pipeline_blend H_default_blend_normal;
H_struct_pipeline_blend H_default_blend_red;
H_struct_pipeline_blend H_default_blend_add;
H_struct_pipeline_blend H_default_blend_multiply;

typedef VkDynamicState H_dynamic_state;
typedef VkPipelineDynamicStateCreateInfo H_struct_pipeline_dynamic;
#define H_create_struct_pipeline_dynamic(                 \
    in_dynamic_state_count,                                 \
    in_dynamic_states                                       \
)                                                         \
		( H_struct_pipeline_dynamic )                           \
		{                                                       \
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, \
				NULL,                                               \
				0,                                                  \
				in_dynamic_state_count,                             \
				in_dynamic_states                                   \
		}

H_dynamic_state _H_dynamic_states_normal[] = {
    VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_BLEND_CONSTANTS, VK_DYNAMIC_STATE_LINE_WIDTH
};
H_struct_pipeline_dynamic H_dynamic_state_normal;

typedef VkPushConstantRange H_struct_push_constant_range;
#define H_create_struct_push_constant_range( \
    in_stage_flags,                            \
    in_offset,                                 \
    in_size                                    \
)                                            \
		( H_struct_push_constant_range )           \
		{                                          \
			in_stage_flags,                          \
				in_offset,                             \
				in_size                                \
		}

typedef VkPipelineLayoutCreateInfo H_struct_pipeline_layout;
#define H_create_struct_pipeline_layout(           \
    in_set_layout_count,                             \
    in_set_layouts,                                  \
    in_push_constant_range_count,                    \
    in_push_constant_ranges                          \
)                                                  \
		( H_struct_pipeline_layout )                     \
		{                                                \
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, \
				NULL,                                        \
				0,                                           \
				in_set_layout_count,                         \
				in_set_layouts,                              \
				in_push_constant_range_count,                \
				in_push_constant_ranges                      \
		}
typedef VkPipelineLayout H_pipeline_layout;

H_pipeline_layout _H_new_pipeline_layout( H_struct_pipeline_layout* in_info) {
    H_pipeline_layout temp_pipeline_layout = NULL;
    H_CHECK(vkCreatePipelineLayout( H_current_device, in_info, NULL, &temp_pipeline_layout ));
    return temp_pipeline_layout;
}

#define H_new_pipeline_layout( STRUCT_PIPELINE_LAYOUT ) _H_new_pipeline_layout( &STRUCT_PIPELINE_LAYOUT )

typedef VkGraphicsPipelineCreateInfo H_struct_render_pipeline;
#define H_create_struct_render_pipeline(             \
    in_stage_count,                                    \
    in_stages,                                         \
    in_vertex_input_state,                             \
    in_input_assembly_state,                           \
    in_tessellation_state,                             \
    in_viewport_state,                                 \
    in_rasterization_state,                            \
    in_multisample_state,                              \
    in_depth_stencil_state,                            \
    in_color_blend_state,                              \
    in_dynamic_state,                                  \
    in_layout,                                         \
    in_render_pass,                                    \
    in_subpass,                                        \
    in_base_pipeline_handle,                           \
    in_base_pipeline_index                             \
)                                                    \
		( H_struct_render_pipeline )                       \
		{                                                  \
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, \
				NULL,                                          \
				0,                                             \
				in_stage_count,                                \
				in_stages,                                     \
				in_vertex_input_state,                         \
				in_input_assembly_state,                       \
				in_tessellation_state,                         \
				in_viewport_state,                             \
				in_rasterization_state,                        \
				in_multisample_state,                          \
				in_depth_stencil_state,                        \
				in_color_blend_state,                          \
				in_dynamic_state,                              \
				in_layout,                                     \
				in_render_pass,                                \
				in_subpass,                                    \
				in_base_pipeline_handle,                       \
				in_base_pipeline_index                         \
		}

typedef VkComputePipelineCreateInfo H_struct_compute_pipeline;
#define H_create_struct_compute_pipeline(           \
    in_stage,                                         \
    in_layout,                                        \
    in_base_pipeline_handle,                          \
    in_base_pipeline_index                            \
)                                                   \
		( H_struct_compute_pipeline )                     \
		{                                                 \
			VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, \
				NULL,                                         \
				0,                                            \
				in_stage,                                     \
				in_layout,                                    \
				in_base_pipeline_handle,                      \
				in_base_pipeline_index                        \
		}

typedef VkPipeline H_pipeline;

H_pipeline _H_new_render_pipeline( H_struct_render_pipeline* in_info) {
    H_pipeline temp_pipeline = NULL;
    H_CHECK(vkCreateGraphicsPipelines( H_current_device, VK_NULL_HANDLE, 1U, in_info, NULL, &temp_pipeline ));
    return temp_pipeline;
}

#define H_new_render_pipeline( STRUCT_RENDER_PIPELINE ) _H_new_render_pipeline( &STRUCT_RENDER_PIPELINE )

H_pipeline _H_new_compute_pipeline( H_struct_compute_pipeline* in_info) {
    H_pipeline temp_pipeline = NULL;
    H_CHECK(vkCreateComputePipelines( H_current_device, VK_NULL_HANDLE, 1U, in_info, NULL, &temp_pipeline ));
    return temp_pipeline;
}

#define H_new_compute_pipeline( STRUCT_COMPUTE_PIPELINE ) _H_new_compute_pipeline( &STRUCT_COMPUTE_PIPELINE )

typedef VkPipelineStageFlags H_pipeline_stage;

#define H_pipeline_stage_color_attachment_output VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
#define H_pipeline_stage_fragment_shader VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
#define H_pipeline_stage_top_of_pipe VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
#define H_pipeline_stage_bottom_of_pipe VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
#define H_pipeline_stage_transfer VK_PIPELINE_STAGE_TRANSFER_BIT

/*typedef VkRenderPassCreateInfo H_struct_render_pass;
#define H_create_struct_render_pass(           \
    in_attachment_count,                         \
    in_attachments,                              \
    in_subpass_count,                            \
    in_subpasses,                                \
    in_dependency_count,                         \
    in_dependencies                              \
)                                              \
		( H_struct_render_pass )                     \
		{                                            \
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, \
				NULL,                                    \
				0,                                       \
				in_attachment_count,                     \
				in_attachments,                          \
				in_subpass_count,                        \
				in_subpasses,                            \
				in_dependency_count,                     \
				in_dependencies                          \
		}*/

typedef VkRenderPass H_render_pass;
typedef VkRenderPassCreateInfo H_struct_render_pass;
H_render_pass H_new_render_pass(
	unsigned int in_attachment_count,
	const VkAttachmentDescription* in_attachments,
	unsigned int in_subpass_count,
	const VkSubpassDescription* in_subpasses,
	unsigned int in_dependency_count,
	const VkSubpassDependency* in_dependencies
	) {
    H_render_pass temp_render_pass;
    H_struct_render_pass temp_struct_render_pass = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};

	temp_struct_render_pass.attachmentCount = in_attachment_count;
	temp_struct_render_pass.pAttachments = in_attachments;
	temp_struct_render_pass.subpassCount = in_subpass_count;
	temp_struct_render_pass.pSubpasses = in_subpasses;
	temp_struct_render_pass.dependencyCount = in_dependency_count;
	temp_struct_render_pass.pDependencies = in_dependencies;

    H_CHECK(vkCreateRenderPass( H_current_device, &temp_struct_render_pass, NULL, &temp_render_pass ));
    return temp_render_pass;
}

typedef VkRenderPassBeginInfo H_struct_begin_render_pass;
#define H_create_struct_begin_render_pass(    \
    in_render_pass,                             \
    in_framebuffer,                             \
    in_render_area,                             \
    in_clear_count,                             \
    in_clear_values                             \
)                                             \
		( H_struct_begin_render_pass )              \
		{                                           \
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, \
				NULL,                                   \
				in_render_pass,                         \
				in_framebuffer,                         \
				in_render_area,                         \
				in_clear_count,                         \
				in_clear_values                         \
		}

typedef VkFramebufferCreateInfo H_struct_frame;
#define H_create_struct_frame(                 \
    in_render_pass,                              \
    in_attachment_count,                         \
    in_attachments,                              \
    in_width,                                    \
    in_height,                                   \
    in_layers                                    \
)                                              \
		( H_struct_frame )                           \
		{                                            \
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, \
				NULL,                                    \
				0,                                       \
				in_render_pass,                          \
				in_attachment_count,                     \
				in_attachments,                          \
				in_width,                                \
				in_height,                               \
				in_layers                                \
		}
typedef VkFramebuffer H_frame;

H_frame _H_new_frame( H_struct_frame* in_info) {
    H_frame temp_framebuffer = NULL;
    H_CHECK(vkCreateFramebuffer( H_current_device, in_info, NULL, &temp_framebuffer ));
    return temp_framebuffer;
}

#define H_new_frame(  STRUCT_FRAME ) _H_new_frame(  &STRUCT_FRAME )

typedef VkAttachmentDescription H_attachment_description;
typedef VkAttachmentReference H_attachment_reference;
typedef VkSubpassDescription H_subpass_description;
typedef VkSubpassDependency H_subpass_dependency;

typedef VkDescriptorSetLayoutCreateInfo H_struct_descriptor_layout;
#define H_create_struct_descriptor_layout(               \
    in_binding_count,                                      \
    in_bindings                                            \
)                                                        \
		( H_struct_descriptor_layout )                         \
		{                                                      \
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, \
				NULL,                                              \
				0,                                                 \
				in_binding_count,                                  \
				in_bindings                                        \
		}
typedef VkDescriptorSetLayout H_descriptor_layout;

H_descriptor_layout _H_new_descriptor_layout( H_struct_descriptor_layout* in_info) {
    H_descriptor_layout temp_descriptor;
    H_CHECK(vkCreateDescriptorSetLayout( H_current_device, in_info, NULL, &temp_descriptor ));
    return temp_descriptor;
}

#define H_new_descriptor_layout( STRUCT_DESCRIPTOR_LAYOUT ) _H_new_descriptor_layout( &STRUCT_DESCRIPTOR_LAYOUT )

typedef VkDescriptorSetLayoutBinding H_descriptor_layout_binding;

typedef VkDescriptorPoolCreateInfo H_struct_descriptor_pool;
#define H_create_struct_descriptor_pool(           \
    in_max_sets,                                     \
    in_pool_size_count,                              \
    in_pool_sizes                                    \
)                                                  \
		( H_struct_descriptor_pool )                     \
		{                                                \
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, \
				NULL,                                        \
				0,                                           \
				in_max_sets,                                 \
				in_pool_size_count,                          \
				in_pool_sizes                                \
		}
typedef VkDescriptorPool H_descriptor_pool;

typedef VkDescriptorPoolSize H_descriptor_pool_size;

H_descriptor_pool _H_new_descriptor_pool( H_struct_descriptor_pool* in_info) {
    H_descriptor_pool temp_descriptor_pool = NULL;
    H_CHECK(vkCreateDescriptorPool( H_current_device, in_info, NULL, &temp_descriptor_pool ));
    return temp_descriptor_pool;
}

#define H_new_descriptor_pool( STRUCT_DESCRIPTOR_POOL ) _H_new_descriptor_pool( &STRUCT_DESCRIPTOR_POOL )

typedef VkDescriptorSetAllocateInfo H_struct_descriptor;
#define H_create_struct_descriptor(                 \
    in_descriptor_pool,                               \
    in_descriptor_set_count,                          \
    in_set_layouts                                    \
)                                                   \
		( H_struct_descriptor )                           \
		{                                                 \
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, \
				NULL,                                         \
				in_descriptor_pool,                           \
				in_descriptor_set_count,                      \
				in_set_layouts                                \
		}
typedef VkDescriptorSet H_descriptor;

typedef VkWriteDescriptorSet H_set_descriptor_write;
typedef VkCopyDescriptorSet H_set_descriptor_copy;

typedef VkDescriptorType H_descriptor_type;
#define H_descriptor_type_image VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
#define H_descriptor_type_storage VK_DESCRIPTOR_TYPE_STORAGE_BUFFER

H_descriptor _H_new_descriptor( H_struct_descriptor* in_info) {
    H_descriptor temp_descriptor = NULL;
    H_CHECK(vkAllocateDescriptorSets( H_current_device, in_info, &temp_descriptor ));
    return temp_descriptor;
}

#define H_new_descriptor( STRUCT_DESCRIPTOR ) _H_new_descriptor( &STRUCT_DESCRIPTOR )

void H_update_descriptor_set_storage(unsigned int in_binding,  H_descriptor in_descriptor,
                                     H_buffer in_buffer, unsigned long long int in_size) {
    VkDescriptorBufferInfo buffer_info = {
        .offset = 0,
        .buffer = in_buffer,
        .range = in_size
    };

    VkWriteDescriptorSet descriptor_write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = in_descriptor,
        .dstBinding = in_binding,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .pBufferInfo = &buffer_info
    };

    vkUpdateDescriptorSets(H_current_device, 1, &descriptor_write, 0, NULL);
}

void H_update_descriptor_set_image(unsigned int in_binding,  H_descriptor in_descriptor,
                                   H_sampler in_sampler, H_image_view in_image_view) {
    VkDescriptorImageInfo image_info = {
        .sampler = in_sampler,
        .imageView = in_image_view,
        .imageLayout = H_image_layout_shader_read_only_optimal
    };

    VkWriteDescriptorSet descriptor_write = {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = in_descriptor,
        .dstBinding = in_binding,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &image_info
    };

    vkUpdateDescriptorSets(H_current_device, 1, &descriptor_write, 0, NULL);
}

typedef VkSemaphoreCreateInfo H_struct_semaphore;
#define H_create_struct_semaphore()          \
		( H_struct_semaphore )                     \
		{                                          \
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, \
				0                                      \
		}
typedef VkSemaphore H_semaphore;

H_semaphore _H_new_semaphore( ) {
    H_semaphore temp_semaphore = NULL;
	const H_struct_semaphore temp_struct_semaphore = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    H_CHECK(vkCreateSemaphore( H_current_device, &temp_struct_semaphore, NULL, &temp_semaphore ));
    return temp_semaphore;
}

#define H_new_semaphore( ) _H_new_semaphore( )

typedef VkFenceCreateInfo H_struct_fence;
#define H_create_struct_fence()          \
		( H_struct_fence )                     \
		{                                      \
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, \
				NULL,                              \
				VK_FENCE_CREATE_SIGNALED_BIT       \
		}
typedef VkFence H_fence;

H_fence _H_new_fence() {
    H_fence temp_fence = NULL;
    const H_struct_fence temp_struct_fence = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,.flags = VK_FENCE_CREATE_SIGNALED_BIT};
    H_CHECK(vkCreateFence( H_current_device, &temp_struct_fence, NULL, &temp_fence ));
    return temp_fence;
}

#define H_new_fence( ) _H_new_fence( )

void _H_wait_fence( H_fence* in_fence) {
    H_CHECK(vkWaitForFences( H_current_device, 1, in_fence, VK_TRUE, UINT64_MAX ));
}

#define H_wait_fence( FENCE ) _H_wait_fence(  &FENCE )

void _H_reset_fence( H_fence* in_fence) {
    H_CHECK(vkResetFences( H_current_device, 1, in_fence ));
}

#define H_reset_fence(  FENCE ) _H_reset_fence(  &FENCE )

typedef VkEventCreateInfo H_struct_event;
#define H_create_struct_event()          \
		( H_struct_event )                     \
		{                                      \
			VK_STRUCTURE_TYPE_EVENT_CREATE_INFO, \
				NULL,                              \
				0                                  \
		}
typedef VkEvent H_event;

H_event _H_new_event( H_struct_event* in_info) {
    H_event temp_event = NULL;
    H_CHECK(vkCreateEvent( H_current_device, in_info, NULL, &temp_event ));
    return temp_event;
}

#define H_new_event(  STRUCT_EVENT ) _H_new_event(  &STRUCT_EVENT )

typedef VkQueryPoolCreateInfo H_struct_query_pool;
#define H_create_struct_query_pool(           \
    in_query_type,                              \
    in_query_count,                             \
    in_pipeline_statistics                      \
)                                             \
		( H_struct_query_pool )                     \
		{                                           \
			VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO, \
				NULL,                                   \
				0,                                      \
				in_query_type,                          \
				in_query_count,                         \
				in_pipeline_statistics                  \
		}
typedef VkQueryPool H_query_pool;

typedef VkSwapchainCreateInfoKHR H_struct_swapchain;
#define H_create_struct_swapchain(               \
    in_surface,                                    \
    in_min_image_count,                            \
    in_image_format,                               \
    in_image_color_space,                          \
    in_image_extent,                               \
    in_image_array_layers,                         \
    in_image_usage,                                \
    in_image_sharing_mode,                         \
    in_queue_family_index_count,                   \
    in_queue_family_indices,                       \
    in_pre_transform,                              \
    in_composite_alpha,                            \
    in_present_mode,                               \
    in_clipped,                                    \
    in_old_swapchain                               \
)                                                \
		( H_struct_swapchain )                         \
		{                                              \
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, \
				NULL,                                      \
				0,                                         \
				in_surface,                                \
				in_min_image_count,                        \
				in_image_format,                           \
				in_image_color_space,                      \
				in_image_extent,                           \
				in_image_array_layers,                     \
				in_image_usage,                            \
				in_image_sharing_mode,                     \
				in_queue_family_index_count,               \
				in_queue_family_indices,                   \
				in_pre_transform,                          \
				in_composite_alpha,                        \
				in_present_mode,                           \
				in_clipped,                                \
				in_old_swapchain                           \
		}
typedef VkSwapchainKHR H_swapchain;

H_swapchain _H_new_swapchain( H_struct_swapchain* in_info) {
    H_swapchain temp_swapchain = NULL;
    H_CHECK(vkCreateSwapchainKHR( H_current_device, in_info, NULL, &temp_swapchain ));
    return temp_swapchain;
}

#define H_new_swapchain(  STRUCT_SWAPCHAIN ) _H_new_swapchain(  &STRUCT_SWAPCHAIN )

unsigned int H_get_swapchain_images( H_swapchain in_swapchain, H_image* out_image) {
    static unsigned int images_count = 0;
    H_CHECK(vkGetSwapchainImagesKHR( H_current_device, in_swapchain, &images_count, out_image ));
    return images_count;
}



typedef VkSubmitInfo H_struct_submit;
#define H_create_struct_submit(    \
    in_wait_semaphore_count,         \
    in_wait_semaphores,              \
    in_wait_dst_stage_mask,          \
    in_command_buffer_count,         \
    in_command_buffers,              \
    in_signal_semaphore_count,       \
    in_signal_semaphores             \
)                                  \
		( H_struct_submit )              \
		{                                \
			VK_STRUCTURE_TYPE_SUBMIT_INFO, \
				NULL,                        \
				in_wait_semaphore_count,     \
				in_wait_semaphores,          \
				in_wait_dst_stage_mask,      \
				in_command_buffer_count,     \
				in_command_buffers,          \
				in_signal_semaphore_count,   \
				in_signal_semaphores         \
		}

void _H_submit_queue(H_queue in_queue, H_struct_submit* in_struct_submit, H_fence in_fence) {
    H_CHECK(vkQueueSubmit( in_queue, 1, in_struct_submit, in_fence ));
}

#define H_submit_queue( QUEUE, STRUCT_SUBMIT, FENCE ) _H_submit_queue( QUEUE, &STRUCT_SUBMIT, FENCE )

typedef VkPresentInfoKHR H_struct_present;
#define H_create_struct_present(        \
    in_wait_semaphore_count,              \
    in_wait_semaphores,                   \
    in_swapchain_count,                   \
    in_swapchains,                        \
    in_image_indices                      \
)                                       \
		( H_struct_present )                  \
		{                                     \
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, \
				NULL,                             \
				in_wait_semaphore_count,          \
				in_wait_semaphores,               \
				in_swapchain_count,               \
				in_swapchains,                    \
				in_image_indices,                 \
				0                                 \
		}

H_result _H_present(H_queue in_queue, H_struct_present* in_struct_present) {
    return vkQueuePresentKHR(in_queue, in_struct_present);
}

#define H_present( QUEUE, STRUCT_PRESENT ) _H_present( QUEUE, &STRUCT_PRESENT )

//

void H_init() {
    volkInitialize();
    // other globals
    H_blend_mode_none = H_create_struct_pipeline_blend_mode(
        VK_FALSE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_normal = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_SRC_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_constant = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_CONSTANT_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_add = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_subtract = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_REVERSE_SUBTRACT,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_REVERSE_SUBTRACT,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_multiply = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_DST_COLOR,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_DST_ALPHA,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_screen = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_blend_mode_overlay = H_create_struct_pipeline_blend_mode(
        VK_TRUE,
        VK_BLEND_FACTOR_SRC_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        VK_BLEND_OP_ADD,
        VK_BLEND_FACTOR_SRC_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_OP_ADD,
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    );

    H_default_blend_none = H_create_struct_pipeline_blend(no, 0, 1, &( H_blend_mode_none ), 1., 1., 1., 1.);
    H_default_blend_normal = H_create_struct_pipeline_blend(no, 0, 1, &( H_blend_mode_normal ), 1., 1., 1., 1.);
    H_default_blend_red = H_create_struct_pipeline_blend(no, 0, 1, &( H_blend_mode_constant ), 1., 0., 0., 1.);
    H_default_blend_add = H_create_struct_pipeline_blend(no, 0, 1, &( H_blend_mode_add ), 1., 1., 1., 1.);
    H_default_blend_multiply = H_create_struct_pipeline_blend(no, 0, 1, &( H_blend_mode_multiply ), 1., 1., 1., 1.);

    H_dynamic_state_normal = H_create_struct_pipeline_dynamic(2, _H_dynamic_states_normal);
}

#endif
