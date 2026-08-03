#include "ViewportMath.h"

namespace Engine::Core {

ViewportRect ComputeLetterboxedViewport(int framebufferWidth,
                                        int framebufferHeight,
                                        float targetAspectRatio) {
  if (targetAspectRatio <= 0.0f || framebufferWidth <= 0 ||
      framebufferHeight <= 0) {
    return {0, 0, framebufferWidth, framebufferHeight};
  }

  float windowAspect = static_cast<float>(framebufferWidth) /
                       static_cast<float>(framebufferHeight);

  ViewportRect rect;

  if (windowAspect > targetAspectRatio) {
    // Window wider than target — bars on left/right (pillarbox).
    rect.Height = framebufferHeight;
    rect.Width = static_cast<int>(framebufferHeight * targetAspectRatio);
    rect.X = (framebufferWidth - rect.Width) / 2;
    rect.Y = 0;
  } else {
    // Window taller than target — bars on top/bottom (letterbox).
    rect.Width = framebufferWidth;
    rect.Height = static_cast<int>(framebufferWidth / targetAspectRatio);
    rect.X = 0;
    rect.Y = (framebufferHeight - rect.Height) / 2;
  }

  return rect;
}

} // namespace Engine::Core