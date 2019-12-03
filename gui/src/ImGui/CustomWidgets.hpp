#pragma once

namespace ImGui {

bool MiniSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, bool top, float power);
bool MiniSliderFloat(const char* label, float* v, float v_min, float v_max, bool top = true, float power = 1.0f);

} // namespace ImGui