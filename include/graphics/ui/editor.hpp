#pragma once

#include <graphics/ui.hpp>
#include <scene/scene.hpp>
#include <system/timer.hpp>
#include <system/log.hpp>

#include <filesystem>
#include <functional>

namespace Graphics
{
    class CEditor
    {
    public:
        CEditor(CScene& scene);
        ~CEditor();
    
        void Update();

    private:
        void ShowDockSpace(bool* p_open);
        static void ShowExplorerRec(std::filesystem::path path, std::filesystem::path& selected);
        static bool ShowFileExplorer(const std::string& name, std::function<void(const std::string&)> openCb);
        // returns id of selected object
        uint32_t ShowSceneHierarhy(bool* p_open, CScene& scene);
        void ShowObjectInspector(bool* p_open, CObject* object);
        void ShowFPS(bool* p_open);
        void ShowLog(bool* p_open);

    private:
        CScene& m_scene;
        CObject* m_selected;

        bool m_showLog, m_showOverlay, m_showInspector, m_showHierarhy, m_showDock;
    };
};