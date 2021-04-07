#include "UIButtonMenu.h"

USING_NS_CC;

UIButtonMenu::UIButtonMenu(std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>>& buttons_callbacks, 
	Node* parent, EventDispatcher* dispatcher, std::function<Action*()> focused_button_action_callback) :
	m_buttonsCallbacks(buttons_callbacks),
	m_parent(parent),
	m_dispatcher(dispatcher),
	m_focusedButtonActionCallback(focused_button_action_callback)
{
	if (m_buttonsCallbacks.empty())
		throw std::runtime_error("Empty buttons menu");

	if (!m_parent)
		throw std::runtime_error("Parent node must be set for buttons menu");

	if (!m_dispatcher)
		throw std::runtime_error("Event dispatcher must be set by parent scene for buttons menu");
}

bool UIButtonMenu::init()
{
	if (!Node::init())
	{
		return false;
	}

	int i = 0;
	for (const auto& item : m_buttonsCallbacks)
	{
		ui::Button* button = item.first;
		ui::Widget::ccWidgetClickCallback callback = item.second;
		if (button->getParent() != m_parent)
		{
			button->setParent(m_parent);
		}
		button->addClickEventListener(callback);
		auto button_listener = EventListenerCustom::create("button_pressed_" + std::to_string(i),
			[=](EventCustom* event) { callback(nullptr); });
		m_dispatcher->addEventListenerWithSceneGraphPriority(button_listener, m_parent);
		button->setScale(0.8);
		i++;
	}

	m_buttonsCallbacks.front().first->runAction(m_focusedButtonActionCallback());

	auto keyboard_listener = EventListenerKeyboard::create();
	keyboard_listener->onKeyPressed = CC_CALLBACK_2(UIButtonMenu::onKeyPressed, this);
	keyboard_listener->onKeyReleased = CC_CALLBACK_2(UIButtonMenu::onKeyReleased, this);

	m_dispatcher->addEventListenerWithSceneGraphPriority(keyboard_listener, m_parent);

	m_parent->schedule([this](float dt) { this->update(); }, 0.1f, "button_menu_update_callback");

	return true;
}

UIButtonMenu* UIButtonMenu::create(std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>>& buttons_callbacks,
	Node* parent, EventDispatcher* dispatcher, std::function<Action* ()> focused_button_action_callback)
{
	UIButtonMenu* pRet = new(std::nothrow) UIButtonMenu(buttons_callbacks, parent, dispatcher, focused_button_action_callback);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}

}


void UIButtonMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        m_isUpPressed = true;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
        m_isDownPressed = true;
    }
	else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		EventCustom event("button_pressed_" + std::to_string(m_currButtonId));
		m_dispatcher->dispatchEvent(&event);
	}

}

void UIButtonMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
		m_isUpPressed = false;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
    {
		m_isDownPressed = false;
    }
}

void UIButtonMenu::update()
{
	m_moveUp = int(m_isUpPressed) - int(m_isDownPressed);
	if (!m_moveUp)
		return;
	int old_id = m_currButtonId;
	m_currButtonId -= m_moveUp;
	if (m_currButtonId < 0)
	{
		m_currButtonId = 0;
	}
	if (m_currButtonId > m_buttonsCallbacks.size() - 1)
	{
		m_currButtonId = m_buttonsCallbacks.size() - 1;
	}

	if (old_id != m_currButtonId)
	{
		m_buttonsCallbacks[old_id].first->stopAllActions();
		m_buttonsCallbacks[old_id].first->setScale(0.8);
		m_buttonsCallbacks[m_currButtonId].first->runAction(m_focusedButtonActionCallback());
	}

}
