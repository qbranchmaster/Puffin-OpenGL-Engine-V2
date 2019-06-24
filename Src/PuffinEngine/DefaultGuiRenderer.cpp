/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultGuiRenderer.hpp"

using namespace puffin;

DefaultGuiRenderer::DefaultGuiRenderer(RenderSettingsPtr render_settings, WindowPtr window,
    CameraPtr camera, MasterRendererPtr master_renderer) {
    if (!window || !render_settings || !camera || !master_renderer) {
        throw Exception("DefaultGuiRenderer::DefaultGuiRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    render_settings_ = render_settings;
    target_window_ = window;
    camera_ = camera;
    master_renderer_ = master_renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    setupImGui();

    const char *glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(target_window_->handle_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void DefaultGuiRenderer::render(ScenePtr scene) {
    if (!enabled_) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderMainMenuBar();
    renderAboutDialog();
    renderCameraDialog();
    renderPostprocessDialog();
    renderLightingDialog();
    renderShadowMappingDialog();
    renderFogDialog();
    renderCaptureDialog();

    renderWaterRendererDialog(scene);
    renderSkyboxRendererDialog(scene);
    renderMeshRendererDialog(scene);

    // ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLboolean DefaultGuiRenderer::isCapturingMouse() const {
    return static_cast<GLboolean>(ImGui::GetIO().WantCaptureMouse);
}

GLboolean DefaultGuiRenderer::isCapturingKeyboard() const {
    return static_cast<GLboolean>(ImGui::GetIO().WantCaptureKeyboard);
}

void DefaultGuiRenderer::setupImGui() {
    ImGuiIO &io = ImGui::GetIO();

    // Setup style
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(4, 3);
    style.ItemSpacing = ImVec2(6, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);

    style.IndentSpacing = 16;
    style.ScrollbarSize = 12;
    style.GrabMinSize = 12;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.WindowRounding = 5.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 0.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowTitleAlign = ImVec2(0.0f, 0.50f);
    style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.DisplaySafeAreaPadding = ImVec2(3, 3);

    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::StyleColorsDark();
}

void DefaultGuiRenderer::renderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load scene")) {
            }

            if (ImGui::MenuItem("Save scene")) {
            }

            if (ImGui::MenuItem("Reset scene")) {
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Quit")) {
                master_renderer_->stop();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Camera", NULL, &render_camera_dialog_);
            ImGui::MenuItem("Lighting", NULL, &render_lighting_dialog_);
            ImGui::MenuItem("Shadow mapping", NULL, &render_shadow_map_dialog_);
            ImGui::MenuItem("Postprocess", NULL, &render_postprocess_dialog_);
            ImGui::MenuItem("Fog", NULL, &render_fog_dialog_);
            ImGui::Separator();
            ImGui::MenuItem("Skybox renderer", NULL, &render_skybox_renderer_dialog_);
            ImGui::MenuItem("Mesh renderer", NULL, &render_mesh_renderer_dialog_);
            ImGui::MenuItem("Water renderer", NULL, &render_water_renderer_dialog_);
            ImGui::Separator();
            ImGui::MenuItem("Capture", NULL, &render_capture_dialog_);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About Puffin Engine", NULL, &render_about_dialog_);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void DefaultGuiRenderer::renderAboutDialog() {
    if (!render_about_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(496, 110), ImGuiCond_Always);

    if (!ImGui::Begin("About Puffin Engine", &render_about_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Puffin Engine ver. 2.0");
    ImGui::Separator();
    ImGui::Text("Coded by: Sebastian 'qbranchmaster' Tabaka");
    ImGui::Text("Contact:  sebastian.tabaka@outlook.com");
    ImGui::Text("Repo URL:");
    ImGui::SameLine();
    std::string url = "https://github.com/qbranchmaster/Puffin-OpenGL-Engine-V2";
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), url.c_str());
    if (ImGui::IsItemClicked()) {
#ifdef WIN32
        ShellExecute(0, 0, url.c_str(), 0, 0, SW_SHOW);
#endif // WIN32
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderCameraDialog() {
    if (!render_camera_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(350, 195), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Camera", &render_camera_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    glm::vec3 position = camera_->getPosition();
    float cam_pos[] = {position.x, position.y, position.z};
    ImGui::DragFloat3("Position", cam_pos, 0.01f);
    camera_->setPosition(glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

    float rot_tab[] = {camera_->getHorizontalAngle(), camera_->getVerticalAngle()};
    ImGui::DragFloat2("Rotation", rot_tab, 0.01f);
    camera_->setRotation(rot_tab[0], rot_tab[1]);

    glm::vec3 dir = camera_->getDirectionVector();
    std::string dir_str = "Direction: [" + std::to_string(dir.x) + ", " + std::to_string(dir.y) +
        ", " + std::to_string(dir.z) + "]";
    ImGui::Text(dir_str.c_str());

    float near_plane = camera_->getNearPlane();
    ImGui::DragFloat("Near plane", &near_plane, 0.01f);
    float far_plane = camera_->getFarPlane();
    ImGui::DragFloat("Far plane", &far_plane, 0.01f);
    float fov = camera_->getFov();
    ImGui::SliderFloat("FOV", &fov, 0.01f, 2.65f);
    float aspect = camera_->getAspect();
    ImGui::DragFloat("Aspect", &aspect, 0.01f);
    camera_->setProjection(fov, aspect, near_plane, far_plane);

    ImGui::End();
}

void DefaultGuiRenderer::renderPostprocessDialog() {
    if (!render_postprocess_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(360, 240), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Postprocess", &render_postprocess_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    PostprocessEffect current = render_settings_->postprocess()->getEffect();
    GLuint selected_index = static_cast<GLuint>(current);
    const char *postprocess_types[] = {
        "None", "Negative", "Grayscale", "Sharpen", "Blur", "Edge", "Tint"};
    static const char *current_item = postprocess_types[selected_index];

    if (ImGui::BeginCombo("Type", current_item)) {
        for (unsigned int i = 0; i < IM_ARRAYSIZE(postprocess_types); i++) {
            bool is_selected = current_item == postprocess_types[i];
            if (ImGui::Selectable(postprocess_types[i], is_selected)) {
                current_item = postprocess_types[i];
                render_settings_->postprocess()->setEffect(static_cast<PostprocessEffect>(i));
            }
        }

        ImGui::EndCombo();
    }

    if (render_settings_->postprocess()->getEffect() == PostprocessEffect::Tint) {
        glm::vec3 color = render_settings_->postprocess()->getTintColor();
        float tint_color[] = {color.r, color.g, color.b, 1.0f};
        ImGui::ColorEdit3("Tint color", tint_color);
        render_settings_->postprocess()->setTintColor(
            glm::vec3(tint_color[0], tint_color[1], tint_color[2]));
    }

    if (render_settings_->postprocess()->getEffect() == PostprocessEffect::Blur ||
        render_settings_->postprocess()->getEffect() == PostprocessEffect::Edge ||
        render_settings_->postprocess()->getEffect() == PostprocessEffect::Sharpen) {
        float k_size = render_settings_->postprocess()->getKernelSize();
        ImGui::SliderFloat("Kernel size", &k_size, 1.0f, 500.0f);
        render_settings_->postprocess()->setKernelSize(k_size);
    }

    float gamma = render_settings_->postprocess()->getGamma();
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 10.0f);
    render_settings_->postprocess()->setGamma(gamma);

    float exposure = render_settings_->postprocess()->getExposure();
    ImGui::SliderFloat("Exposure", &exposure, 1.0f, 10.0f);
    render_settings_->postprocess()->setExposure(exposure);

    bool enable_bloom = render_settings_->postprocess()->isGlowBloomEnabled();
    ImGui::Checkbox("Glow bloom", &enable_bloom);
    render_settings_->postprocess()->enableGlowBloom(enable_bloom);

    if (enable_bloom) {
        glm::vec3 bloom_thresh = render_settings_->postprocess()->getGlowBloomThresholdColor();
        float thresh_color[] = {bloom_thresh.r, bloom_thresh.g, bloom_thresh.b};
        ImGui::ColorEdit3("Threshold color", thresh_color);
        render_settings_->postprocess()->setGlowBloomThresholdColor(
            glm::vec3(thresh_color[0], thresh_color[1], thresh_color[2]));
    }

    bool dof_enabled = render_settings_->postprocess()->isDepthOfFieldEnabled();
    ImGui::Checkbox("Depth of Field", &dof_enabled);
    render_settings_->postprocess()->enableDepthOfField(dof_enabled);

    if (dof_enabled) {
        float aperture = render_settings_->postprocess()->getAperture();
        ImGui::SliderFloat("Aperture", &aperture, 0.01f, 0.1f);
        render_settings_->postprocess()->setAperture(aperture);
        float focus = render_settings_->postprocess()->getFocusDistance();
        ImGui::SliderFloat("Distance", &focus, 0.0f, 1.0f);
        render_settings_->postprocess()->setFocusDistance(focus);
        float max_blur = render_settings_->postprocess()->getDepthOfFieldMaxBlur();
        ImGui::SliderFloat("Max blur", &max_blur, 0.0f, 1.0f);
        render_settings_->postprocess()->setDepthOfFieldMaxBlur(max_blur);
    }

    ImGui::Text("Wire frame");

    {
        WireframeMode current = render_settings_->wireframe()->getMode();
        GLuint selected_index = static_cast<GLuint>(current);
        const char *modes_str[] = {"None", "Overlay", "Full"};
        static const char *current_item = modes_str[selected_index];

        if (ImGui::BeginCombo("Mode", current_item)) {
            for (unsigned int i = 0; i < IM_ARRAYSIZE(modes_str); i++) {
                bool is_selected = (current_item == modes_str[i]);
                if (ImGui::Selectable(modes_str[i], is_selected)) {
                    current_item = modes_str[i];
                    render_settings_->wireframe()->setMode(static_cast<WireframeMode>(i));
                }
            }

            ImGui::EndCombo();
        }

        if (current != WireframeMode::None) {
            glm::vec3 wireframe_color = render_settings_->wireframe()->getColor();
            float color[] = {wireframe_color.r, wireframe_color.g, wireframe_color.b};
            ImGui::ColorEdit3("Color", color);
            render_settings_->wireframe()->setColor(glm::vec3(color[0], color[1], color[2]));

            int width = render_settings_->wireframe()->getLineWidth();
            ImGui::SliderInt("Line width", &width, 1, 15);
            render_settings_->wireframe()->setLineWidth(width);
        }
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderLightingDialog() {
    if (!render_lighting_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(485, 220), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Lighting", &render_lighting_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool enabled = render_settings_->lighting()->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    render_settings_->lighting()->enable(enabled);

    ImGui::Text("Directional light");
    glm::vec3 dir_light_direction =
        render_settings_->lighting()->directionalLight()->getDirection();
    float direction[3] = {dir_light_direction.x, dir_light_direction.y, dir_light_direction.z};
    ImGui::DragFloat3("Direction", direction, 0.1f);
    render_settings_->lighting()->directionalLight()->setDirection(
        glm::vec3(direction[0], direction[1], direction[2]));

    glm::vec3 ambient_dir_color =
        render_settings_->lighting()->directionalLight()->getAmbientColor();
    float amb_dir_color[] = {ambient_dir_color.r, ambient_dir_color.g, ambient_dir_color.b};
    ImGui::ColorEdit3("Ambient color", amb_dir_color);
    render_settings_->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(amb_dir_color[0], amb_dir_color[1], amb_dir_color[2]));

    glm::vec3 diffuse_dir_color =
        render_settings_->lighting()->directionalLight()->getDiffuseColor();
    float diff_dir_color[] = {diffuse_dir_color.r, diffuse_dir_color.g, diffuse_dir_color.b};
    ImGui::ColorEdit3("Diffuse color", diff_dir_color);
    render_settings_->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(diff_dir_color[0], diff_dir_color[1], diff_dir_color[2]));

    glm::vec3 specular_dir_color =
        render_settings_->lighting()->directionalLight()->getSpecularColor();
    float spec_dir_color[] = {specular_dir_color.r, specular_dir_color.g, specular_dir_color.b};
    ImGui::ColorEdit3("Specular color", spec_dir_color);
    render_settings_->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(spec_dir_color[0], spec_dir_color[1], spec_dir_color[2]));

    float emission_factor = render_settings_->lighting()->getEmissionFactor();
    ImGui::SliderFloat("Emission factor", &emission_factor, 0.0f, 30.0f);
    render_settings_->lighting()->setEmissionFactor(emission_factor);

    glm::vec3 skybox_light_color = render_settings_->lighting()->getSkyboxLightColor();
    float sb_light[] = {skybox_light_color.r, skybox_light_color.g, skybox_light_color.b};
    ImGui::ColorEdit3("Skybox lighting color", sb_light);
    render_settings_->lighting()->setSkyboxLightingColor(
        glm::vec3(sb_light[0], sb_light[1], sb_light[2]));

    ImGui::End();
}

void DefaultGuiRenderer::renderShadowMappingDialog() {
    if (!render_shadow_map_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(385, 385), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Shadow mapping", &render_shadow_map_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool shadow_mapping_enabled = render_settings_->lighting()->isShadowMappingEnabled();
    ImGui::Checkbox("Enabled", &shadow_mapping_enabled);
    render_settings_->lighting()->enableShadowMapping(shadow_mapping_enabled);

    float shadow_distance = render_settings_->lighting()->getShadowDistance();
    ImGui::SliderFloat("Shadow distance", &shadow_distance, 1.0f, 100.0f);
    render_settings_->lighting()->setShadowDistance(shadow_distance);

    int shadow_pcf_samples = render_settings_->lighting()->getShadowMappingPcfSamplesCount();
    ImGui::SliderInt("PCF Samples", &shadow_pcf_samples, 1, 10);
    render_settings_->lighting()->setShadowMappingPcfsamplesCount(shadow_pcf_samples);

    ImGui::Text("Shadow map");

    DefaultShadowMapRendererPtr sm_renderer =
        std::static_pointer_cast<DefaultShadowMapRenderer>(master_renderer_->shadowMapRenderer());
    DepthTextureBufferPtr shadow_map_texture = sm_renderer->getOutputData().shadow_map_texture;
    ImTextureID texture_handle = (void *)(shadow_map_texture->getHandle());
    ImGui::Image(texture_handle,
        ImVec2(shadow_map_texture->getWidth() / 4.0f, shadow_map_texture->getHeight() / 4.0f));

    ImGui::End();
}

void DefaultGuiRenderer::renderFogDialog() {
    if (!render_fog_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(296, 200), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Fog", &render_fog_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool enabled = render_settings_->fog()->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    render_settings_->fog()->enable(enabled);

    glm::vec3 fog_color = render_settings_->fog()->getColor();
    float color[] = {fog_color.r, fog_color.g, fog_color.b};
    ImGui::ColorEdit3("Color", color);
    render_settings_->fog()->setColor(glm::vec3(color[0], color[1], color[2]));

    float density = render_settings_->fog()->getDensity();
    ImGui::SliderFloat("Density", &density, 0.0f, 1.0f);
    render_settings_->fog()->setDensity(density);

    ImGui::Text("Skybox fog");

    float sb_density = render_settings_->fog()->getSkyboxFogOverallDensity();
    ImGui::SliderFloat("Density##SkyboxDensity", &sb_density, 0.0f, 1.0f);

    float transition = render_settings_->fog()->getSkyboxFogTransitionPower();
    ImGui::SliderFloat("Transition", &transition, 0.0f, 100.0f);

    float height = render_settings_->fog()->getSkyboxFogHeight();
    ImGui::SliderFloat("Height", &height, 0.0f, 1.0f);

    render_settings_->fog()->setSkyboxFog(sb_density, transition, height);

    ImGui::End();
}

void DefaultGuiRenderer::renderCaptureDialog() {
    if (!render_capture_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(400, 105), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Capture", &render_capture_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

	static std::string file_name = "screen_capture";
    ImGuiInputTextFlags flags = 0;
    flags |= ImGuiInputTextFlags_CharsNoBlank;
    ImGui::InputText("File name (*.png)", &file_name, flags);

	static bool add_timestamp = true;
    ImGui::Checkbox("Add timestamp", &add_timestamp);

	if (ImGui::Button("Save capture")) {
        master_renderer_->captureScreen(file_name, add_timestamp);
	}

	ImGui::End();
}

void DefaultGuiRenderer::renderWaterRendererDialog(ScenePtr scene) {
    if (!render_water_renderer_dialog_ || !scene) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(380, 260), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Water renderer", &render_water_renderer_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool water_renderer_enabled = master_renderer_->waterRenderer()->isEnabled();
    ImGui::Checkbox("Enabled", &water_renderer_enabled);
    master_renderer_->waterRenderer()->enable(water_renderer_enabled);

    DefaultWaterRendererPtr water_renderer =
        std::static_pointer_cast<DefaultWaterRenderer>(master_renderer_->waterRenderer());

    int texture_tiling = water_renderer->getTextureTiling();
    ImGui::SliderInt("Texture tiling", &texture_tiling, 1, 10);
    water_renderer->setTextureTiling(texture_tiling);

    ImGui::Text("Water render parameters");

    std::vector<WaterTilePtr> water_tiles;
    for (GLuint i = 0; i < scene->getWaterTilesCount(); i++) {
        water_tiles.push_back(scene->getWaterTile(i));
    }

    static WaterTilePtr selected_obj = nullptr;
    static std::string current_item = "";

    if (ImGui::BeginCombo("Water tile", current_item.c_str())) {
        if (water_tiles.size() != 0) {
            for (unsigned int i = 0; i < water_tiles.size(); i++) {
                bool is_selected = current_item == water_tiles[i]->getName();
                if (ImGui::Selectable(water_tiles[i]->getName().c_str(), is_selected)) {
                    current_item = water_tiles[i]->getName();
                    selected_obj = water_tiles[i];
                }
            }
        }

        ImGui::EndCombo();
    }

    if (selected_obj) {
        glm::vec3 water_color = selected_obj->getWaterColor();
        float color[] = {water_color.r, water_color.g, water_color.b};
        ImGui::ColorEdit3("Color", color);
        selected_obj->setWaterColor(glm::vec3(color[0], color[1], color[2]));

        float wave_strength = selected_obj->getWaveStrength();
        ImGui::SliderFloat("Wave strength", &wave_strength, 0.0f, 10.0f);
        selected_obj->setWaveStrength(wave_strength);

        float wave_speed = selected_obj->getWaveSpeed();
        ImGui::SliderFloat("Wave speed", &wave_speed, 0.0f, 10.0f);
        selected_obj->setWaveSpeed(wave_speed);

        int shininess = selected_obj->getShininess();
        ImGui::SliderInt("Shininess", &shininess, 1, 100);
        selected_obj->setShininess(shininess);

        float ambient_factor = selected_obj->getAmbientFactor();
        ImGui::SliderFloat("Ambient factor", &ambient_factor, 0.0f, 10.0f);
        selected_obj->setAmbientFactor(ambient_factor);

        float specular_factor = selected_obj->getSpecularFactor();
        ImGui::SliderFloat("Specular factor", &specular_factor, 0.0f, 10.0f);
        selected_obj->setSpecularFactor(specular_factor);
    }
    else {
        ImGui::Text("No water object selected");
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderSkyboxRendererDialog(ScenePtr scene) {
    if (!render_skybox_renderer_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(280, 135), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Skybox renderer", &render_skybox_renderer_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool skybox_renderer_enabled = master_renderer_->skyboxRenderer()->isEnabled();
    ImGui::Checkbox("Enabled", &skybox_renderer_enabled);
    master_renderer_->skyboxRenderer()->enable(skybox_renderer_enabled);

    DefaultSkyboxRendererPtr skybox_renderer =
        std::static_pointer_cast<DefaultSkyboxRenderer>(master_renderer_->skyboxRenderer());

    ImGui::Text("Skybox render parameters");

    SkyboxPtr skybox = scene->getSkybox();
    if (!skybox) {
        ImGui::Text("No skybox in scene");
    }
    else {
        ImGui::Text(std::string("Name: " + skybox->getName()).c_str());
        ImGui::Text("No configuration is available yet");
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderMeshRendererDialog(ScenePtr scene) {
    if (!render_mesh_renderer_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(280, 135), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Mesh renderer", &render_mesh_renderer_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool mesh_renderer_enabled = master_renderer_->meshRenderer()->isEnabled();
    ImGui::Checkbox("Enabled", &mesh_renderer_enabled);
    master_renderer_->meshRenderer()->enable(mesh_renderer_enabled);

    DefaultMeshRendererPtr mesh_renderer =
        std::static_pointer_cast<DefaultMeshRenderer>(master_renderer_->meshRenderer());

    ImGui::End();
}