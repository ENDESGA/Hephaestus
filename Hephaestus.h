// // // // // // //
// > hephaestus _
// -------
// c7h16-conforming vulkan abstraction
// requires: Volk, (Vulkan + vk_video)
// @ENDESGA 2023
// // // // // // //

#pragma once
#ifndef HEPHAESTUS_H
	#define HEPHAESTUS_H

	#ifndef debug
		#define VK_NO_PROTOTYPES
		#define VK_NO_DEBUG
		#define VK_NO_FILESYSTEM
	#endif

	#include <Volk/volk.c>

//

	#define H_create_version( major, minor, patch ) VK_MAKE_API_VERSION( 0, major, minor, patch )
/*
	#define H_NARGS( ... ) H_NARGS_( __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1 )
	#define H_NARGS_( _1, _2, _3, _4, _5, _6, _7, N, ... ) N

	#define H_CONCAT_( A, B ) A##B
	#define H_CONCAT( A, B ) H_CONCAT_( A, B )

	#define H_APPLY1( X ) ( X )
	#define H_APPLY2( X, ... ) ( X ) | H_APPLY1( __VA_ARGS__ )
	#define H_APPLY3( X, ... ) ( X ) | H_APPLY2( __VA_ARGS__ )
	#define H_APPLY4( X, ... ) ( X ) | H_APPLY3( __VA_ARGS__ )
	#define H_APPLY5( X, ... ) ( X ) | H_APPLY4( __VA_ARGS__ )
	#define H_APPLY6( X, ... ) ( X ) | H_APPLY5( __VA_ARGS__ )
	#define H_APPLY7( X, ... ) ( X ) | H_APPLY6( __VA_ARGS__ )

	#define H_set_flags( ... ) H_CONCAT( H_APPLY, H_NARGS( __VA_ARGS__ ) )( __VA_ARGS__ )
*/
//

typedef VkLayerProperties H_layer_properties;

//

typedef VkApplicationInfo H_info_app;
	#define H_create_info_app(                                                 \
		in_app_name,                                                             \
		in_app_version,                                                          \
		in_engine_name,                                                          \
		in_engine_version,                                                       \
		in_api_version                                                           \
	)                                                                          \
		( H_info_app )                                                           \
		{                                                                        \
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,                           \
			.pApplicationName = in_app_name, .applicationVersion = in_app_version, \
			.pEngineName = in_engine_name, .engineVersion = in_engine_version,     \
			.apiVersion = in_api_version,                                          \
		}

//

inline H_info_app H_new_info( const char* in_name, unsigned int in_version )
{
	volkInitialize();
	return H_create_info_app( in_name, H_create_version( in_version, 0, 0 ), "hept", H_create_version( 0, 0, 1 ), H_create_version( 1, 0, 0 ) );
}

//

unsigned int H_get_debug_layers( H_layer_properties* in_layers )
{
	static unsigned int debug_layer_count = 0;
	vkEnumerateInstanceLayerProperties( &debug_layer_count, in_layers );
	return debug_layer_count;
}

//

typedef VkInstance H_instance;
typedef VkInstanceCreateInfo H_info_instance;
	#define H_create_info_instance(                                           \
		in_app_info,                                                            \
		in_layer_count,                                                         \
		in_layer_names,                                                         \
		in_extension_count,                                                     \
		in_extension_names                                                      \
	)                                                                         \
		( H_info_instance )                                                     \
		{                                                                       \
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,                      \
			.pApplicationInfo = in_app_info, .enabledLayerCount = in_layer_count, \
			.ppEnabledLayerNames = in_layer_names,                                \
			.enabledExtensionCount = in_extension_count,                          \
			.ppEnabledExtensionNames = in_extension_names,                        \
		}

H_instance H_new_instance( H_info_instance in_info )
{
	H_instance temp_instance;
	vkCreateInstance( &( in_info ), NULL, &( temp_instance ) );
	volkLoadInstanceOnly( temp_instance );
	return temp_instance;
}

//

typedef VkSurfaceKHR H_surface;
typedef VkSurfaceCapabilitiesKHR H_surface_capabilities;
typedef VkSurfaceFormatKHR H_surface_format;

//

typedef VkPhysicalDevice H_physical_device;
typedef VkPhysicalDeviceProperties H_physical_device_properties;
typedef VkPhysicalDeviceMemoryProperties H_physical_device_memory_properties;
typedef VkPhysicalDeviceFeatures H_physical_device_features;
typedef VkDevice H_device;
typedef VkDeviceCreateInfo H_info_device;
	#define H_create_info_device(                              \
		in_queue_create_info_count,                              \
		in_queue_create_infos,                                   \
		in_enabled_layer_count,                                  \
		in_enabled_layer_names,                                  \
		in_enabled_extension_count,                              \
		in_enabled_extension_names,                              \
		in_enabled_features                                      \
	)                                                          \
		( H_info_device )                                        \
		{                                                        \
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,         \
			.queueCreateInfoCount = in_queue_create_info_count,    \
			.pQueueCreateInfos = in_queue_create_infos,            \
			.enabledLayerCount = in_enabled_layer_count,           \
			.ppEnabledLayerNames = in_enabled_layer_names,         \
			.enabledExtensionCount = in_enabled_extension_count,   \
			.ppEnabledExtensionNames = in_enabled_extension_names, \
			.pEnabledFeatures = in_enabled_features,               \
		}

unsigned int H_get_physical_devices( H_instance in_instance, H_physical_device* out_physical_devices )
{
	static unsigned int physical_device_count = 0;
	vkEnumeratePhysicalDevices( in_instance, &physical_device_count, out_physical_devices );
	return physical_device_count;
}

H_physical_device_features H_get_physical_device_features( H_physical_device in_physical_device )
{
	static H_physical_device_features features;
	vkGetPhysicalDeviceFeatures( in_physical_device, &features );
	return features;
}

typedef VkQueueFamilyProperties H_physical_device_queue_properties;

unsigned int H_get_physical_device_queue_properties( H_physical_device in_physical_device, H_physical_device_queue_properties* out_properties )
{
	static unsigned int queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( in_physical_device, &queue_family_count, out_properties );
	return queue_family_count;
}

typedef VkDeviceQueueCreateInfo H_info_device_queue;
	#define H_create_info_device_queue(                                          \
		in_queue_family_index,                                                     \
		in_queue_count,                                                            \
		in_queue_priorities                                                        \
	)                                                                            \
		( H_info_device_queue )                                                    \
		{                                                                          \
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                     \
			.queueFamilyIndex = in_queue_family_index, .queueCount = in_queue_count, \
			.pQueuePriorities = in_queue_priorities,                                 \
		}

H_device H_new_device( H_physical_device in_physical_device, H_info_device in_info )
{
	H_device temp_device;
	vkCreateDevice( in_physical_device, &in_info, NULL, &temp_device );
	volkLoadDevice( temp_device );
	return temp_device;
}

//

typedef VkBuffer H_buffer;
typedef VkBufferUsageFlags H_buffer_usage;
	#define H_buffer_usage_vertex VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
	#define H_buffer_usage_index VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	#define H_buffer_usage_storage VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
	#define H_buffer_usage_indirect VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
	#define H_buffer_usage_uniform VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
	#define H_buffer_usage_transfer_dst VK_BUFFER_USAGE_TRANSFER_DST_BIT
typedef VkBufferCreateInfo H_info_buffer;
	#define H_create_info_buffer(                      \
		in_size,                                         \
		in_usage,                                        \
		in_sharing_mode                                  \
	)                                                  \
		( H_info_buffer )                                \
		{                                                \
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, \
			.size = in_size,                               \
			.usage = in_usage,                             \
			.sharingMode = in_sharing_mode                 \
		}
H_buffer H_new_buffer( H_device in_device, H_info_buffer in_info )
{
	static H_buffer temp_buffer = NULL;
	vkCreateBuffer( in_device, &in_info, NULL, &temp_buffer );
	return temp_buffer;
}

typedef VkDeviceMemory H_memory;
typedef VkMemoryRequirements H_memory_requirements;
typedef VkMemoryPropertyFlags H_memory_properties;

	#define H_memory_property_host_visible VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	#define H_memory_property_host_coherent VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	#define H_memory_property_host_cached VK_MEMORY_PROPERTY_HOST_CACHED_BIT
	#define H_memory_property_device_local VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	#define H_memory_property_protected VK_MEMORY_PROPERTY_PROTECTED_BIT
	#define H_memory_property_lazily_allocated VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT

unsigned int H_find_mem( H_physical_device in_physical_device, unsigned int in_type_filter, H_memory_properties in_properties )
{
	H_physical_device_memory_properties memory_properties;
	vkGetPhysicalDeviceMemoryProperties( in_physical_device, &memory_properties );

	for( unsigned int i = 0; i < memory_properties.memoryTypeCount; i++ )
	{
		if( ( in_type_filter & ( 1 << i ) ) && ( memory_properties.memoryTypes[ i ].propertyFlags & in_properties ) == in_properties )
		{
			return i;
		}
	}
	return 0;
}

H_memory_requirements H_get_memory_requirements_buffer( H_device in_device, H_buffer in_buffer )
{
	static H_memory_requirements temp_requirements;
	vkGetBufferMemoryRequirements( in_device, in_buffer, &temp_requirements );
	return temp_requirements;
}

typedef VkMemoryAllocateInfo H_info_memory;
	#define H_create_info_memory(                        \
		in_size,                                           \
		in_memory_index                                    \
	)                                                    \
		( H_info_memory )                                  \
		{                                                  \
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, \
			.allocationSize = in_size,                       \
			.memoryTypeIndex = in_memory_index               \
		}
H_memory H_new_memory_buffer( H_device in_device, H_info_memory in_info, H_buffer in_buffer )
{
	static H_memory temp_memory = NULL;
	vkAllocateMemory( in_device, &in_info, NULL, &temp_memory );
	vkBindBufferMemory( in_device, in_buffer, temp_memory, 0 );
	return temp_memory;
}

//

typedef VkQueue H_queue;

H_queue H_get_queue( H_device in_device, uint32_t family_index, uint32_t queue_index )
{
	H_queue temp_queue;
	vkGetDeviceQueue( in_device, family_index, queue_index, &temp_queue );
	return temp_queue;
}

//

	#define H_present_mode_vsync_off VK_PRESENT_MODE_IMMEDIATE_KHR
	#define H_present_mode_vsync_on VK_PRESENT_MODE_FIFO_KHR
	#define H_present_mode_vsync_optimal VK_PRESENT_MODE_MAILBOX_KHR

typedef VkPresentModeKHR H_present_mode;

//

typedef VkSwapchainKHR H_swapchain;
typedef VkSwapchainCreateInfoKHR H_info_swapchain;
	#define H_create_info_swapchain(                                               \
		in_surface,                                                                  \
		in_min_image_count,                                                          \
		in_image_format,                                                             \
		in_image_color_space,                                                        \
		in_image_extent,                                                             \
		in_image_array_layers,                                                       \
		in_image_usage,                                                              \
		in_image_sharing_mode,                                                       \
		in_queue_family_index_count,                                                 \
		in_queue_family_indices,                                                     \
		in_pre_transform,                                                            \
		in_composite_alpha,                                                          \
		in_present_mode,                                                             \
		in_clipped,                                                                  \
		in_old_swapchain                                                             \
	)                                                                              \
		( H_info_swapchain )                                                         \
		{                                                                            \
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,                      \
			.surface = in_surface, .minImageCount = in_min_image_count,                \
			.imageFormat = in_image_format, .imageColorSpace = in_image_color_space,   \
			.imageExtent = in_image_extent, .imageArrayLayers = in_image_array_layers, \
			.imageUsage = in_image_usage, .imageSharingMode = in_image_sharing_mode,   \
			.queueFamilyIndexCount = in_queue_family_index_count,                      \
			.pQueueFamilyIndices = in_queue_family_indices,                            \
			.preTransform = in_pre_transform, .compositeAlpha = in_composite_alpha,    \
			.presentMode = in_present_mode, .clipped = in_clipped,                     \
			.oldSwapchain = in_old_swapchain,                                          \
		}

H_swapchain H_new_swapchain( H_device in_device, H_info_swapchain in_info )
{
	H_swapchain temp_swapchain;
	vkCreateSwapchainKHR( in_device, &in_info, NULL, &temp_swapchain );
	return temp_swapchain;
}

//

typedef VkFormat H_format;

typedef VkExtent2D H_extent;
typedef VkExtent3D H_extent_3d;

typedef VkOffset2D H_offset;
typedef VkOffset3D H_offset_3d;

typedef VkComponentMapping H_component_mapping;

//

typedef VkSampler H_sampler;
typedef VkSamplerCreateInfo H_info_sampler;
	#define H_create_info_sampler(                             \
		in_mag_filter,                                           \
		in_min_filter,                                           \
		in_mipmap_mode,                                          \
		in_mode_U,                                               \
		in_mode_V,                                               \
		in_mode_W,                                               \
		in_mip_lod_bias,                                         \
		in_anisotropy_enable,                                    \
		in_max_anisotropy,                                       \
		in_compare_enable,                                       \
		in_compare_op,                                           \
		in_min_lod,                                              \
		in_max_lod,                                              \
		in_border_color,                                         \
		in_unnormalized_coordinates                              \
	)                                                          \
		( H_info_sampler )                                       \
		{                                                        \
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,        \
			.magFilter = in_mag_filter,                            \
			.minFilter = in_min_filter,                            \
			.mipmapMode = in_mipmap_mode,                          \
			.addressModeU = in_mode_U,                             \
			.addressModeV = in_mode_V,                             \
			.addressModeW = in_mode_W,                             \
			.mipLodBias = in_mip_lod_bias,                         \
			.anisotropyEnable = in_anisotropy_enable,              \
			.maxAnisotropy = in_max_anisotropy,                    \
			.compareEnable = in_compare_enable,                    \
			.compareOp = in_compare_op,                            \
			.minLod = in_min_lod,                                  \
			.maxLod = in_max_lod,                                  \
			.borderColor = in_border_color,                        \
			.unnormalizedCoordinates = in_unnormalized_coordinates \
		}

H_sampler H_new_sampler( H_device in_device, H_info_sampler in_info )
{
	H_sampler temp_sampler;
	vkCreateSampler( in_device, &in_info, NULL, &temp_sampler );
	return temp_sampler;
}

//

	#define H_image_usage_transfer_src VK_IMAGE_USAGE_TRANSFER_SRC_BIT
	#define H_image_usage_transfer_dst VK_IMAGE_USAGE_TRANSFER_DST_BIT
	#define H_image_usage_sampled VK_IMAGE_USAGE_SAMPLED_BIT
	#define H_image_usage_storage VK_IMAGE_USAGE_STORAGE_BIT
	#define H_image_usage_color_attachment VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
	#define H_image_usage_depth_stencil_attachment VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
	#define H_image_usage_transient_attachment VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT
	#define H_image_usage_input_attachment VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT

typedef VkImage H_image;
typedef VkImageSubresourceRange H_image_subresource_range;
typedef VkImageLayout H_image_layout;
typedef VkImageTiling H_image_tiling;
typedef VkImageCreateInfo H_info_image;
	#define H_create_info_image(                      \
		in_image_type,                                  \
		in_extent,                                      \
		in_mip_levels,                                  \
		in_array_layers,                                \
		in_format,                                      \
		in_tiling,                                      \
		in_initial_layout,                              \
		in_usage,                                       \
		in_sharing_mode,                                \
		in_samples                                      \
	)                                                 \
		( H_info_image )                                \
		{                                               \
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, \
			.imageType = in_image_type,                   \
			.extent = in_extent,                          \
			.mipLevels = in_mip_levels,                   \
			.arrayLayers = in_array_layers,               \
			.format = in_format,                          \
			.tiling = in_tiling,                          \
			.initialLayout = in_initial_layout,           \
			.usage = in_usage,                            \
			.sharingMode = in_sharing_mode,               \
			.samples = in_samples,                        \
		}

H_image H_new_image( H_device in_device, H_info_image in_info )
{
	H_image temp_image;
	vkCreateImage( in_device, &in_info, NULL, &temp_image );
	return temp_image;
}

H_memory_requirements H_get_memory_requirements_image( H_device in_device, H_image in_image )
{
	static H_memory_requirements temp_requirements;
	vkGetImageMemoryRequirements( in_device, in_image, &temp_requirements );
	return temp_requirements;
}

H_memory H_new_memory_image( H_device in_device, H_info_memory in_info, H_image in_image )
{
	static H_memory temp_memory = NULL;
	vkAllocateMemory( in_device, &in_info, NULL, &temp_memory );
	vkBindImageMemory( in_device, in_image, temp_memory, 0 );
	return temp_memory;
}

typedef VkImageLayout H_image_layout;
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
	#define H_image_layout_present_src_KHR VK_IMAGE_LAYOUT_PRESENT_SRC_KHR

typedef VkImageMemoryBarrier H_image_barrier;
	#define H_create_image_barrier(                       \
		in_image,                                          \
		in_oldLayout,                                      \
		in_newLayout,                                      \
		in_srcAccessMask,                                  \
		in_dstAccessMask,                                  \
		in_aspectMask,                                     \
		in_baseMipLevel,                                   \
		in_levelCount,                                     \
		in_baseArrayLayer,                                 \
		in_layerCount                                      \
	)                                                    \
		( H_image_barrier )                                 \
		{                                                  \
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, \
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,  \
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,  \
			.image = in_image,                               \
			.oldLayout = in_oldLayout,                       \
			.newLayout = in_newLayout,                       \
			.srcAccessMask = in_srcAccessMask,               \
			.dstAccessMask = in_dstAccessMask,               \
			.subresourceRange = {                            \
				.aspectMask = in_aspectMask,                   \
				.baseMipLevel = in_baseMipLevel,               \
				.levelCount = in_levelCount,                   \
				.baseArrayLayer = in_baseArrayLayer,           \
				.layerCount = in_layerCount                    \
			}                                                \
		}

typedef VkImageView H_image_view;
typedef VkImageViewCreateInfo H_info_image_view;
	#define H_create_info_image_view(                      \
		in_image,                                            \
		in_view_type,                                        \
		in_format,                                           \
		in_components,                                       \
		in_subresource_range                                 \
	)                                                      \
		( H_info_image_view )                                \
		{                                                    \
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, \
			.image = in_image,                                 \
			.viewType = in_view_type,                          \
			.format = in_format,                               \
			.components = in_components,                       \
			.subresourceRange = in_subresource_range,          \
		}

H_image_view H_new_image_view( H_device in_device, H_info_image_view in_info )
{
	H_image_view temp_image_view;
	vkCreateImageView( in_device, &in_info, NULL, &temp_image_view );
	return temp_image_view;
}

//

typedef VkShaderModule H_shader_module;
typedef VkShaderModuleCreateInfo H_info_shader_module;
	#define H_create_info_shader_module(                      \
		in_code,                                                \
		in_code_size                                            \
	)                                                         \
		( H_info_shader_module )                                \
		{                                                       \
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, \
			.pCode = ( unsigned int* )in_code,                    \
			.codeSize = in_code_size,                             \
		}

H_shader_module H_new_shader_module( H_device in_device, H_info_shader_module in_info )
{
	H_shader_module temp_image_view;
	vkCreateShaderModule( in_device, &in_info, NULL, &temp_image_view );
	return temp_image_view;
}

typedef VkShaderStageFlagBits H_shader_stage;
	#define H_shader_stage_vertex VK_SHADER_STAGE_VERTEX_BIT
	#define H_shader_stage_geometry VK_SHADER_STAGE_GEOMETRY_BIT
	#define H_shader_stage_fragment VK_SHADER_STAGE_FRAGMENT_BIT
	#define H_shader_stage_compute VK_SHADER_STAGE_COMPUTE_BIT

typedef VkPipelineShaderStageCreateInfo H_info_pipeline_shader_stage;
	#define H_create_info_pipeline_shader_stage(                      \
		in_stage,                                                       \
		in_module,                                                      \
		in_name                                                         \
	)                                                                 \
		( H_info_pipeline_shader_stage )                                \
		{                                                               \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, \
			.stage = in_stage,                                            \
			.module = in_module,                                          \
			.pName = in_name,                                             \
		}

//

typedef VkVertexInputBindingDescription H_vertex_binding;
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

typedef VkVertexInputAttributeDescription H_vertex_attribute;

typedef VkPipelineVertexInputStateCreateInfo H_info_pipeline_vertex;
	#define H_create_info_pipeline_vertex(                                  \
		in_binding_count,                                                     \
		in_binding,                                                           \
		in_attribute_count,                                                   \
		in_attributes                                                         \
	)                                                                       \
		( H_info_pipeline_vertex )                                            \
		{                                                                     \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, \
			.vertexBindingDescriptionCount = in_binding_count,                  \
			.pVertexBindingDescriptions = in_binding,                           \
			.vertexAttributeDescriptionCount = in_attribute_count,              \
			.pVertexAttributeDescriptions = in_attributes,                      \
		}

typedef VkPipelineInputAssemblyStateCreateInfo H_info_pipeline_assembly;
	#define H_create_info_pipeline_assembly(                                  \
		in_topology,                                                            \
		in_primitive_restart_enable                                             \
	)                                                                         \
		( H_info_pipeline_assembly )                                            \
		{                                                                       \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, \
			.topology = in_topology,                                              \
			.primitiveRestartEnable = in_primitive_restart_enable,                \
		}

typedef VkPrimitiveTopology H_topology;
	#define H_topology_tri VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	#define H_topology_line VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	#define H_topology_strip VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
	#define H_topology_pixel VK_PRIMITIVE_TOPOLOGY_POINT_LIST

//

typedef VkViewport H_viewport;
	#define H_create_viewport(    \
		in_x,                       \
		in_y,                       \
		in_width,                   \
		in_height,                  \
		in_deptH_min,               \
		in_deptH_max                \
	)                             \
		( H_viewport )              \
		{                           \
			.x = in_x,                \
			.y = in_y,                \
			.width = in_width,        \
			.height = in_height,      \
			.minDepth = in_deptH_min, \
			.maxDepth = in_deptH_max, \
		}

typedef VkRect2D H_scissor;

typedef VkPipelineViewportStateCreateInfo H_info_pipeline_viewport;
	#define H_create_info_pipeline_viewport(                            \
		in_viewport_count,                                                \
		in_viewports,                                                     \
		in_scissor_count,                                                 \
		in_scissors                                                       \
	)                                                                   \
		( H_info_pipeline_viewport )                                      \
		{                                                                 \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, \
			.viewportCount = in_viewport_count,                             \
			.pViewports = in_viewports,                                     \
			.scissorCount = in_scissor_count,                               \
			.pScissors = in_scissors,                                       \
		}

typedef VkPipelineRasterizationStateCreateInfo H_info_pipeline_raster;
	#define H_create_info_pipeline_raster(                                   \
		in_depth_clamp_enable,                                                 \
		in_rasterizer_discard_enable,                                          \
		in_polygon_mode,                                                       \
		in_cull_mode,                                                          \
		in_front_face,                                                         \
		in_depth_bias_enable,                                                  \
		in_depth_bias_constant_factor,                                         \
		in_depth_bias_clamp,                                                   \
		in_depth_bias_slope_factor,                                            \
		in_line_width                                                          \
	)                                                                        \
		( H_info_pipeline_raster )                                             \
		{                                                                      \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, \
			.flags = 0,                                                          \
			.depthClampEnable = in_depth_clamp_enable,                           \
			.rasterizerDiscardEnable = in_rasterizer_discard_enable,             \
			.polygonMode = in_polygon_mode,                                      \
			.cullMode = in_cull_mode,                                            \
			.frontFace = in_front_face,                                          \
			.depthBiasEnable = in_depth_bias_enable,                             \
			.depthBiasConstantFactor = in_depth_bias_constant_factor,            \
			.depthBiasClamp = in_depth_bias_clamp,                               \
			.depthBiasSlopeFactor = in_depth_bias_slope_factor,                  \
			.lineWidth = in_line_width                                           \
		}

	#define H_multisamples_1 VK_SAMPLE_COUNT_1_BIT
	#define H_multisamples_2 VK_SAMPLE_COUNT_2_BIT
	#define H_multisamples_4 VK_SAMPLE_COUNT_4_BIT
	#define H_multisamples_8 VK_SAMPLE_COUNT_8_BIT
	#define H_multisamples_16 VK_SAMPLE_COUNT_16_BIT
	#define H_multisamples_32 VK_SAMPLE_COUNT_32_BIT
	#define H_multisamples_64 VK_SAMPLE_COUNT_64_BIT

typedef VkPipelineMultisampleStateCreateInfo H_info_pipeline_multisample;
	#define H_create_info_pipeline_multisample(                            \
		in_rasterization_samples,                                            \
		in_sample_shading_enable,                                            \
		in_min_sample_shading,                                               \
		in_p_sample_mask,                                                    \
		in_alpha_to_coverage_enable,                                         \
		in_alpha_to_one_enable                                               \
	)                                                                      \
		( H_info_pipeline_multisample )                                      \
		{                                                                    \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, \
			.rasterizationSamples = in_rasterization_samples,                  \
			.sampleShadingEnable = in_sample_shading_enable,                   \
			.minSampleShading = in_min_sample_shading,                         \
			.pSampleMask = in_p_sample_mask,                                   \
			.alphaToCoverageEnable = in_alpha_to_coverage_enable,              \
			.alphaToOneEnable = in_alpha_to_one_enable                         \
		}

typedef VkPipelineDepthStencilStateCreateInfo H_info_pipeline_depth_stencil;
	#define H_create_info_pipeline_depth_stencil(                            \
		in_depth_test_enable,                                                  \
		in_depth_write_enable,                                                 \
		in_depth_compare_op,                                                   \
		in_depth_bounds_test_enable,                                           \
		in_stencil_test_enable,                                                \
		in_front_stencil_op_state,                                             \
		in_back_stencil_op_state,                                              \
		in_min_depth_bounds,                                                   \
		in_max_depth_bounds                                                    \
	)                                                                        \
		( H_info_pipeline_depth_stencil )                                      \
		{                                                                      \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO, \
			.depthTestEnable = in_depth_test_enable,                             \
			.depthWriteEnable = in_depth_write_enable,                           \
			.depthCompareOp = in_depth_compare_op,                               \
			.depthBoundsTestEnable = in_depth_bounds_test_enable,                \
			.stencilTestEnable = in_stencil_test_enable,                         \
			.front = in_front_stencil_op_state,                                  \
			.back = in_back_stencil_op_state,                                    \
			.minDepthBounds = in_min_depth_bounds,                               \
			.maxDepthBounds = in_max_depth_bounds                                \
		}

typedef VkPipelineColorBlendAttachmentState H_info_pipeline_blend_mode;
	#define H_create_info_pipeline_blend_mode(            \
		in_blend_enable,                                    \
		in_src_color_blend_factor,                          \
		in_dst_color_blend_factor,                          \
		in_color_blend_op,                                  \
		in_src_alpha_blend_factor,                          \
		in_dst_alpha_blend_factor,                          \
		in_alpha_blend_op,                                  \
		in_color_write_mask                                 \
	)                                                     \
		{                                                   \
			.blendEnable = in_blend_enable,                   \
			.srcColorBlendFactor = in_src_color_blend_factor, \
			.dstColorBlendFactor = in_dst_color_blend_factor, \
			.colorBlendOp = in_color_blend_op,                \
			.srcAlphaBlendFactor = in_src_alpha_blend_factor, \
			.dstAlphaBlendFactor = in_dst_alpha_blend_factor, \
			.alphaBlendOp = in_alpha_blend_op,                \
			.colorWriteMask = in_color_write_mask,            \
		}

volatile static const H_info_pipeline_blend_mode H_blend_mode_none = H_create_info_pipeline_blend_mode(
	VK_FALSE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_normal = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_constant = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_CONSTANT_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_add = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_subtract = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_REVERSE_SUBTRACT,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_REVERSE_SUBTRACT,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_multiply = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_DST_COLOR,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_DST_ALPHA,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_screen = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

volatile static const H_info_pipeline_blend_mode H_blend_mode_overlay = H_create_info_pipeline_blend_mode(
	VK_TRUE,
	VK_BLEND_FACTOR_SRC_COLOR,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
);

typedef VkPipelineColorBlendStateCreateInfo H_info_pipeline_blend;
	#define H_create_info_pipeline_blend(                                  \
		in_logic_op_enable,                                                  \
		in_logic_op,                                                         \
		in_blend_mode_count,                                                 \
		in_blend_modes,                                                      \
		in_blend_r,                                                          \
		in_blend_g,                                                          \
		in_blend_b,                                                          \
		in_blend_a                                                           \
	)                                                                      \
		( H_info_pipeline_blend )                                            \
		{                                                                    \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, \
			.logicOpEnable = in_logic_op_enable,                               \
			.logicOp = in_logic_op,                                            \
			.attachmentCount = in_blend_mode_count,                            \
			.pAttachments = in_blend_modes,                                    \
			.blendConstants[ 0 ] = in_blend_r,                                 \
			.blendConstants[ 1 ] = in_blend_g,                                 \
			.blendConstants[ 2 ] = in_blend_b,                                 \
			.blendConstants[ 3 ] = in_blend_a,                                 \
		}

typedef VkDynamicState H_dynamic_state;
typedef VkPipelineDynamicStateCreateInfo H_info_pipeline_dynamic;
	#define H_create_info_pipeline_dynamic(                            \
		in_dynamic_state_count,                                          \
		in_p_dynamic_states                                              \
	)                                                                  \
		{                                                                \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, \
			.dynamicStateCount = in_dynamic_state_count,                   \
			.pDynamicStates = in_p_dynamic_states,                         \
			.flags = 0,                                                    \
			.pNext = NULL                                                  \
		}

volatile static const H_info_pipeline_dynamic H_dynamic_state_normal = H_create_info_pipeline_dynamic(
	2, ( ( H_dynamic_state[] ){ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_BLEND_CONSTANTS } )
);

typedef VkPushConstantRange H_info_push_constant_range;
	#define H_create_info_push_constant_range( \
		in_stage_flags,                          \
		in_offset,                               \
		in_size                                  \
	)                                          \
		( H_info_push_constant_range )           \
		{                                        \
			.stageFlags = in_stage_flags,          \
			.offset = in_offset,                   \
			.size = in_size,                       \
		}

//

typedef VkAttachmentReference H_attachment_reference;
typedef VkSubpassDescription H_subpass_description;
typedef VkAttachmentDescription H_attachment_description;

typedef VkRenderPass H_render_pass;
typedef VkRenderPassCreateInfo H_info_render_pass;
	#define H_create_info_render_pass(                      \
		in_attachment_count,                                  \
		in_attachments,                                       \
		in_subpass_count,                                     \
		in_subpasses,                                         \
		in_dependency_count,                                  \
		in_dependencies                                       \
	)                                                       \
		( H_info_render_pass )                                \
		{                                                     \
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, \
			.attachmentCount = in_attachment_count,             \
			.pAttachments = in_attachments,                     \
			.subpassCount = in_subpass_count,                   \
			.pSubpasses = in_subpasses,                         \
			.dependencyCount = in_dependency_count,             \
			.pDependencies = in_dependencies,                   \
		}

H_render_pass H_new_render_pass( H_device in_device, H_info_render_pass in_info )
{
	H_render_pass temp_render_pass;
	vkCreateRenderPass( in_device, &in_info, NULL, &temp_render_pass );
	return temp_render_pass;
}

typedef VkRenderPassBeginInfo H_info_begin_render_pass;
	#define H_create_info_begin_render_pass(               \
		in_render_pass,                                      \
		in_framebuffer,                                      \
		in_render_area,                                      \
		in_clear_count,                                      \
		in_clear_values                                      \
	)                                                      \
		( H_info_begin_render_pass )                         \
		{                                                    \
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, \
			.renderPass = in_render_pass,                      \
			.framebuffer = in_framebuffer,                     \
			.renderArea = in_render_area,                      \
			.clearValueCount = in_clear_count,                 \
			.pClearValues = in_clear_values                    \
		}

//

typedef VkSemaphore H_semaphore;
typedef VkSemaphoreCreateInfo H_info_semaphore;
	#define H_create_info_semaphore()                     \
		( H_info_semaphore )                                \
		{                                                   \
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, \
		}

H_semaphore H_new_semaphore( H_device in_device, H_info_semaphore in_info )
{
	H_semaphore temp_semaphore;
	vkCreateSemaphore( in_device, &in_info, NULL, &temp_semaphore );
	return temp_semaphore;
}

//

typedef VkFence H_fence;
typedef VkFenceCreateInfo H_info_fence;
	#define H_create_info_fence()                     \
		( H_info_fence )                                \
		{                                               \
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, \
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,        \
		}

H_fence H_new_fence( H_device in_device, H_info_fence in_info )
{
	H_fence temp_fence;
	vkCreateFence( in_device, &in_info, NULL, &temp_fence );
	return temp_fence;
}

//

typedef VkPipeline H_pipeline;
typedef VkGraphicsPipelineCreateInfo H_info_pipeline;
	#define H_create_info_pipeline(                               \
		in_flags,                                                   \
		in_stage_count,                                             \
		in_stages,                                                  \
		in_vertex_input_state,                                      \
		in_input_assembly_state,                                    \
		in_tessellation_state,                                      \
		in_viewport_state,                                          \
		in_rasterization_state,                                     \
		in_multisample_state,                                       \
		in_deptH_stencil_state,                                     \
		in_color_blend_state,                                       \
		in_dynamic_state,                                           \
		in_layout,                                                  \
		in_render_pass,                                             \
		in_subpass,                                                 \
		in_base_pipeline_handle,                                    \
		in_base_pipeline_index                                      \
	)                                                             \
		( H_info_pipeline )                                         \
		{                                                           \
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, \
			.flags = in_flags,                                        \
			.stageCount = in_stage_count,                             \
			.pStages = in_stages,                                     \
			.pVertexInputState = in_vertex_input_state,               \
			.pInputAssemblyState = in_input_assembly_state,           \
			.pTessellationState = in_tessellation_state,              \
			.pViewportState = in_viewport_state,                      \
			.pRasterizationState = in_rasterization_state,            \
			.pMultisampleState = in_multisample_state,                \
			.pDepthStencilState = in_deptH_stencil_state,             \
			.pColorBlendState = in_color_blend_state,                 \
			.pDynamicState = in_dynamic_state,                        \
			.layout = in_layout,                                      \
			.renderPass = in_render_pass,                             \
			.subpass = in_subpass,                                    \
			.basePipelineHandle = in_base_pipeline_handle,            \
			.basePipelineIndex = in_base_pipeline_index,              \
		}

H_pipeline H_new_pipeline( H_device in_device, H_info_pipeline in_info )
{
	H_pipeline temp_pipeline;
	vkCreateGraphicsPipelines( in_device, VK_NULL_HANDLE, 1U, &in_info, NULL, &temp_pipeline );
	return temp_pipeline;
}

//

typedef VkPipelineLayout H_pipeline_layout;
typedef VkPipelineLayoutCreateInfo H_info_pipeline_layout;
	#define H_create_info_pipeline_layout(                      \
		in_set_layout_count,                                      \
		in_set_layouts,                                           \
		in_push_constant_range_count,                             \
		in_push_constant_ranges                                   \
	)                                                           \
		( H_info_pipeline_layout )                                \
		{                                                         \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, \
			.setLayoutCount = in_set_layout_count,                  \
			.pSetLayouts = in_set_layouts,                          \
			.pushConstantRangeCount = in_push_constant_range_count, \
			.pPushConstantRanges = in_push_constant_ranges,         \
		}

H_pipeline_layout H_new_pipeline_layout( H_device in_device, H_info_pipeline_layout in_info )
{
	H_pipeline_layout temp_pipeline_layout;
	vkCreatePipelineLayout( in_device, &in_info, NULL, &temp_pipeline_layout );
	return temp_pipeline_layout;
}

//

typedef VkDescriptorType H_descriptor_type;
	#define H_descriptor_type_image VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	#define H_descriptor_type_storage VK_DESCRIPTOR_TYPE_STORAGE_BUFFER

typedef VkDescriptorSetLayout H_descriptor_set_layout;
typedef VkDescriptorSetLayoutBinding H_descriptor_set_layout_binding;
typedef VkDescriptorSetLayoutCreateInfo H_info_descriptor_set_layout;
	#define H_create_info_descriptor_set_layout(                      \
		in_binding_count,                                               \
		in_bindings,                                                    \
		in_flags                                                        \
	)                                                                 \
		( H_info_descriptor_set_layout )                                \
		{                                                               \
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, \
			.bindingCount = in_binding_count,                             \
			.pBindings = in_bindings,                                     \
			.flags = in_flags                                             \
		}

H_descriptor_set_layout H_new_descriptor_set_layout( H_device in_device, H_info_descriptor_set_layout in_info )
{
	H_descriptor_set_layout temp_descriptor_set;
	vkCreateDescriptorSetLayout( in_device, &in_info, NULL, &temp_descriptor_set );
	return temp_descriptor_set;
}

typedef VkDescriptorPool H_descriptor_pool;
typedef VkDescriptorPoolSize H_descriptor_pool_size;
typedef VkDescriptorPoolCreateInfo H_info_descriptor_pool;
	#define H_create_info_descriptor_pool(                      \
		in_max_sets,                                              \
		in_pool_size_count,                                       \
		in_pool_sizes,                                            \
		in_flags                                                  \
	)                                                           \
		( H_info_descriptor_pool )                                \
		{                                                         \
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, \
			.maxSets = in_max_sets,                                 \
			.poolSizeCount = in_pool_size_count,                    \
			.pPoolSizes = in_pool_sizes,                            \
			.flags = in_flags                                       \
		}

H_descriptor_pool H_new_descriptor_pool( H_device in_device, H_info_descriptor_pool in_info )
{
	H_descriptor_pool temp_descriptor_pool;
	vkCreateDescriptorPool( in_device, &in_info, NULL, &temp_descriptor_pool );
	return temp_descriptor_pool;
}

typedef VkDescriptorSet H_descriptor_set;
typedef VkDescriptorSetAllocateInfo H_info_descriptor_set;
	#define H_create_info_descriptor_set(                        \
		in_descriptor_pool,                                        \
		in_descriptor_set_count,                                   \
		in_set_layouts                                             \
	)                                                            \
		( H_info_descriptor_set )                                  \
		{                                                          \
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, \
			.descriptorPool = in_descriptor_pool,                    \
			.descriptorSetCount = in_descriptor_set_count,           \
			.pSetLayouts = in_set_layouts,                           \
		}

H_descriptor_set H_new_descriptor_set( H_device in_device, H_info_descriptor_set in_info )
{
	H_descriptor_set temp_descriptor_set;
	vkAllocateDescriptorSets( in_device, &in_info, &temp_descriptor_set );
	return temp_descriptor_set;
}

//

void H_update_descriptor_set_storage( unsigned int in_binding, H_device in_device, H_descriptor_set in_descriptor_set, H_buffer in_buffer, unsigned long long int in_size )
{
	VkDescriptorBufferInfo buffer_info = {
		.offset = 0,
		.buffer = in_buffer,
		.range = in_size };

	VkWriteDescriptorSet descriptor_write = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = in_descriptor_set,
		.dstBinding = in_binding,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		.pBufferInfo = &buffer_info };

	vkUpdateDescriptorSets( in_device, 1, &descriptor_write, 0, NULL );
}

void H_update_descriptor_set_image( unsigned int in_binding, H_device in_device, H_descriptor_set in_descriptor_set, H_sampler in_sampler, H_image_view in_image_view )
{
	VkDescriptorImageInfo image_info = {
		.sampler = in_sampler,
		.imageView = in_image_view,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

	VkWriteDescriptorSet descriptor_write = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = in_descriptor_set,
		.dstBinding = in_binding,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &image_info };

	vkUpdateDescriptorSets( in_device, 1, &descriptor_write, 0, NULL );
}

//

typedef VkFramebuffer H_framebuffer;
typedef VkFramebufferCreateInfo H_info_framebuffer;
	#define H_create_info_framebuffer(                      \
		in_render_pass,                                       \
		in_attachment_count,                                  \
		in_attachments,                                       \
		in_width,                                             \
		in_height,                                            \
		in_layers                                             \
	)                                                       \
		( H_info_framebuffer )                                \
		{                                                     \
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, \
			.renderPass = in_render_pass,                       \
			.attachmentCount = in_attachment_count,             \
			.pAttachments = in_attachments,                     \
			.width = in_width,                                  \
			.height = in_height,                                \
			.layers = in_layers,                                \
		}

H_framebuffer H_new_framebuffer( H_device in_device, H_info_framebuffer in_info )
{
	H_framebuffer temp_framebuffer;
	vkCreateFramebuffer( in_device, &in_info, NULL, &temp_framebuffer );
	return temp_framebuffer;
}

//

typedef VkCommandPool H_command_pool;
typedef VkCommandPoolCreateInfo H_info_command_pool;
	#define H_create_info_command_pool(                      \
		in_queue_family_index                                  \
	)                                                        \
		( H_info_command_pool )                                \
		{                                                      \
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, \
			.queueFamilyIndex = in_queue_family_index,           \
		}

H_command_pool H_new_command_pool( H_device in_device, H_info_command_pool in_info )
{
	H_command_pool temp_command_pool;
	vkCreateCommandPool( in_device, &in_info, NULL, &temp_command_pool );
	return temp_command_pool;
}

//

typedef VkCommandBuffer H_command_buffer;
typedef VkCommandBufferAllocateInfo H_info_command_buffer;
	#define H_create_info_command_buffer(                        \
		in_command_pool,                                           \
		in_level,                                                  \
		in_count                                                   \
	)                                                            \
		( H_info_command_buffer )                                  \
		{                                                          \
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, \
			.commandPool = in_command_pool,                          \
			.level = in_level,                                       \
			.commandBufferCount = in_count,                          \
		}

	#define H_command_buffer_level_primary VK_COMMAND_BUFFER_LEVEL_PRIMARY
	#define H_command_buffer_level_secondary VK_COMMAND_BUFFER_LEVEL_SECONDARY

void H_allocate_command_buffers( H_device in_device, H_info_command_buffer in_info, H_command_buffer* command_buffers )
{
	vkAllocateCommandBuffers( in_device, &in_info, command_buffers );
}

//

typedef VkSubmitInfo H_info_submit;
	#define H_create_info_submit(                                               \
		in_ready_semaphore_count,                                                 \
		in_ready_semaphores,                                                      \
		in_wait_stages,                                                           \
		in_command_count,                                                         \
		in_commands,                                                              \
		in_done_semaphore_count,                                                  \
		in_done_semaphores                                                        \
	)                                                                           \
		( H_info_submit )                                                         \
		{                                                                         \
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,                                 \
			.waitSemaphoreCount = in_ready_semaphore_count,                         \
			.pWaitSemaphores = in_ready_semaphores,                                 \
			.pWaitDstStageMask = in_wait_stages,                                    \
			.commandBufferCount = in_command_count, .pCommandBuffers = in_commands, \
			.signalSemaphoreCount = in_done_semaphore_count,                        \
			.pSignalSemaphores = in_done_semaphores,                                \
		}

void H_submit_queue( H_queue in_queue, H_info_submit in_submit, H_fence in_fence )
{
	vkQueueSubmit( in_queue, 1, &in_submit, in_fence );
}

#endif