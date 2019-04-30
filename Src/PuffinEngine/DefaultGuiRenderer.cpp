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
    ImGui_ImplGlfw_InitForOpenGL(target_window_->getHandle(), true);
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

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLboolean DefaultGuiRenderer::isCapturingMouse() const {
    return ImGui::GetIO().WantCaptureMouse;
}

void DefaultGuiRenderer::setupImGui() {
    ImGuiIO &io = ImGui::GetIO();

    ImGuiStyle &style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
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

        if (ImGui::BeginMenu("Edit")) {
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

void DefaultGuiRenderer::postprocessDialog() {


    ImGui::Begin("Postprocess");

    auto current = render_settings_->postprocess()->getEffect();
    GLuint selected_index = static_cast<GLuint>(current);
    const char *postprocess_types[] = {"None", "Negative", "Grayscale",
        "Sharpen", "Blur", "Edge", "Tint"};
    static const char *current_item = postprocess_types[selected_index];

    auto color = render_settings_->postprocess()->getTintColor();
    ImVec4 tint_color = ImVec4(color.r, color.g, color.b, 1.0f);

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

    ImGui::ColorEdit3("Tint color", (float*)&tint_color);
    render_settings_->postprocess()->setTintColor(glm::vec3(tint_color.x,
        tint_color.y, tint_color.z));

    auto k_size = render_settings_->postprocess()->getKernelSize();
    ImGui::InputFloat("Kernel size", &k_size, 1.0f, 5.0f, "%.2f");
    render_settings_->postprocess()->setKernelSize(k_size);

    float gamma = render_settings_->getGamma();
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);
    render_settings_->setGamma(gamma);

    float exposure = render_settings_->getExposure();
    ImGui::SliderFloat("Exposure", &exposure, 0.1f, 10.0f);
    render_settings_->setExposure(exposure);

    bool enable_bloom = render_settings_->postprocess()->isGlowBloomEnabled();
    ImGui::Checkbox("Glow bloom", &enable_bloom);
    render_settings_->postprocess()->enableGlowBloom(enable_bloom);

    auto bloom_thresh = render_settings_->postprocess()->
        getGlowBloomThresholdColor();
    ImVec4 thresh_color = ImVec4(bloom_thresh.r, bloom_thresh.g,
        bloom_thresh.b, 1.0f);
    ImGui::ColorEdit3("Glow bloom threshold color", (float*)&thresh_color);
    render_settings_->postprocess()->setGlowBloomThresholdColor(glm::vec3(
        thresh_color.x, thresh_color.y, thresh_color.z));

    ImGui::End();
}

void DefaultGuiRenderer::lightingDialog() {
    ImGui::Begin("Lighting");

    bool enabled = render_settings_->lighting()->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    render_settings_->lighting()->enable(enabled);

    ImGui::Text("Directional light");
    auto dir_light_direction = render_settings_->lighting()->
        directionalLight()->getDirection();
    float direction[3] = {dir_light_direction.x, dir_light_direction.y,
        dir_light_direction.z};
    ImGui::InputFloat3("Direction", direction, 2);
    render_settings_->lighting()->directionalLight()->setDirection(
        glm::vec3(direction[0], direction[1], direction[2]));

    auto ambient_dir_color = render_settings_->lighting()->
        directionalLight()->getAmbientColor();
    ImVec4 amb_dir_color(ambient_dir_color.r, ambient_dir_color.g,
        ambient_dir_color.b, 1.0f);
    ImGui::ColorEdit3("Ambient color", (float*)&amb_dir_color);
    render_settings_->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(amb_dir_color.x, amb_dir_color.y, amb_dir_color.z));

    auto diffuse_dir_color = render_settings_->lighting()->
        directionalLight()->getDiffuseColor();
    ImVec4 diff_dir_color(diffuse_dir_color.r, diffuse_dir_color.g,
        diffuse_dir_color.b, 1.0f);
    ImGui::ColorEdit3("Diffuse color", (float*)&diff_dir_color);
    render_settings_->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(diff_dir_color.x, diff_dir_color.y, diff_dir_color.z));

    auto specular_dir_color = render_settings_->lighting()->
        directionalLight()->getSpecularColor();
    ImVec4 spec_dir_color(specular_dir_color.r, specular_dir_color.g,
        specular_dir_color.b, 1.0f);
    ImGui::ColorEdit3("Specular color", (float*)&spec_dir_color);
    render_settings_->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(spec_dir_color.x, spec_dir_color.y, spec_dir_color.z));

    auto emission_factor = render_settings_->lighting()->getEmissionFactor();
    ImGui::SliderFloat("Emission factor", &emission_factor, 0.0f, 30.0f);
    render_settings_->lighting()->setEmissionFactor(emission_factor);

    ImGui::End();
}

void DefaultGuiRenderer::shadowMappingDialog() {
    ImGui::Begin("Shadow mapping");

    bool shadow_mapping_enabled = render_settings_->lighting()->
        isShadowMappingEnabled();
    ImGui::Checkbox("Enabled", &shadow_mapping_enabled);
    render_settings_->lighting()->enableShadowMapping(shadow_mapping_enabled);

    auto shadow_distance = render_settings_->lighting()->getShadowDistance();
    ImGui::SliderFloat("Shadow distance", &shadow_distance, 5.0f, 100.0f);
    render_settings_->lighting()->setShadowDistance(shadow_distance);

    auto shadow_pcf_samples = static_cast<GLint>(render_settings_->lighting()->
        getShadowMappingPcfSamplesCount());
    ImGui::SliderInt("PCF Samples", &shadow_pcf_samples, 1, 15);
    render_settings_->lighting()->setShadowMappingPcfsamplesCount(
        shadow_pcf_samples);

    ImGui::End();
}

void DefaultGuiRenderer::debugDialog() {
    ImGui::Begin("Debug");

    ImGui::ShowMetricsWindow();

    ImGui::Text("Directional light depth map:");
    auto shadow_map_texture = renderers_shared_data_->shadow_map_texture;
    ImTextureID texture_handle = (void*)(shadow_map_texture->getHandle());
    ImGui::Image(texture_handle, ImVec2(shadow_map_texture->getWidth() / 4.0f,
        shadow_map_texture->getHeight() / 4.0f));

    ImGui::End();
}

void DefaultGuiRenderer::cameraDialog() {
    ImGui::Begin("Camera");

    auto position = camera_->getPosition();
    float cam_pos[3] = {position.x, position.y, position.z};
    ImGui::InputFloat3("Position", cam_pos, 2);
    camera_->setPosition(glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

    auto near_plane = camera_->getNearPlane();
    ImGui::InputFloat("Near plane", &near_plane, 0.1f, 1.0f, 2);
    auto far_plane = camera_->getFarPlane();
    ImGui::InputFloat("Far plane", &far_plane, 0.1f, 1.0f, 2);
    auto fov = camera_->getFov();
    ImGui::InputFloat("FOV", &fov, 0.1f, 1.0f, 2);
    auto aspect = camera_->getAspect();
    ImGui::InputFloat("Aspect", &aspect, 0.1f, 1.0f, 2);

    camera_->setProjection(fov, aspect, near_plane, far_plane);

    bool dof_enabled = render_settings_->postprocess()->isDepthOfFieldEnabled();
    ImGui::Checkbox("Depth of Field", &dof_enabled);
    render_settings_->postprocess()->enableDepthOfField(dof_enabled);

    auto aperture = camera_->getAperture();
    ImGui::InputFloat("Aperture", &aperture, 0.1f, 1.0f, 2);
    camera_->setAperture(aperture);
    auto focus = camera_->getFocusDistance();
    ImGui::InputFloat("Focus distance", &focus, 0.1f, 1.0f, 2);
    camera_->setFocusDistance(focus);
    auto max_blur = render_settings_->postprocess()->getDepthOfFieldMaxBlur();
    ImGui::InputFloat("Max blur", &max_blur, 0.1f, 1.0f, 2);
    render_settings_->postprocess()->setDepthOfFieldMaxBlur(max_blur);

    ImGui::End();
}