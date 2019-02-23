/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/ConfigGuiRenderer.hpp"

using namespace puffin;

ConfigGuiRenderer::ConfigGuiRenderer(WindowPtr window,
    RenderSettingsPtr render_settings) {
    if (!window || !render_settings) {
        throw Exception("ConfigGuiRenderer::ConfigGuiRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    target_window_ = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    setupImGui();

    const char *glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(target_window_->handle_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void ConfigGuiRenderer::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    lightingDialog();
    postprocessDialog();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ConfigGuiRenderer::setupImGui() {
    ImGuiIO &io = ImGui::GetIO();

    ImGuiStyle &style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
}

void ConfigGuiRenderer::postprocessDialog() {
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

void ConfigGuiRenderer::lightingDialog() {
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

    ImGui::End();
}