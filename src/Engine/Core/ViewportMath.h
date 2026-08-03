#pragma once

namespace Engine::Core {

struct ViewportRect {
  int X = 0, Y = 0;
  int Width = 0, Height = 0;
};

// Computes a letterboxed/pillarboxed viewport rectangle for the given
// framebuffer size and a target aspect ratio (width / height).
//
// - If targetAspect <= 0, returns a viewport filling the entire
//   framebuffer (aspect-ratio locking disabled).
// - If the framebuffer is wider than the target aspect, bars are placed
//   on the left/right (pillarbox).
// - If the framebuffer is taller than the target aspect, bars are placed
//   on the top/bottom (letterbox).
ViewportRect ComputeLetterboxedViewport(int framebufferWidth,
                                        int framebufferHeight,
                                        float targetAspectRatio);

} // namespace Engine::Core