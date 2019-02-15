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
    gammaDialog();

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

    ImGui::End();
}

void ConfigGuiRenderer::lightingDialog() {
    ImGui::Begin("Lighting");

    float exposure = render_settings_->getExposure();
    ImGui::SliderFloat("Exposure", &exposure, 0.1f, 10.0f);
    render_settings_->setExposure(exposure);

    ImGui::End();
}

void ConfigGuiRenderer::gammaDialog() {
    ImGui::Begin("Gamma");

    float gamma = render_settings_->getGamma();
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);
    render_settings_->setGamma(gamma);

    ImGui::End();
}