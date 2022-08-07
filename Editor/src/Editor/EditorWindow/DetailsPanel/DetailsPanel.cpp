#include "editor_pch.h"
#include "DetailsPanel.hpp"
#include "Editor/Registry/ClassRegistry.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "BehaviorCreator.hpp"
#include "BehaviorPicker.hpp"
#include "PresenterDefs.hpp"


namespace Sharpheus {

	DetailsPanel::DetailsPanel(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxScrolledWindow(parent, wxID_ANY, pos, size), freeY(60)
	{
		ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

		title = new wxStaticText(this, wxID_ANY, "Details", wxPoint(10, 10));
		title->SetFont(wxFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

		typeName = new wxStaticText(this, wxID_ANY, "", wxPoint(30, 35));
		typeIcon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(10, 35), wxSize(16, 16));
		headerPresenter = new HeaderPresenter(this, "Name", std::move(currDataChangedCallback), std::move(currNameChangedCallback), freeY);
		mainTrafo = new MainTrafoPresenter(this, "Transform", std::move(currTrafoChangedCallback), freeY);
	}


	DetailsPanel::~DetailsPanel()
	{
		ClearPresenters();
		delete mainTrafo;
		delete headerPresenter;
	}


	void DetailsPanel::BindCallbacks(std::function<void(const std::string&, const std::string&)>&& currNameChangedCallback,
		std::function<void()>&& currDataChangedCallback, std::function<void()>&& currTrafoChangedCallback, std::function<void(uint32)>&& behaviorChangedCallback)
	{
		this->currNameChangedCallback = std::move(currNameChangedCallback);
		this->currDataChangedCallback = std::move(currDataChangedCallback);
		this->currTrafoChangedCallback = std::move(currTrafoChangedCallback);
		this->behaviorChangedCallback = std::move(behaviorChangedCallback);
	}


	void DetailsPanel::CurrentChanged(GameObject* curr)
	{
		if (curr != nullptr) {
			if (curr->GetType() == GameObject::Type::Behavior) {
				if (lastType != GameObject::Type::Behavior || lastSubType != ((Behavior*)curr)->GetSubType()) {
					lastType = GameObject::Type::Behavior;
					lastSubType = ((Behavior*)curr)->GetSubType();
					CreatePresenters(curr);
				}
			} else {
				if (curr->GetType() != lastType) {
					lastType = curr->GetType();
					CreatePresenters(curr);
				}
			}
			FillPresenters(curr);
		} else {
			SetPresentersToDefault();
		}
	}


	void DetailsPanel::CurrentNameChanged()
	{
		headerPresenter->Refresh();
	}


	void DetailsPanel::CurrentDataChanged()
	{
		RefreshPresenters();
	}


	void DetailsPanel::CurrentTrafoChanged()
	{
		mainTrafo->Refresh();
	}


	void DetailsPanel::CreatePresenters(GameObject* obj)
	{
		ClearPresenters();

		ClassInfoPtr classInfo = ClassRegistry::GetClassInfo(obj);
		GameObject::Type type = obj->GetType();
		if (classInfo == nullptr) {
			SPHE_ERROR("Class Registry: Unexpected GameObject type {0}", type);
			return;
		}

		Scroll(0, 0);
		uint32 y = freeY;
		for (CommonProvider* provider : classInfo->GetProviders()) {
			switch (type) {
				case GameObject::Type::Collection:
					CreatePresenterFrom<Collection>(provider, y);
					break;
				case GameObject::Type::Camera:
					CreatePresenterFrom<Camera>(provider, y);
					break;
				case GameObject::Type::Sprite:
					CreatePresenterFrom<Sprite>(provider, y);
					break;
				case GameObject::Type::Quad:
					CreatePresenterFrom<Quad>(provider, y);
					break;
				case GameObject::Type::Text:
					CreatePresenterFrom<Text>(provider, y);
					break;
				case GameObject::Type::AnimatedSprite:
					CreatePresenterFrom<AnimatedSprite>(provider, y);
					break;
				case GameObject::Type::AnimationPlayer:
					CreatePresenterFrom<AnimationPlayer>(provider, y);
					break;
				case GameObject::Type::TileMap:
					CreatePresenterFrom<TileMap>(provider, y);
					break;
				case GameObject::Type::ParticleEmitter:
					CreatePresenterFrom<ParticleEmitter>(provider, y);
					break;
				case GameObject::Type::PhysicsObject:
					CreatePresenterFrom<PhysicsObject>(provider, y);
					break;
				case GameObject::Type::BoxCollider:
					CreatePresenterFrom<BoxCollider>(provider, y);
					break;
				case GameObject::Type::CircleCollider:
					CreatePresenterFrom<CircleCollider>(provider, y);
					break;
				case GameObject::Type::CapsuleCollider:
					CreatePresenterFrom<CapsuleCollider>(provider, y);
					break;
				case GameObject::Type::MusicPlayer:
					CreatePresenterFrom<MusicPlayer>(provider, y);
					break;
				case GameObject::Type::SoundEffects:
					CreatePresenterFrom<SoundEffects>(provider, y);
					break;
				case GameObject::Type::Button:
					CreatePresenterFrom<Button>(provider, y);
					break;
				case GameObject::Type::ImageButton:
					CreatePresenterFrom<ImageButton>(provider, y);
					break;
				case GameObject::Type::CheckBox:
					CreatePresenterFrom<CheckBox>(provider, y);
					break;
				case GameObject::Type::RadioButton:
					CreatePresenterFrom<RadioButton>(provider, y);
					break;
				case GameObject::Type::PythonBehavior:
					CreatePresenterFrom<PythonBehavior>(provider, y);
					break;
				case GameObject::Type::Behavior:
					SPH_PRESENT_BEHAVIOR(obj);
					break;
				default:
					SPHE_WARN("Details Panel: Unexpected GameObject type");
			}
		}

		wxSize realSize = GetClientSize();
		if (y > realSize.y) {
			SetScrollbars(0, UI::scrollSpeed, realSize.x, y / UI::scrollSpeed);
		} else {
			SetScrollbars(0, UI::scrollSpeed, realSize.x, realSize.y / UI::scrollSpeed);
		}
	}


	void DetailsPanel::ClearPresenters()
	{
		for (Presenter* presenter : presenters) {
			delete presenter;
		}
		presenters.clear();
	}


	void DetailsPanel::FillPresenters(GameObject* curr)
	{
		ClassInfoPtr classInfo = ClassRegistry::GetClassInfo(curr);
		if (classInfo == nullptr) {
			SPHE_ERROR("Class Registry: Unexpected GameObject type {0}", curr->GetType());
			return;
		}

		typeName->SetLabel(classInfo->GetName());
		typeIcon->SetBitmap(ImageManager::GetImage(classInfo->GetIconPath(), ImageManager::PathType::GAMEOBJECTS));
		mainTrafo->SetCurrent(curr);
		headerPresenter->SetCurrent(curr);

		for (Presenter* presenter : presenters) {
			presenter->SetCurrent(curr);
		}
	}


	void DetailsPanel::SetPresentersToDefault()
	{
		typeName->SetLabel("None");
		typeIcon->SetBitmap(wxNullBitmap);
		mainTrafo->SetDefault();
		headerPresenter->SetDefault();
		
		lastType = GameObject::Type::None;
		ClearPresenters();
	}


	void DetailsPanel::RefreshPresenters()
	{
		headerPresenter->Refresh();
		mainTrafo->Refresh();
		for (Presenter* presenter : presenters) {
			presenter->Refresh();
		}
	}


	template<class Class>
	inline void DetailsPanel::CreatePresenterFrom(CommonProvider* provider, uint32& y)
	{
		switch (provider->GetType()) {
			case CommonProvider::Type::BOOL:
				presenters.push_back(new BoolPresenter<Class>(this, (BoolProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::ONEWAYBOOL:
				presenters.push_back(new OneWayBoolPresenter<Class>(this, (OneWayBoolProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::INT:
				presenters.push_back(new IntPresenter<Class>(this, (IntProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::UINT:
				presenters.push_back(new UIntPresenter<Class>(this, (UIntProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::FLOAT:
			case CommonProvider::Type::UFLOAT:
				presenters.push_back(new FloatPresenter<Class>(this, (FloatProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::STRING:
				presenters.push_back(new StringPresenter<Class>(this, (StringProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::POINT:
				presenters.push_back(new PointPresenter<Class>(this, (PointProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::COLOR:
				presenters.push_back(new ColorPresenter<Class>(this, (ColorProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::IMAGE:
				presenters.push_back(new ImagePresenter<Class>(this, (ImageProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::FONT:
				presenters.push_back(new FontPresenter<Class>(this, (FontProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::FONTSTYLE:
				presenters.push_back(new FontStylePresenter<Class>(this, (FontStyleProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::ANIM:
				presenters.push_back(new AnimationPresenter<Class>(this, (AnimationProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::TILESET:
				presenters.push_back(new TileSetPresenter<Class>(this, (TileSetProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::AUDIO:
				presenters.push_back(new AudioPresenter<Class>(this, (AudioProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::STRINGLIST:
				presenters.push_back(new StringListPresenter<Class>(this, (StringListProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::TRAFO:
				presenters.push_back(new TrafoPresenter<Class>(this, (TrafoProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::LAYER:
				presenters.push_back(new LayerPresenter<Class>(this, (LayerProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::BEHAVIOR:
				presenters.push_back(new BehaviorPicker(this, provider->GetName(), behaviorChangedCallback, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::TILEMAP:
				presenters.push_back(new TileMapPresenter(this, provider->GetName(), currDataChangedCallback, y));
				break;
			default:
				SPHE_WARN("Details Panel: Unexpected provider type {0}", provider->GetType());
		}
	}
}