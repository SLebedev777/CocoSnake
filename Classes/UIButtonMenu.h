#pragma once
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;

class UIButtonMenu: public Node
{
public:
	UIButtonMenu(std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>>& buttons_callbacks, 
		Node* parent, EventDispatcher* dispatcher, std::function<Action* ()> focused_button_action_callback=nullptr);
	virtual bool init();
	static UIButtonMenu* create(std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>>& buttons_callbacks,
		Node* parent, EventDispatcher* dispatcher, std::function<Action* ()> focused_button_action_callback=nullptr);
	void update();

private:
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	Action* defaultFocusedButtonActionCallback();
private:
	std::vector<std::pair<ui::Button*, ui::Widget::ccWidgetClickCallback>> m_buttonsCallbacks;
	Node* m_parent = nullptr;
	EventDispatcher* m_dispatcher = nullptr;
	std::function<Action* ()> m_focusedButtonActionCallback;
	int m_currButtonId = 0;
	int m_moveUp = 0;
	bool m_isUpPressed = false;
	bool m_isDownPressed = false;
	Sprite* m_pointer = nullptr;
};