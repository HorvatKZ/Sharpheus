#include "editor_pch.h"
#include "ClassListCtrl.hpp"
#include "Editor/Registry/ClassRegistry.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus {

    const wxColour ClassListCtrl::inativeColour(128, 128, 128);
    const wxFont ClassListCtrl::inactiveFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
    const std::vector<ClassListCtrl::Info> ClassListCtrl::infos = {
            Info("GameObject", "gameobj.png", GameObject::Type::None, 0),
            Info("Collection", "collection.png", GameObject::Type::Collection, 1),
            Info("Rendering", "rendering.png", GameObject::Type::None, 1),
            Info("Camera", "camera.png", GameObject::Type::Camera, 2),
            Info("Sprite", "sprite.png", GameObject::Type::Sprite, 2),
            Info("Quad", "quad.png", GameObject::Type::Quad, 2),
            Info("Text", "text.png", GameObject::Type::Text, 2),
            Info("AnimatedSprite", "animsprite.png", GameObject::Type::AnimatedSprite, 2),
            Info("AnimationPlayer", "animplayer.png", GameObject::Type::AnimationPlayer, 2),
            Info("TileMap", "tilemap.png", GameObject::Type::TileMap, 2),
            Info("ParticleEmitter", "particleemitter.png", GameObject::Type::ParticleEmitter, 2),
            Info("Physics", "physics.png", GameObject::Type::None, 1),
            Info("PhysicsObject", "physicsobj.png", GameObject::Type::PhysicsObject, 2),
            Info("Colliders", "colliders.png", GameObject::Type::None, 2),
            Info("BoxCollider", "boxcollider.png", GameObject::Type::BoxCollider, 3),
            Info("CircleCollider", "circlecollider.png", GameObject::Type::CircleCollider, 3),
            Info("CapsuleCollider", "capsulecollider.png", GameObject::Type::CapsuleCollider, 3),
            Info("Sounds", "sounds.png", GameObject::Type::None, 1),
            Info("MusicPlayer", "musicplayer.png", GameObject::Type::MusicPlayer, 2),
            Info("SoundEffects", "soundeffects.png", GameObject::Type::SoundEffects, 2),
            Info("Controls", "controls.png", GameObject::Type::None, 1),
            Info("Button", "button.png", GameObject::Type::Button, 2),
            Info("ImageButton", "imagebutton.png", GameObject::Type::ImageButton, 2),
            Info("CheckBox", "checkbox.png", GameObject::Type::CheckBox, 2),
            Info("RadioButton", "radio.png", GameObject::Type::RadioButton, 2),
            Info("Behavior", "behavior.png", GameObject::Type::Behavior, 1)
    };


    ClassListCtrl::ClassListCtrl()
    {
    }

    ClassListCtrl::~ClassListCtrl()
    {
    }

    void ClassListCtrl::Init()
    {
    }

    void ClassListCtrl::InitContent(std::function<void(const wxString&, GameObject::Type)>&& selectCallback, std::function<void()>&& dclickCallback)
    {
        this->selectCallback = std::move(selectCallback);
        this->dclickCallback = std::move(dclickCallback);
        Fill();
        Bind(wxEVT_TREE_SEL_CHANGED, &ClassListCtrl::OnSelect, this);
        Bind(wxEVT_LEFT_DCLICK, &ClassListCtrl::OnDoubleClick, this);
    }

    bool ClassListCtrl::Create(wxWindow* parent)
    {
        return wxTreeCtrl::Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
    }

    wxWindow* ClassListCtrl::GetControl()
    {
        return this;
    }

    wxString ClassListCtrl::GetStringValue() const
    {
        wxTreeItemId selected = GetSelection();
        if (selected.IsOk()) {
            return GetItemText(selected);
        }
    }

    void ClassListCtrl::OnSelect(wxTreeEvent& e)
    {
        wxString selectedName = GetItemText(GetSelection());
        if (nameToType[selectedName] == GameObject::Type::None) {
            Unselect();
            selectCallback("", GameObject::Type::None);
        } else {
            selectCallback(selectedName, nameToType[selectedName]);
        }
       
    }

    void ClassListCtrl::OnDoubleClick(wxMouseEvent& e)
    {
        dclickCallback();
    }

    void ClassListCtrl::Fill()
    {
        wxImageList* images = new wxImageList(16, 16);
        for (const Info& info : infos) {
            images->Add(ImageManager::GetImage(info.path, ImageManager::PathType::GAMEOBJECTS));
        }
        AssignImageList(images);

        wxTreeItemId prev = AddRoot(infos[0].name, 0);
        SetItemTextColour(prev, inativeColour);
        SetItemFont(prev, inactiveFont);
        nameToType[infos[0].name] = infos[0].type;

        for (uint32 i = 1; i < infos.size(); ++i) {
            uint32 prevLevel = infos[i - 1].level;
            for (uint32 j = prevLevel; j >= infos[i].level; --j) {
                GetItemText(prev);
                prev = GetItemParent(prev);
            }
            prev = AppendItem(prev, infos[i].name, i);

            if (infos[i].type == GameObject::Type::None) {
                SetItemTextColour(prev, inativeColour);
                SetItemFont(prev, inactiveFont);
            }
            nameToType[infos[i].name] = infos[i].type;
        }

        SetIndent(10);
        Expand(GetRootItem());
    }

}