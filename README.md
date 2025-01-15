# android-vulkan-engine

android-vulkan-engine

todo list

- [x] 01 simple triangle (vertex buffer / index buffer)


- [x] 02 single color filled triangle (uniform buffer)


- [x] 03 colored triangle (vertex buffer with multiple uniforms)

- [ ] 04 texture image (sampler2D)

- [ ] 05 model loading (obj)

- [x] 06 msaa (anti-aliasing) (done)

- [ ] 07 skybox (cubemap)

- [ ] 08 normal mapping

- [ ] 09 tillable texture

- [ ] 06 lighting (phong)

- [ ] 07 shadow mapping

- [ ] 08 post processing (bloom)

- [ ] 09 deferred rendering

- [ ] 10 instancing

- [ ] 11 compute shader

- [ ] 12 particles

- [ ] 13 physics



### todo

engine api design

```C++
intEngine(){
    // create vk::instance
    engine = EngineBuilder{}
    // set extensions
    .extensions(..)
    // or provide extensions
    .extensionsSelection([](availableExtensions)->vector<char>{ ... })
    // set layers
    .layers(..)
    // provide layers
    .layersSelection([](availablelayers)->vector<char>{ ... })
    // set work mode
    .asGraphics() // or asComputer(), return GraphicsEngine or ComputerEngine
        // set surface
        .surface(WindowsSurface(..)) // or AndroidSurface(..)/LinuxSurface(..)/MacSurface(..)/IosSurface(..)
        // set PhysicalDevice , use one of apis
        .selectPhysicalDevice([](vector<vk::PhysicalDevice>->vk::PhysicalDevice){..})
        .selectPhysicalDevice(DefaultPhysicalDeviceSelector()) // or CustomPhysicalDeviceSelector
        .autoSelectPhysicalDevice()
        // set mass
        .enableMsaa() // auto provide
        .enableMsaa(maxMsaa) // provide less or equal maxMsaa Samples
        .enableMsaa([](vk::SampleCountFlags flags)->vk::SampleCountFlags) // by your own
        //vertex shader
        .vertexShaderBuilder()
            .code(vertexShaderCode)
            .addVertex(sizeof(app::Vertex)) // return VulkanVertexBuilder
                .addAttrbute(ShaderFormat::Vec3) // binding = 0 location = 0, offset = 0
                                                 // offset += offsetof(app::Vertex, attr1) , location++
                .addAttrbute(ShaderFormat::Vec3) // binding = 0 location = 1, offset = 16 
                                                 // align(16) vec3 = sizeof(vec4)
                                                 // offset += offsetof(app::Vertex, attr2)
            .build() // binding ++ , location = 0, offset = 0 , return to vertexShaderBuilder
            .addVertex(..)...build()
            .setPushConstants(sizeof(app:MvpMatrix))
            .addUniformSet(sizeof(app::Ubo1)) // binding = 0, binding++
            .addUniformSet(sizeof(app::Ubo2)) // binding = 1
        .build() // vertexShaderBuilder, return to engine
        .fragmentShaderBuilder()
            .code(fragmentShaderCode)
            // .. same as vertex shader, set uniform / push constants /...
            .build()
        .createVertexBuffer(maxCount*sizeof(app::Vertex))
        .updateVertex(vertices)
        .createIndexBuffer(maxCount*sizeof(uint32_t))
        .updateIndexBuffer(indices)
        .updateVertexPushConstant(mvpMatrix);
}

drawFrame(){
    engine.updateVertexPushConstant(mvpMatrix);
    engine->drawFrame();
}
```



