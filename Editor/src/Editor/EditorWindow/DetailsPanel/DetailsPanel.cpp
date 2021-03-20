#include "editor_pch.h"
#include "DetailsPanel.hpp"
#include "Editor/Registry/ClassRegistry.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "BehaviorCreator.hpp"
#include "BehaviorPicker.hpp"


namespace Sharpheus {

	DetailsPanel::DetailsPanel(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxPanel(parent, wxID_ANY, pos, size), freeY(60)
	{
		title = new wxStaticText(this, wxID_ANY, "Details", wxPoint(10, 10));
		title->SetFont(wxFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

		typeName = new wxStaticText(this, wxID_ANY, "", wxPoint(30, 35));
		typeIcon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(10, 35), wxSize(16, 16));
		namePresenter = new NamePresenter(this, "Name", std::move(currDataChangedCallback), std::move(currNameChangedCallback), freeY);
		mainTrafo = new MainTrafoPresenter(this, "Transform", std::move(currDataChangedCallback), freeY);
	}


	DetailsPanel::~DetailsPanel()
	{
		ClearPresenters();
		delete mainTrafo;
		delete namePresenter;
	}


	void DetailsPanel::BindCallbacks(std::function<void(const std::string&, const std::string&)>&& currNameChangedCallback,
		std::function<void()>&& currDataChangedCallback, std::function<void(uint32_t)>&& behaviorChangedCallback)
	{
		this->currNameChangedCallback = std::move(currNameChangedCallback);
		this->currDataChangedCallback = std::move(currDataChangedCallback);
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

	void DetailsPanel::CurrentNameChanged(const std::string& oldName, const std::string& newName)
	{
		namePresenter->Refresh();
	}

	void DetailsPanel::CurrentDataChanged(GameObject* curr)
	{
		RefreshPresenters();
	}

	void DetailsPanel::CreatePresenters(GameObject* obj)
	{
		ClearPresenters();

		ClassInfo* classInfo = ClassRegistry::GetClassInfo(obj);
		GameObject::Type type = obj->GetType();
		if (classInfo == nullptr) {
			SPHE_ERROR("Class Registry: Unexpected GameObject type {0}", type);
			return;
		}

		uint32_t y = freeY;
		for (CommonProvider* provider : classInfo->GetProviders()) {
			switch (type) {
				case GameObject::Type::Collection:
					CreatePresenterFrom<Collection>(provider, y);
					break;
				case GameObject::Type::Camera:
					CreatePresenterFrom<Camera>(provider, y);
					break;
				case GameObject::Type::Sprite:
					CreatePresenterFrom<Collection>(provider, y);
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
				case GameObject::Type::Behavior:
					SPH_PRESENT_BEHAVIOR(obj);
					break;
				default:
					SPHE_WARN("Details Panel: Unexpected GameObject type {0}", type);
			}
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
		ClassInfo* classInfo = ClassRegistry::GetClassInfo(curr);
		if (classInfo == nullptr) {
			SPHE_ERROR("Class Registry: Unexpected GameObject type {0}", curr->GetType());
			return;
		}

		typeName->SetLabel(classInfo->GetName());
		typeIcon->SetBitmap(ImageManager::GetImage(classInfo->GetIconPath(), ImageManager::PathType::GAMEOBJECTS));
		mainTrafo->SetCurrent(curr);
		namePresenter->SetCurrent(curr);

		for (Presenter* presenter : presenters) {
			presenter->SetCurrent(curr);
		}
	}


	void DetailsPanel::SetPresentersToDefault()
	{
		typeName->SetLabel("None");
		typeIcon->SetBitmap(wxNullBitmap);
		mainTrafo->SetDefault();
		namePresenter->SetDefault();
		for (Presenter* presenter : presenters) {
			presenter->SetDefault();
		}
	}


	void DetailsPanel::RefreshPresenters()
	{
		namePresenter->Refresh();
		mainTrafo->Refresh();
		for (Presenter* presenter : presenters) {
			presenter->Refresh();
		}
	}


	template<class Class>
	inline void DetailsPanel::CreatePresenterFrom(CommonProvider* provider, uint32_t& y)
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
			case CommonProvider::Type::TRAFO:
				presenters.push_back(new TrafoPresenter<Class>(this, (TrafoProvider<Class>*)provider, currDataChangedCallback, y));
				break;
			case CommonProvider::Type::BEHAVIOR:
				presenters.push_back(new BehaviorPicker(this, provider->GetName(), behaviorChangedCallback, currDataChangedCallback, y));
				break;
			default:
				SPHE_WARN("Details Panel: Unexpected provider type {0}", provider->GetType());
		}
	}
}