#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include <gtest/gtest.h>

using namespace Engine::Events;

TEST(EventTest, WindowResizeEventStoresDimensions) {
  WindowResizeEvent event(1920, 1080);

  EXPECT_EQ(event.GetWidth(), 1920u);
  EXPECT_EQ(event.GetHeight(), 1080u);
  EXPECT_EQ(event.GetEventType(), EventType::WindowResize);
}

TEST(EventTest, WindowCloseEventHasCorrectType) {
  WindowCloseEvent event;
  EXPECT_EQ(event.GetEventType(), EventType::WindowClose);
}

TEST(EventTest, EventsAreNotHandledByDefault) {
  WindowCloseEvent event;
  EXPECT_FALSE(event.Handled);
}

TEST(EventTest, CategoryFlagsMatchApplicationCategory) {
  WindowResizeEvent event(800, 600);
  EXPECT_TRUE(event.IsInCategory(EventCategoryApplication));
  EXPECT_FALSE(event.IsInCategory(EventCategoryInput));
}

TEST(EventTest, ToStringIncludesDimensions) {
  WindowResizeEvent event(640, 480);
  std::string str = event.ToString();

  EXPECT_NE(str.find("640"), std::string::npos);
  EXPECT_NE(str.find("480"), std::string::npos);
}

// ---------- EventDispatcher ----------

TEST(EventDispatcherTest, DispatchesToMatchingHandler) {
  WindowResizeEvent event(1280, 720);
  EventDispatcher dispatcher(event);

  bool handlerCalled = false;
  bool result =
      dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent &e) {
        handlerCalled = true;
        EXPECT_EQ(e.GetWidth(), 1280u);
        return true;
      });

  EXPECT_TRUE(result);
  EXPECT_TRUE(handlerCalled);
}

TEST(EventDispatcherTest, DoesNotDispatchToMismatchedType) {
  WindowResizeEvent event(1280, 720);
  EventDispatcher dispatcher(event);

  bool handlerCalled = false;
  bool result = dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent &) {
    handlerCalled = true;
    return true;
  });

  EXPECT_FALSE(result);
  EXPECT_FALSE(handlerCalled);
}

TEST(EventDispatcherTest, HandlerReturnValueSetsHandledFlag) {
  WindowResizeEvent event(100, 100);
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowResizeEvent>(
      [](WindowResizeEvent &) { return true; });

  EXPECT_TRUE(event.Handled);
}

TEST(EventDispatcherTest, HandlerReturningFalseDoesNotSetHandled) {
  WindowResizeEvent event(100, 100);
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowResizeEvent>(
      [](WindowResizeEvent &) { return false; });

  EXPECT_FALSE(event.Handled);
}

TEST(EventDispatcherTest, MultipleDispatchCallsOnlyMatchingOneFires) {
  WindowCloseEvent event;
  EventDispatcher dispatcher(event);

  bool resizeCalled = false;
  bool closeCalled = false;

  dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent &) {
    resizeCalled = true;
    return true;
  });

  dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent &) {
    closeCalled = true;
    return true;
  });

  EXPECT_FALSE(resizeCalled);
  EXPECT_TRUE(closeCalled);
}