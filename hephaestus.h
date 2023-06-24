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

	#include <stdlib.h>

	#include <Volk/volk.c>
/// need <vulkan/...>
/// need <vk_video/...>

//

	#define h_make_version( major, minor, patch ) VK_MAKE_API_VERSION( 0, major, minor, patch )

//

typedef VkLayerProperties h_layer_properties;

//

typedef VkApplicationInfo h_info_app;
	#define h_make_info_app( in_app_name,                                      \
													 in_app_version,                                   \
													 in_engine_name,                                   \
													 in_engine_version,                                \
													 in_api_version )                                  \
		( h_info_app )                                                           \
		{                                                                        \
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,                           \
			.pApplicationName = in_app_name, .applicationVersion = in_app_version, \
			.pEngineName = in_engine_name, .engineVersion = in_engine_version,     \
			.apiVersion = in_api_version,                                          \
		}

//

typedef VkInstance h_instance;
typedef VkInstanceCreateInfo h_info_instance;
	#define h_make_info_instance( in_app_info,                                \
																in_layer_count,                             \
																in_layer_names,                             \
																in_extension_count,                         \
																in_extension_names )                        \
		( h_info_instance )                                                     \
		{                                                                       \
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,                      \
			.pApplicationInfo = in_app_info, .enabledLayerCount = in_layer_count, \
			.ppEnabledLayerNames = in_layer_names,                                \
			.enabledExtensionCount = in_extension_count,                          \
			.ppEnabledExtensionNames = in_extension_names,                        \
		}

h_instance h_new_instance( h_info_instance in_info )
{
	h_instance temp_instance;
	vkCreateInstance( &( in_info ), NULL, &( temp_instance ) );
	volkLoadInstanceOnly( temp_instance );
	return temp_instance;
}

//

typedef VkBuffer h_device_buffer;
typedef struct VkBuffer_T struct_h_device_buffer;
typedef VkBufferCreateInfo h_info_buffer;
typedef VkDeviceMemory h_device_memory;
typedef struct VkDeviceMemory_T struct_h_device_memory;
typedef VkMemoryRequirements h_mem_requirements;
typedef VkMemoryAllocateInfo h_info_mem_allocate;

//

typedef VkSurfaceKHR h_surface;
typedef VkSurfaceCapabilitiesKHR h_surface_capabilities;
typedef VkSurfaceFormatKHR h_surface_format;

//

typedef VkPhysicalDevice h_physical_device;
typedef VkPhysicalDeviceProperties h_physical_device_properties;
typedef VkPhysicalDeviceMemoryProperties h_physical_device_memory_properties;
typedef VkPhysicalDeviceFeatures h_physical_device_features;
typedef VkDevice h_device;
typedef VkDeviceCreateInfo h_info_device;
	#define h_make_info_device( in_queue_create_info_count,    \
															in_queue_create_infos,         \
															in_enabled_layer_count,        \
															in_enabled_layer_names,        \
															in_enabled_extension_count,    \
															in_enabled_extension_names,    \
															in_enabled_features )          \
		( h_info_device )                                        \
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

typedef VkDeviceQueueCreateInfo h_info_device_queue;
	#define h_make_info_device_queue( in_queue_family_index,                     \
																		in_queue_count,                            \
																		in_queue_priorities )                      \
		( h_info_device_queue )                                                    \
		{                                                                          \
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                     \
			.queueFamilyIndex = in_queue_family_index, .queueCount = in_queue_count, \
			.pQueuePriorities = in_queue_priorities,                                 \
		}

h_device h_new_device( h_physical_device in_physical_device, h_info_device in_info )
{
	h_device temp_device;
	vkCreateDevice( in_physical_device, &in_info, NULL, &temp_device );
	volkLoadDevice( temp_device );
	return temp_device;
}

//

typedef VkQueue h_queue;

h_queue h_new_queue( h_device in_device, uint32_t family_index, uint32_t queue_index )
{
	h_queue temp_queue;
	vkGetDeviceQueue( in_device, family_index, queue_index, &temp_queue );
	return temp_queue;
}

//

	#define h_present_mode_vsync_off VK_PRESENT_MODE_IMMEDIATE_KHR
	#define h_present_mode_vsync_on VK_PRESENT_MODE_FIFO_KHR
	#define h_present_mode_vsync_optimal VK_PRESENT_MODE_MAILBOX_KHR

typedef VkPresentModeKHR h_present_mode;

//

typedef VkSwapchainKHR h_swapchain;
typedef VkSwapchainCreateInfoKHR h_info_swapchain;
	#define h_make_info_swapchain(                                                 \
			in_surface, in_min_image_count, in_image_format, in_image_color_space,     \
			in_image_extent, in_image_array_layers, in_image_usage,                    \
			in_image_sharing_mode, in_queue_family_index_count,                        \
			in_queue_family_indices, in_pre_transform, in_composite_alpha,             \
			in_present_mode, in_clipped, in_old_swapchain )                            \
		( h_info_swapchain )                                                         \
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

h_swapchain h_new_swapchain( h_device in_device, h_info_swapchain in_info )
{
	h_swapchain temp_swapchain;
	vkCreateSwapchainKHR( in_device, &in_info, NULL, &temp_swapchain );
	return temp_swapchain;
}

//

typedef VkFormat h_format;

typedef VkExtent2D h_extent;
typedef VkExtent3D h_extent_3d;

typedef VkOffset2D h_offset;
typedef VkOffset3D h_offset_3d;

typedef VkComponentMapping h_component_mapping;

//

typedef VkSampler h_sampler;

//

typedef VkImage h_image;
typedef VkImageSubresourceRange h_image_subresource_range;
typedef VkImageLayout h_image_layout;
typedef VkImageTiling h_image_tiling;
typedef VkImageCreateInfo h_info_image;
	#define h_make_info_image(                        \
			in_image_type,                                \
			in_extent,                                    \
			in_mip_levels,                                \
			in_array_layers,                              \
			in_format,                                    \
			in_tiling,                                    \
			in_initial_layout,                            \
			in_usage,                                     \
			in_sharing_mode,                              \
			in_samples )                                  \
		( h_info_image )                                \
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

h_image h_new_image( h_device in_device, h_info_image in_info )
{
	h_image temp_image;
	vkCreateImage( in_device, &in_info, NULL, &temp_image );
	return temp_image;
}

typedef VkImageView h_image_view;
typedef VkImageViewCreateInfo h_info_image_view;
	#define h_make_info_image_view(                        \
			in_image,                                          \
			in_view_type,                                      \
			in_format,                                         \
			in_components,                                     \
			in_subresource_range )                             \
		( h_info_image_view )                                \
		{                                                    \
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, \
			.image = in_image,                                 \
			.viewType = in_view_type,                          \
			.format = in_format,                               \
			.components = in_components,                       \
			.subresourceRange = in_subresource_range,          \
		}

h_image_view h_new_image_view( h_device in_device, h_info_image_view in_info )
{
	h_image_view temp_image_view;
	vkCreateImageView( in_device, &in_info, NULL, &temp_image_view );
	return temp_image_view;
}

//

typedef VkShaderModule h_shader_module;
typedef VkShaderModuleCreateInfo h_info_shader_module;
	#define h_make_info_shader_module(                        \
			in_code,                                              \
			in_code_size )                                        \
		( h_info_shader_module )                                \
		{                                                       \
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, \
			.pCode = in_code,                                     \
			.codeSize = in_code_size,                             \
		}

h_shader_module h_new_shader_module( h_device in_device, h_info_shader_module in_info )
{
	h_shader_module temp_image_view;
	vkCreateShaderModule( in_device, &in_info, NULL, &temp_image_view );
	return temp_image_view;
}

typedef VkPipelineShaderStageCreateInfo h_info_pipeline_shader_stage;
	#define h_make_info_pipeline_shader_stage(                        \
			in_stage,                                                     \
			in_module,                                                    \
			in_name )                                                     \
		( h_info_pipeline_shader_stage )                                \
		{                                                               \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, \
			.stage = in_stage,                                            \
			.module = in_module,                                          \
			.pName = in_name,                                             \
		}

//

typedef VkVertexInputBindingDescription h_vertex_binding;
	#define h_make_vertex_binding_per_vertex(    \
			in_binding,                              \
			in_form_size )                           \
		( h_vertex_binding )                       \
		{                                          \
			.binding = in_binding,                   \
			.stride = in_form_size,                  \
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX \
		}
	#define h_make_vertex_binding_per_instance(    \
			in_binding,                                \
			in_form_size )                             \
		( h_vertex_binding )                         \
		{                                            \
			.binding = in_binding,                     \
			.stride = in_form_size,                    \
			.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE \
		}

typedef VkVertexInputAttributeDescription h_vertex_attribute;

typedef VkPipelineVertexInputStateCreateInfo h_info_pipeline_vertex;
	#define h_make_info_pipeline_vertex(                                    \
			in_binding_count,                                                   \
			in_binding,                                                         \
			in_attribute_count,                                                 \
			in_attributes )                                                     \
		( h_info_pipeline_vertex )                                            \
		{                                                                     \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, \
			.vertexBindingDescriptionCount = in_binding_count,                  \
			.pVertexBindingDescriptions = in_binding,                           \
			.vertexAttributeDescriptionCount = in_attribute_count,              \
			.pVertexAttributeDescriptions = in_attributes,                      \
		}

typedef VkPipelineInputAssemblyStateCreateInfo h_info_pipeline_assembly;
	#define h_make_info_pipeline_assembly(                                    \
			in_topology )                                                         \
		( h_info_pipeline_assembly )                                            \
		{                                                                       \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, \
			.topology = in_topology                                               \
		}

typedef VkPrimitiveTopology h_topology;
	#define h_topology_tri VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	#define h_topology_line VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	#define h_topology_strip VK_PRIMITIVE_TOPOLOGY_LINE_STRIP
	#define h_topology_pixel VK_PRIMITIVE_TOPOLOGY_POINT_LIST

//

typedef VkViewport h_viewport;
	#define h_make_viewport(      \
			in_x,                     \
			in_y,                     \
			in_width,                 \
			in_height,                \
			in_depth_min,             \
			in_depth_max )            \
		( h_viewport )              \
		{                           \
			.x = in_x,                \
			.y = in_y,                \
			.width = in_width,        \
			.height = in_height,      \
			.minDepth = in_depth_min, \
			.maxDepth = in_depth_max, \
		}

typedef VkRect2D h_scissor;

typedef VkPipelineViewportStateCreateInfo h_info_pipeline_viewport;
	#define h_make_info_pipeline_viewport(                              \
			in_viewport_count,                                              \
			in_viewports,                                                   \
			in_scissor_count,                                               \
			in_scissors )                                                   \
		( h_info_pipeline_viewport )                                      \
		{                                                                 \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, \
			.viewportCount = in_viewport_count,                             \
			.pViewports = in_viewports,                                     \
			.scissorCount = in_scissor_count,                               \
			.pScissors = in_scissors,                                       \
		}

//

typedef VkPipelineRasterizationStateCreateInfo h_info_pipeline_raster;
	#define h_make_info_pipeline_raster(                                     \
			in_polygon_mode,                                                     \
			in_line_width,                                                       \
			in_cull_mode, in_front_face )                                        \
		( h_info_pipeline_raster )                                             \
		{                                                                      \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, \
			.polygonMode = in_polygon_mode,                                      \
			.lineWidth = in_line_width,                                          \
			.cullMode = in_cull_mode,                                            \
			.frontFace = in_front_face,                                          \
		}

//

typedef VkAttachmentReference h_attachment_reference;
typedef VkSubpassDescription h_subpass_description;
typedef VkAttachmentDescription h_attachment_description;

typedef VkRenderPass h_render_pass;
typedef VkRenderPassCreateInfo h_info_render_pass;
	#define h_make_info_render_pass(                        \
			in_attachment_count,                                \
			in_attachments,                                     \
			in_subpass_count,                                   \
			in_subpasses,                                       \
			in_dependency_count,                                \
			in_dependencies )                                   \
		( h_info_render_pass )                                \
		{                                                     \
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, \
			.attachmentCount = in_attachment_count,             \
			.pAttachments = in_attachments,                     \
			.subpassCount = in_subpass_count,                   \
			.pSubpasses = in_subpasses,                         \
			.dependencyCount = in_dependency_count,             \
			.pDependencies = in_dependencies,                   \
		}

h_render_pass h_new_render_pass( h_device in_device, h_info_render_pass in_info )
{
	h_render_pass temp_render_pass;
	vkCreateRenderPass( in_device, &in_info, NULL, &temp_render_pass );
	return temp_render_pass;
}

//

typedef VkSemaphore h_semaphore;
typedef VkSemaphoreCreateInfo h_info_semaphore;
	#define h_make_info_semaphore()                       \
		( h_info_semaphore )                                \
		{                                                   \
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, \
		}

h_semaphore h_new_semaphore( h_device in_device, h_info_semaphore in_info )
{
	h_semaphore temp_semaphore;
	vkCreateSemaphore( in_device, &in_info, NULL, &temp_semaphore );
	return temp_semaphore;
}

//

typedef VkFence h_fence;
typedef VkFenceCreateInfo h_info_fence;
	#define h_make_info_fence()                       \
		( h_info_fence )                                \
		{                                               \
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, \
			.flags = VK_FENCE_CREATE_SIGNALED_BIT,        \
		}

h_fence h_new_fence( h_device in_device, h_info_fence in_info )
{
	h_fence temp_fence;
	vkCreateFence( in_device, &in_info, NULL, &temp_fence );
	return temp_fence;
}

//

typedef VkPipeline h_pipeline;
typedef VkGraphicsPipelineCreateInfo h_info_pipeline;
	#define h_make_info_pipeline(                                 \
			in_stage_count,                                           \
			in_stages,                                                \
			in_vertex_input_state,                                    \
			in_input_assembly_state,                                  \
			in_tessellation_state,                                    \
			in_viewport_state,                                        \
			in_rasterization_state,                                   \
			in_multisample_state,                                     \
			in_depth_stencil_state,                                   \
			in_color_blend_state,                                     \
			in_dynamic_state,                                         \
			in_layout,                                                \
			in_render_pass,                                           \
			in_subpass,                                               \
			in_base_pipeline_handle,                                  \
			in_base_pipeline_index )                                  \
		( h_info_pipeline )                                         \
		{                                                           \
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, \
			.stageCount = in_stage_count,                             \
			.pStages = in_stages,                                     \
			.pVertexInputState = in_vertex_input_state,               \
			.pInputAssemblyState = in_input_assembly_state,           \
			.pTessellationState = in_tessellation_state,              \
			.pViewportState = in_viewport_state,                      \
			.pRasterizationState = in_rasterization_state,            \
			.pMultisampleState = in_multisample_state,                \
			.pDepthStencilState = in_depth_stencil_state,             \
			.pColorBlendState = in_color_blend_state,                 \
			.pDynamicState = in_dynamic_state,                        \
			.layout = in_layout,                                      \
			.renderPass = in_render_pass,                             \
			.subpass = in_subpass,                                    \
			.basePipelineHandle = in_base_pipeline_handle,            \
			.basePipelineIndex = in_base_pipeline_index,              \
		}

h_pipeline h_new_pipeline( h_device in_device, h_info_pipeline in_info )
{
	h_pipeline temp_pipeline;
	vkCreateGraphicsPipelines( in_device, VK_NULL_HANDLE, 1U, &in_info, NULL,
														 &temp_pipeline );
	return temp_pipeline;
}

//

typedef VkPipelineLayout h_pipeline_layout;
typedef VkPipelineLayoutCreateInfo h_info_pipeline_layout;
	#define h_make_info_pipeline_layout(                        \
			in_set_layout_count,                                    \
			in_set_layouts,                                         \
			in_push_constant_range_count,                           \
			in_push_constant_ranges )                               \
		( h_info_pipeline_layout )                                \
		{                                                         \
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, \
			.setLayoutCount = in_set_layout_count,                  \
			.pSetLayouts = in_set_layouts,                          \
			.pushConstantRangeCount = in_push_constant_range_count, \
			.pPushConstantRanges = in_push_constant_ranges,         \
		}

h_pipeline_layout h_new_pipeline_layout( h_device in_device, h_info_pipeline_layout in_info )
{
	h_pipeline_layout temp_pipeline_layout;
	vkCreatePipelineLayout( in_device, &in_info, NULL, &temp_pipeline_layout );
	return temp_pipeline_layout;
}

//

typedef VkDescriptorSet h_descriptor_set;
typedef VkDescriptorSetAllocateInfo h_info_descriptor_set;
	#define h_make_info_descriptor_set(                          \
			in_descriptor_pool,                                      \
			in_descriptor_set_count,                                 \
			in_set_layouts )                                         \
		( h_info_descriptor_set )                                  \
		{                                                          \
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, \
			.descriptorPool = in_descriptor_pool,                    \
			.descriptorSetCount = in_descriptor_set_count,           \
			.pSetLayouts = in_set_layouts,                           \
		}

h_descriptor_set h_new_descriptor_set( h_device in_device, h_info_descriptor_set in_info )
{
	h_descriptor_set temp_descriptor_set;
	vkAllocateDescriptorSets( in_device, &in_info, &temp_descriptor_set );
	return temp_descriptor_set;
}

//

typedef VkFramebuffer h_framebuffer;
typedef VkFramebufferCreateInfo h_info_framebuffer;
	#define h_make_info_framebuffer(                        \
			in_render_pass,                                     \
			in_attachment_count,                                \
			in_attachments,                                     \
			in_width,                                           \
			in_height,                                          \
			in_layers )                                         \
		( h_info_framebuffer )                                \
		{                                                     \
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, \
			.renderPass = in_render_pass,                       \
			.attachmentCount = in_attachment_count,             \
			.pAttachments = in_attachments,                     \
			.width = in_width,                                  \
			.height = in_height,                                \
			.layers = in_layers,                                \
		}

h_framebuffer h_new_framebuffer( h_device in_device, h_info_framebuffer in_info )
{
	h_framebuffer temp_framebuffer;
	vkCreateFramebuffer( in_device, &in_info, NULL, &temp_framebuffer );
	return temp_framebuffer;
}

//

typedef VkCommandPool h_command_pool;
typedef VkCommandPoolCreateInfo h_info_command_pool;
	#define h_make_info_command_pool(                        \
			in_queue_family_index )                              \
		( h_info_command_pool )                                \
		{                                                      \
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, \
			.queueFamilyIndex = in_queue_family_index,           \
		}

h_command_pool h_new_command_pool( h_device in_device, h_info_command_pool in_info )
{
	h_command_pool temp_command_pool;
	vkCreateCommandPool( in_device, &in_info, NULL, &temp_command_pool );
	return temp_command_pool;
}

//

typedef VkCommandBuffer h_command_buffer;
typedef VkCommandBufferAllocateInfo h_info_command_buffer;
	#define h_make_info_command_buffer(                          \
			in_command_pool,                                         \
			in_level )                                               \
		( h_info_command_buffer )                                  \
		{                                                          \
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, \
			.commandPool = in_command_pool,                          \
			.level = in_level,                                       \
			.commandBufferCount = 1,                                 \
		}

#define h_command_buffer_level_primary VK_COMMAND_BUFFER_LEVEL_PRIMARY
#define h_command_buffer_level_secondary VK_COMMAND_BUFFER_LEVEL_SECONDARY

h_command_buffer h_new_command_buffer( h_device in_device, h_info_command_buffer in_info )
{
	h_command_buffer* temp_buffer = ( h_command_buffer* )malloc( sizeof( h_command_buffer ) );
	vkAllocateCommandBuffers( in_device, &in_info, temp_buffer );
	return *temp_buffer;
}

void allocate_h_command_buffers( h_device in_device, h_info_command_buffer in_info, h_command_buffer* command_buffers )
{
	vkAllocateCommandBuffers( in_device, &in_info, command_buffers );
}



//

typedef VkSubmitInfo h_info_submit;
	#define h_make_info_submit(                                                 \
			in_ready_semaphore_count,                                               \
			in_ready_semaphores,                                                    \
			in_wait_stages,                                                         \
			in_command_count,                                                       \
			in_commands,                                                            \
			in_done_semaphore_count,                                                \
			in_done_semaphores )                                                    \
		( h_info_submit )                                                         \
		{                                                                         \
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,                                 \
			.waitSemaphoreCount = in_ready_semaphore_count,                         \
			.pWaitSemaphores = in_ready_semaphores,                                 \
			.pWaitDstStageMask = in_wait_stages,                                    \
			.commandBufferCount = in_command_count, .pCommandBuffers = in_commands, \
			.signalSemaphoreCount = in_done_semaphore_count,                        \
			.pSignalSemaphores = in_done_semaphores,                                \
		}

void h_submit_queue( h_queue in_queue, h_info_submit in_submit, h_fence in_fence )
{
	vkQueueSubmit( in_queue, 1, &in_submit, in_fence );
}

#endif