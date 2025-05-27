#include "Core/Logging/LogManager.h"
#include "hive.h"
#include "Core/Event/EventManager.h"

#include "Display/DisplayAPI.h"
#include "Display/DisplayType.h"

#include "Graphic/GFX.h"
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <chrono>


#define TINYOBJLOADER_IMPLEMENTATION
#include <Core/Profiling/Profiler.h>

#include "tiny_obj_loader.h"


constexpr int MAX_FRAME_IN_FLIGHT = 3;

//UBO
struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};


struct Testbed
{
    hive::Display *display = nullptr;

    hive::gfx::DeviceHandle *device = nullptr;
    hive::gfx::SwapchainHandle *swapchain = nullptr;
    hive::gfx::RenderpassHandle *render_pass = nullptr;

    hive::gfx::ShaderProgramHandle *shader_program = nullptr;
    hive::gfx::BindingGroupLayoutHandle *binding_layout = nullptr;
    hive::gfx::BindingPoolHandle *binding_pool = nullptr;
    hive::gfx::BindingGroupHandle *binding_group = nullptr;

    hive::gfx::CommandPoolHandle *command_pool = nullptr;
    hive::gfx::CommandBufferHandle *command_buffer = nullptr;

    hive::gfx::TextureHandle *framebuffer_depth_texture = nullptr;
    hive::gfx::FramebufferHandle *framebuffer = nullptr;

    hive::gfx::SemaphoreHandle *image_available_semaphore[MAX_FRAME_IN_FLIGHT];
    hive::gfx::SemaphoreHandle *render_finished_semaphore[MAX_FRAME_IN_FLIGHT];
    hive::gfx::FenceHandle *in_flight_fences[MAX_FRAME_IN_FLIGHT];
};

struct VikingScene
{
    hive::gfx::TextureHandle *viking_texture = nullptr;
    hive::gfx::BufferHandle *vertex_buffer = nullptr;
    hive::gfx::BufferHandle *index_buffer = nullptr;
    int indices_count = 0;

    hive::gfx::BufferHandle *ubo_buffer[MAX_FRAME_IN_FLIGHT];
};

Testbed testbed;
VikingScene scene;

void InitDisplay(hive::EventManager &event_manager);
void InitGraphic();
void InitScene();

void ShutdownDisplay();
void ShutdownGraphic();
void ShutdownScene();
int main()
{
    if(!hive::Init())
    {
        return -1;
    }

    hive::EventManager event_manager;

    {
        ProfileCZoneName(ctx, "init");
        InitDisplay(event_manager);
        InitGraphic();
        InitScene();
        ProfileCZoneEnd(ctx);

    }

    bool application_run = true;

    event_manager.CallbackSubscribe(hive::EVENT_CODE_APPLICATION_QUIT, [&application_run](uint16 code, const hive::Event &event)
    {
        application_run = false;
    });

    HIVE_LOG_INFO("Hello World");
    int current_frame = 0;
    static auto startTime = std::chrono::high_resolution_clock::now();
    while(application_run)
    {
        ProfileCZoneName(ctx, "render");
        hive::DisplayPollEvent();
        event_manager.EventFlush();


        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 1080 / (float)920, 0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;


        hive::gfx::buffer_update(testbed.device, testbed.command_pool, scene.ubo_buffer[0], &ubo, sizeof(ubo));
        hive::gfx::tmp_draw(current_frame, testbed.device, testbed.in_flight_fences[current_frame], testbed.image_available_semaphore[current_frame], testbed.render_finished_semaphore[current_frame], testbed.swapchain, testbed.command_buffer, testbed.framebuffer, testbed.render_pass, testbed.shader_program,scene.vertex_buffer, scene.index_buffer, testbed.binding_group, scene.indices_count);
        current_frame = (current_frame + 1) % MAX_FRAME_IN_FLIGHT;
        ProfileCZoneEnd(ctx);

    }

    {
        ProfileCZoneName(ctx, "shutdown")
        hive::gfx::device_wait_idle(testbed.device);
        ShutdownScene();
        ShutdownGraphic();
        ShutdownDisplay();
        ProfileCZoneEnd(ctx);

    }

    hive::Shutdown();
}

void InitDisplay(hive::EventManager &event_manager)
{
    ProfileScoped();
    hive::DisplayCreateInfo create_info(event_manager, 1200, 1080, "Testbed");
    testbed.display = hive::display_create(create_info);
}

void InitGraphic()
{
    ProfileScoped()
    hive::gfx::DeviceDesc device_desc{};
    device_desc.display = testbed.display;

    testbed.device = hive::gfx::device_create(device_desc);

    //Swapchain
    hive::gfx::SwapchainDesc swap_desc{};
    testbed.swapchain = hive::gfx::swapchain_create(testbed.device, swap_desc);
    // testbed.swapchain = hive::gfx::swapchain_resize(testbed.device, tmp_swap, 1200, 1080);


    //Renderpass
    hive::gfx::RenderpassDesc render_pass_desc{};
    render_pass_desc.swapchain = testbed.swapchain;
    testbed.render_pass = hive::gfx::renderpass_create(testbed.device, render_pass_desc);

    //Framebuffer
    hive::gfx::TextureDesc depth_texture_desc{};
    //TODO get this data from display API instead
    depth_texture_desc.width = 1200;
    depth_texture_desc.height = 1080;

    depth_texture_desc.format = hive::gfx::TEXTURE_FORMAT_D32_SFLOAT;
    depth_texture_desc.usage_flags = hive::gfx::TEXTURE_USAGE_DEPTH;
    depth_texture_desc.aspect_flags = hive::gfx::TEXTURE_ASPECT_DEPTH;
    depth_texture_desc.tiling = hive::gfx::TEXTURE_TILING_OPTIMAL;
    depth_texture_desc.memory_property_flags = hive::gfx::MEMORY_PROPERTY_DEVICE_LOCAL;
    testbed.framebuffer_depth_texture = hive::gfx::texture_create(testbed.device, depth_texture_desc);

    hive::gfx::FramebufferDesc framebuffer_desc{};
    framebuffer_desc.swapchain = testbed.swapchain;
    framebuffer_desc.depth_texture = testbed.framebuffer_depth_texture;
    framebuffer_desc.render_pass = testbed.render_pass;
    testbed.framebuffer = hive::gfx::framebuffer_create(testbed.device, framebuffer_desc);


    //Shader
    hive::gfx::ShaderDesc vertex_shader_desc{};
    //TODO use a resource manager instead of directly?
    vertex_shader_desc.path = "../../Testbed/vert.spv";
    vertex_shader_desc.shader_type = hive::gfx::SHADER_TYPE_VERTEX;
    auto vertex_shader = hive::gfx::shader_create(testbed.device, vertex_shader_desc);

    hive::gfx::ShaderDesc fragment_shader_desc{};
    //TODO use a resource manager instead of directly?
    fragment_shader_desc.path = "../../Testbed/frag.spv";
    fragment_shader_desc.shader_type = hive::gfx::SHADER_TYPE_FRAGMENT;
    auto fragment_shader = hive::gfx::shader_create(testbed.device, fragment_shader_desc);

    std::vector<hive::gfx::BindingGroupLayoutDesc> binding_layout_descs;
    binding_layout_descs.push_back({0, 1, hive::gfx::DESCRIPTOR_TYPE_UBO, hive::gfx::SHADER_TYPE_VERTEX});
    binding_layout_descs.push_back({1, 1, hive::gfx::DESCRIPTOR_TYPE_SAMPLER, hive::gfx::SHADER_TYPE_FRAGMENT});
    testbed.binding_layout = hive::gfx::binding_group_layout_create(testbed.device, binding_layout_descs);

    hive::gfx::ShaderProgramDesc shader_program_desc{};
    shader_program_desc.shader_handles.push_back(vertex_shader);
    shader_program_desc.shader_handles.push_back(fragment_shader);
    shader_program_desc.render_pass = testbed.render_pass;
    shader_program_desc.bindings = testbed.binding_layout;
    testbed.shader_program = hive::gfx::shader_program_create(testbed.device, shader_program_desc);

    hive::gfx::BindingPoolDesc binding_pool_desc{};
    binding_pool_desc.max_sets = 3;
    binding_pool_desc.layouts = binding_layout_descs;
    testbed.binding_pool = hive::gfx::binding_pool_create(testbed.device, binding_pool_desc);

    hive::gfx::BindingGroupDesc binding_group_desc{};
    binding_group_desc.count = 3;
    binding_group_desc.binding_pool = testbed.binding_pool;
    binding_group_desc.binding_layout = testbed.binding_layout;
    testbed.binding_group = hive::gfx::binding_group_allocate(testbed.device, binding_group_desc);


    for (int i = 0; i < MAX_FRAME_IN_FLIGHT; i++)
    {
        testbed.image_available_semaphore[i] = hive::gfx::semaphore_create(testbed.device);
        testbed.render_finished_semaphore[i] = hive::gfx::semaphore_create(testbed.device);
        testbed.in_flight_fences[i] = hive::gfx::fence_create(testbed.device, true);
    }





    //Once the program is compiled, we don't need the shader modules anymore
    hive::gfx::shader_destroy(testbed.device, vertex_shader);
    hive::gfx::shader_destroy(testbed.device, fragment_shader);

    //Command pool
    hive::gfx::CommandPoolDesc command_pool_desc{};
    testbed.command_pool = hive::gfx::command_pool_create(testbed.device, command_pool_desc);

    hive::gfx::CommandBufferDesc command_buffer_desc{};
    command_buffer_desc.command_pool = testbed.command_pool;
    command_buffer_desc.count = MAX_FRAME_IN_FLIGHT;

    testbed.command_buffer = hive::gfx::command_buffer_allocate(testbed.device, command_buffer_desc);
}

void InitScene()
{
    ProfileScoped();
    //Texture
    int width, height, channels;
    auto pixels = stbi_load("../../Testbed/viking_room.png", &width, &height, &channels, STBI_rgb_alpha);
    auto image_size = width * height * 4;

    hive::gfx::TextureDesc texture_desc{};
    texture_desc.width = width;
    texture_desc.height = height;
    texture_desc.format = hive::gfx::TEXTURE_FORMAT_R8G8B8A8_SRGB;
    texture_desc.tiling = hive::gfx::TEXTURE_TILING_OPTIMAL;
    texture_desc.usage_flags = hive::gfx::TEXTURE_USAGE_DST | hive::gfx::TEXTURE_USAGE_SAMPLE;
    texture_desc.memory_property_flags = hive::gfx::MEMORY_PROPERTY_DEVICE_LOCAL;
    texture_desc.aspect_flags = hive::gfx::TEXTURE_ASPECT_COLOR;
    scene.viking_texture = hive::gfx::texture_create(testbed.device, texture_desc);

    hive::gfx::texture_update(testbed.device, testbed.command_pool, scene.viking_texture, pixels, image_size);
    stbi_image_free(pixels);

    //Load obj file
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "../../Testbed/viking_room.obj")) {
        throw std::runtime_error(warn + err);
    }

    struct Vertex
    {
        float pos[3];
        float color[3];
        float texCoord[2];
    };

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    for(const auto& shape : shapes)
    {
        for(const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            vertex.pos[0] = attrib.vertices[3 * index.vertex_index + 0];
            vertex.pos[1] = attrib.vertices[3 * index.vertex_index + 1];
            vertex.pos[2] = attrib.vertices[3 * index.vertex_index + 2];

            vertex.texCoord[0] = attrib.texcoords[2 * index.texcoord_index + 0];
            vertex.texCoord[1] =   1.0f - attrib.texcoords[2 * index.texcoord_index + 1];

            vertex.color[0] = 1.0f;
            vertex.color[1] = 1.0f;
            vertex.color[2] = 1.0f;


            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    //Vertex Buffer
    hive::gfx::BufferDesc vertex_buffer_desc{};
    vertex_buffer_desc.usage_flags = hive::gfx::BUFFER_USAGE_VERTEX_BUFFER | hive::gfx::BUFFER_USAGE_TRANSFER_DST;
    vertex_buffer_desc.memory_property_flags = hive::gfx::MEMORY_PROPERTY_DEVICE_LOCAL;
    vertex_buffer_desc.size = sizeof(Vertex) * vertices.size();
    scene.vertex_buffer = hive::gfx::buffer_create(testbed.device, vertex_buffer_desc);

    hive::gfx::buffer_update(testbed.device, testbed.command_pool, scene.vertex_buffer, vertices.data(), vertex_buffer_desc.size);

    //Index buffer
    hive::gfx::BufferDesc index_buffer_desc{};
    index_buffer_desc.usage_flags = hive::gfx::BUFFER_USAGE_INDEX_BUFFER | hive::gfx::BUFFER_USAGE_TRANSFER_DST;
    index_buffer_desc.memory_property_flags = hive::gfx::MEMORY_PROPERTY_DEVICE_LOCAL;
    index_buffer_desc.size = indices.size() * sizeof(indices[0]);
    scene.index_buffer = hive::gfx::buffer_create(testbed.device, index_buffer_desc);
    scene.indices_count = indices.size();

    hive::gfx::buffer_update(testbed.device, testbed.command_pool, scene.index_buffer, indices.data(), index_buffer_desc.size);



    hive::gfx::BufferDesc ubo_buffer_desc{};
    ubo_buffer_desc.usage_flags = hive::gfx::BUFFER_USAGE_UNIFORM_BUFFER | hive::gfx::BUFFER_USAGE_TRANSFER_DST;
    ubo_buffer_desc.memory_property_flags = hive::gfx::MEMORY_PROPERTY_HOST_VISIBLE_BIT | hive::gfx::MEMORY_PROPERTY_HOST_COHERENT_BIT;
    ubo_buffer_desc.size = sizeof(UniformBufferObject);

    for(int i = 0; i < MAX_FRAME_IN_FLIGHT; i++)
    {
        scene.ubo_buffer[i] = hive::gfx::buffer_create(testbed.device, ubo_buffer_desc);
    }





    //Binding group setup
    hive::gfx::BindingGroupWriteDesc ubo_binding_write_desc{};
    ubo_binding_write_desc.binding = 0;
    ubo_binding_write_desc.count = MAX_FRAME_IN_FLIGHT;
    ubo_binding_write_desc.type = hive::gfx::DESCRIPTOR_TYPE_UBO;
    ubo_binding_write_desc.dst_binding_group = testbed.binding_group;
    ubo_binding_write_desc.buffer = scene.ubo_buffer[0];
    hive::gfx::binding_group_write(testbed.device, ubo_binding_write_desc);


    hive::gfx::BindingGroupWriteDesc texture_binding_write_desc{};
    texture_binding_write_desc.binding = 1;
    texture_binding_write_desc.count = MAX_FRAME_IN_FLIGHT;
    texture_binding_write_desc.type = hive::gfx::DESCRIPTOR_TYPE_SAMPLER;
    texture_binding_write_desc.dst_binding_group = testbed.binding_group;
    texture_binding_write_desc.texture = scene.viking_texture;
    hive::gfx::binding_group_write(testbed.device, texture_binding_write_desc);
}


void ShutdownDisplay()
{
    hive::display_destroy(testbed.display);
    testbed.display = nullptr;
}

void ShutdownGraphic()
{


    for (int i = 0; i < MAX_FRAME_IN_FLIGHT; i++)
    {
        hive::gfx::semaphore_destroy(testbed.device, testbed.image_available_semaphore[i]);
        hive::gfx::semaphore_destroy(testbed.device, testbed.render_finished_semaphore[i]);
        hive::gfx::fence_destroy(testbed.device, testbed.in_flight_fences[i]);
    }

    hive::gfx::texture_destroy(testbed.device, testbed.framebuffer_depth_texture);
    hive::gfx::framebuffer_destroy(testbed.device, testbed.framebuffer);
    hive::gfx::command_buffer_free(testbed.device, testbed.command_pool, testbed.command_buffer);
    hive::gfx::command_pool_destroy(testbed.device, testbed.command_pool);
    hive::gfx::binding_group_free(testbed.device, testbed.binding_pool, testbed.binding_group);
    hive::gfx::binding_pool_destroy(testbed.device, testbed.binding_pool);
    hive::gfx::binding_group_layout_destroy(testbed.device, testbed.binding_layout);
    hive::gfx::shader_program_destroy(testbed.device, testbed.shader_program);
    hive::gfx::renderpass_destroy(testbed.device, testbed.render_pass);
    hive::gfx::swapchain_destroy(testbed.device, testbed.swapchain);
    hive::gfx::device_destroy(testbed.device);
}

void ShutdownScene()
{

    for(int i = 0; i < MAX_FRAME_IN_FLIGHT; i++)
    {
        hive::gfx::buffer_destroy(testbed.device, scene.ubo_buffer[i]);
    }

    hive::gfx::buffer_destroy(testbed.device, scene.index_buffer);
    hive::gfx::buffer_destroy(testbed.device, scene.vertex_buffer);
    hive::gfx::texture_destroy(testbed.device, scene.viking_texture);
}
