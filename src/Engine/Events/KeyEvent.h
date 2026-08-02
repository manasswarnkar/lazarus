#pragma once

#include "Event.h"
#include "Input/KeyCodes.h"
#include <sstream>

namespace Engine::Events {

class KeyEvent : public Event {
public:
  Input::Key GetKeyCode() const { return m_KeyCode; }
  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
  explicit KeyEvent(Input::Key keycode) : m_KeyCode(keycode) {}
  Input::Key m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(Input::Key keycode, bool isRepeat)
      : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

  bool IsRepeat() const { return m_IsRepeat; }

  std::string ToString() const override {
    std::ostringstream ss;
    ss << "KeyPressedEvent: " << static_cast<int>(m_KeyCode)
       << " (repeat=" << m_IsRepeat << ")";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

private:
  bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent {
public:
  explicit KeyReleasedEvent(Input::Key keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::ostringstream ss;
    ss << "KeyReleasedEvent: " << static_cast<int>(m_KeyCode);
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

} // namespace Engine::Events