#include "common.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "script.hpp"
#include "pointers.hpp"
#include "renderer.hpp"

#include "utility/features/all.hpp"
#include "settings.hpp"

#include <imgui.h>

#include "menu/player_menu.h"
#include "menu/player_stats.h"
#include "menu/setting_menu.h"

namespace big
{
	void gui::get_entity()
	{
		if (g_settings->player.esp)
		{
			g_gui.m_entity_list.clear();

			auto level_array = (*g_pointers->m_world)->m_level;

			for (int j = 0; j < level_array.count(); j++)
			{
				auto level_data = level_array[j];
				if (!level_array.valid(j)) continue;

				auto actor_array = level_data->m_actor;

				for (int i = 0; i < actor_array.count(); i++)
				{
					auto actor = actor_array[i];
					if (!actor_array.valid(i)) continue;

					if (int id = actor->m_name_index)
					{
						auto name = unreal_engine::get_name(id);
						if (!actor->valid_root_component()) continue;

						if (auto root_component = actor->m_root_component)
						{
							auto pos = root_component->m_relative_location;
							if (auto player = unreal_engine::get_local_player())
							{
								if (auto control = player->m_player_controller)
								{
									if (auto camera_mgr = control->m_camera_manager)
									{
										auto location = camera_mgr->m_camera_cache.project_world_to_screen(pos);
										//g_logger->info("Location : %f | %f | %f", location.x, location.y, location.z);
										m_entity_list[name] = { location.x, location.y, location.z };
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void gui::script_on_tick_timed()
	{
		TRY_CLAUSE
		{
			static auto start = std::chrono::steady_clock::now();
			if ((std::chrono::steady_clock::now() - start).count() >= std::chrono::milliseconds(1000).count())
			{
				//if (unreal_engine::world_state())
					//get_entity();

				start = std::chrono::steady_clock::now();
			}
		} EXCEPT_CLAUSE
	}

	void gui::dx_init()
	{
		auto &style = ImGui::GetStyle();
		style.WindowPadding = { 10.f, 10.f };
		style.PopupRounding = 0.f;
		style.FramePadding = { 8.f, 4.f };
		style.ItemSpacing = { 10.f, 8.f };
		style.ItemInnerSpacing = { 6.f, 6.f };
		style.TouchExtraPadding = { 0.f, 0.f };
		style.IndentSpacing = 21.f;
		style.ScrollbarSize = 15.f;
		style.GrabMinSize = 8.f;
		style.WindowBorderSize = 1.f;
		style.ChildBorderSize = 0.f;
		style.PopupBorderSize = 1.f;
		style.FrameBorderSize = 0.f;
		style.TabBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.ChildRounding = 0.f;
		style.FrameRounding = 0.f;
		style.ScrollbarRounding = 0.f;
		style.GrabRounding = 0.f;
		style.TabRounding = 0.f;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.DisplaySafeAreaPadding = { 3.f, 3.f };

		auto &colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.90f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.90f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.41f, 0.41f, 0.41f, 0.74f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
		colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
		colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
		colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void gui::dx_on_tick()
	{
		TRY_CLAUSE
		{
			if (ImGui::Begin("[Internal] Tower of Fantasy"))
			{
				ImGui::BeginTabBar("tabbar");
				player_menu::render_menu();
				player_stat::render_menu();
				setting_menu::render_menu();
				ImGui::EndTabBar();
			}
			ImGui::End();
		} EXCEPT_CLAUSE
	}

	void gui::script_init()
	{
		g_gui.m_opened = true;
	}

	void gui::script_on_tick()
	{
		if (unreal_engine::game_state())
		{
			TRY_CLAUSE
			{
				attack::rapid_attack(g_settings->player.fast_attack);
				
				movement::infinite_jump(g_settings->player.infinite_jump);
				movement::infinite_dodge(g_settings->player.infinite_dodge);
			} EXCEPT_CLAUSE
		}
	}

	void gui::script_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.script_on_tick();
			g_gui.script_on_tick_timed();
		}
	}
}
