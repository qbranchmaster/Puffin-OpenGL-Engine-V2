/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultGuiRenderer.hpp"

using namespace puffin;

DefaultGuiRenderer::DefaultGuiRenderer(RenderSettingsPtr render_settings,
    RenderersSharedDataPtr renderers_shared_data, WindowPtr window,
    CameraPtr camera, MasterRendererPtr master_renderer) {
    if (!window || !render_settings || !renderers_shared_data || !camera ||
        !master_renderer) {
        throw Exception("ConfigGuiRenderer::ConfigGuiRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    target_window_ = window;
    renderers_shared_data_ = renderers_shared_data;
    camera_ = camera;
    master_renderer_ = master_renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    setupImGui();

    const char *glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(target_window_->handle_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void DefaultGuiRenderer::render() {
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

    //ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLboolean DefaultGuiRenderer::isCapturingMouse() const {
    return ImGui::GetIO().WantCaptureMouse;
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

    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();
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

    if (!ImGui::Begin("About Puffin Engine", &render_about_dialog_,
        window_flags)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Puffin Engine ver. 2.0");
    ImGui::Separator();
    ImGui::Text("Coded by: Sebastian 'qbranchmaster' Tabaka");
    ImGui::Text("Contact:  sebastian.tabaka@outlook.com");
    ImGui::Text("Repo URL:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f),
        "https://github.com/qbranchmaster/Puffin-OpenGL-Engine-V2");
    if (ImGui::IsItemClicked()) {
        // TODO: Open URL
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderCameraDialog() {
    if (!render_camera_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(350, 195), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Camera", &render_camera_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    glm::vec3 position = camera_->getPosition();
    float cam_pos[] = {position.x, position.y, position.z};
    ImGui::DragFloat3("Position", cam_pos, 0.01f);
    camera_->setPosition(glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

    float rot_tab[] = {camera_->getHorizontalAngle(),
        camera_->getVerticalAngle()};
    ImGui::DragFloat2("Rotation", rot_tab, 0.01f);
    camera_->setRotation(rot_tab[0], rot_tab[1]);

    glm::vec3 dir = camera_->getDirectionVector();
    std::string dir_str = "Direction: [" + std::to_string(dir.x) + ", " +
        std::to_string(dir.y) + ", " + std::to_string(dir.z) + "]";
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
    ImGui::SetNextWindowSize(ImVec2(360, 240), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Postprocess", &render_postprocess_dialog_,
        window_flags)) {
        ImGui::End();
        return;
    }

    PostprocessEffect current = render_settings_->postprocess()->getEffect();
    GLuint selected_index = static_cast<GLuint>(current);
    const char *postprocess_types[] = {"None", "Negative", "Grayscale",
        "Sharpen", "Blur", "Edge", "Tint"};
    static const char *current_item = postprocess_types[selected_index];

    if (ImGui::BeginCombo("Type", current_item)) {
        for (unsigned int i = 0; i < IM_ARRAYSIZE(postprocess_types); i++) {
            bool is_selected = current_item == postprocess_types[i];
            if (ImGui::Selectable(postprocess_types[i], is_selected)) {
                current_item = postprocess_types[i];
                render_settings_->postprocess()->setEffect(
                    static_cast<PostprocessEffect>(i));
            }
        }

        ImGui::EndCombo();
    }

    if (render_settings_->postprocess()->getEffect() ==
        PostprocessEffect::TINT) {
        glm::vec3 color = render_settings_->postprocess()->getTintColor();
        float tint_color[] = {color.r, color.g, color.b, 1.0f};
        ImGui::ColorEdit3("Tint color", tint_color);
        render_settings_->postprocess()->setTintColor(glm::vec3(tint_color[0],
            tint_color[1], tint_color[2]));
    }

    if (render_settings_->postprocess()->getEffect() ==
        PostprocessEffect::BLUR ||
        render_settings_->postprocess()->getEffect() ==
        PostprocessEffect::EDGE ||
        render_settings_->postprocess()->getEffect() ==
        PostprocessEffect::SHARPEN) {
        float k_size = render_settings_->postprocess()->getKernelSize();
        ImGui::SliderFloat("Kernel size", &k_size, 1.0f, 500.0f);
        render_settings_->postprocess()->setKernelSize(k_size);
    }

    float gamma = render_settings_->getGamma();
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 10.0f);
    render_settings_->setGamma(gamma);

    float exposure = render_settings_->getExposure();
    ImGui::SliderFloat("Exposure", &exposure, 1.0f, 10.0f);
    render_settings_->setExposure(exposure);

    bool enable_bloom = render_settings_->postprocess()->isGlowBloomEnabled();
    ImGui::Checkbox("Glow bloom", &enable_bloom);
    render_settings_->postprocess()->enableGlowBloom(enable_bloom);

    if (enable_bloom) {
        glm::vec3 bloom_thresh = render_settings_->postprocess()->
            getGlowBloomThresholdColor();
        float thresh_color[] = {bloom_thresh.r, bloom_thresh.g,
            bloom_thresh.b};
        ImGui::ColorEdit3("Threshold color", thresh_color);
        render_settings_->postprocess()->setGlowBloomThresholdColor(glm::vec3(
            thresh_color[0], thresh_color[1], thresh_color[2]));
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
        float max_blur = render_settings_->postprocess()->
            getDepthOfFieldMaxBlur();
        ImGui::SliderFloat("Max blur", &max_blur, 0.0f, 1.0f);
        render_settings_->postprocess()->setDepthOfFieldMaxBlur(max_blur);
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderLightingDialog() {
    if (!render_lighting_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(390, 195), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Lighting", &render_lighting_dialog_,
        window_flags)) {
        ImGui::End();
        return;
    }

    bool enabled = render_settings_->lighting()->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    render_settings_->lighting()->enable(enabled);

    ImGui::Text("Directional light");
    glm::vec3 dir_light_direction = render_settings_->lighting()->
        directionalLight()->getDirection();
    float direction[3] = {dir_light_direction.x, dir_light_direction.y,
        dir_light_direction.z};
    ImGui::DragFloat3("Direction", direction, 0.1f);
    render_settings_->lighting()->directionalLight()->setDirection(
        glm::vec3(direction[0], direction[1], direction[2]));

    glm::vec3 ambient_dir_color = render_settings_->lighting()->
        directionalLight()->getAmbientColor();
    float amb_dir_color[] = {ambient_dir_color.r, ambient_dir_color.g,
        ambient_dir_color.b};
    ImGui::ColorEdit3("Ambient color", amb_dir_color);
    render_settings_->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(amb_dir_color[0], amb_dir_color[1], amb_dir_color[2]));

    glm::vec3 diffuse_dir_color = render_settings_->lighting()->
        directionalLight()->getDiffuseColor();
    float diff_dir_color[] = {diffuse_dir_color.r, diffuse_dir_color.g,
        diffuse_dir_color.b};
    ImGui::ColorEdit3("Diffuse color", diff_dir_color);
    render_settings_->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(diff_dir_color[0], diff_dir_color[1], diff_dir_color[2]));

    glm::vec3 specular_dir_color = render_settings_->lighting()->
        directionalLight()->getSpecularColor();
    float spec_dir_color[] = {specular_dir_color.r, specular_dir_color.g,
        specular_dir_color.b};
    ImGui::ColorEdit3("Specular color", spec_dir_color);
    render_settings_->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(spec_dir_color[0], spec_dir_color[1], spec_dir_color[2]));

    float emission_factor = render_settings_->lighting()->getEmissionFactor();
    ImGui::SliderFloat("Emission factor", &emission_factor, 0.0f, 30.0f);
    render_settings_->lighting()->setEmissionFactor(emission_factor);

    ImGui::End();
}

void DefaultGuiRenderer::renderShadowMappingDialog() {
    if (!render_shadow_map_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    ImGui::SetNextWindowSize(ImVec2(385, 385), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Shadow mapping", &render_shadow_map_dialog_,
        window_flags)) {
        ImGui::End();
        return;
    }

    bool shadow_mapping_enabled = render_settings_->lighting()->
        isShadowMappingEnabled();
    ImGui::Checkbox("Enabled", &shadow_mapping_enabled);
    render_settings_->lighting()->enableShadowMapping(shadow_mapping_enabled);

    float shadow_distance = render_settings_->lighting()->getShadowDistance();
    ImGui::SliderFloat("Shadow distance", &shadow_distance, 1.0f, 100.0f);
    render_settings_->lighting()->setShadowDistance(shadow_distance);

    int shadow_pcf_samples = render_settings_->lighting()->
        getShadowMappingPcfSamplesCount();
    ImGui::SliderInt("PCF Samples", &shadow_pcf_samples, 1, 10);
    render_settings_->lighting()->setShadowMappingPcfsamplesCount(
        shadow_pcf_samples);

    ImGui::Text("Shadow map");
    DepthTextureBufferPtr shadow_map_texture =
        renderers_shared_data_->shadow_map_texture;
    ImTextureID texture_handle = (void*)(shadow_map_texture->getHandle());
    ImGui::Image(texture_handle, ImVec2(shadow_map_texture->getWidth() / 4.0f,
        shadow_map_texture->getHeight() / 4.0f));

    ImGui::End();
}