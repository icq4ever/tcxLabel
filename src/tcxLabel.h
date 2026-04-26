#pragma once

// =============================================================================
// tcxLabel - Label / text rendering on top of TrussC's Font class.
// =============================================================================
//
// Bundled font: Sarasa Fixed K Nerd Font (Korean + Nerd icons + monospace).
// Copied to <project>/bin/data/fonts/ at CMake configure time.
//
// Quick start:
//   #include <tcxLabel.h>
//
//   void setup() override {
//       tcx::label::setFont(TCX_LABEL_FONT_DEFAULT, 16);
//   }
//
//   void draw() override {
//       tcx::label::drawText("Hello \xed\x95\x9c\xea\xb8\x80", 20, 30);
//
//       tcx::label::drawTextWithBackground("status: OK", 20, 60,
//           colors::white, colors::darkGreen, /*padding*/ 6.0f);
//   }
// =============================================================================

#include <TrussC.h>
#include <map>
#include <set>
#include <string>

namespace tcx {
namespace label {

// -----------------------------------------------------------------------------
// Bundled font path (resolved through getDataPath -> bin/data/fonts/...)
// -----------------------------------------------------------------------------
#define TCX_LABEL_FONT_SARASA_FIXED_K "fonts/sarasa-fixed-k-regular-nerd-font.ttf"
#define TCX_LABEL_FONT_DEFAULT        TCX_LABEL_FONT_SARASA_FIXED_K

// -----------------------------------------------------------------------------
// Style for drawTextWithStyle
// -----------------------------------------------------------------------------
struct TextStyle {
    tc::Color    textColor   = tc::Color(1.0f, 1.0f, 1.0f, 1.0f);
    tc::Color    bgColor     = tc::Color(0.0f, 0.0f, 0.0f, 0.0f);  // a=0 → no bg
    float        padding     = 6.0f;
    int          size        = 0;          // 0 = use default size
    tc::Direction hAlign     = tc::Left;
    tc::Direction vAlign     = tc::Top;
};

namespace detail {

// Per-size Font cache (Font itself is cheap to copy once cached internally)
inline std::map<int, tc::Font>& fontCache() {
    static std::map<int, tc::Font> cache;
    return cache;
}

// Sizes we already tried and failed — avoid retrying every frame (would spam logs).
inline std::set<int>& failedSizes() {
    static std::set<int> failed;
    return failed;
}

inline std::string& currentFontPath() {
    static std::string path = TCX_LABEL_FONT_DEFAULT;
    return path;
}

inline int& currentSize() {
    static int size = 16;
    return size;
}

// Get (or lazily load) the Font for the requested size.
inline tc::Font* getFont(int size) {
    if (size <= 0) size = currentSize();

    auto& cache = fontCache();
    auto it = cache.find(size);
    if (it != cache.end()) {
        return &it->second;
    }

    auto& failed = failedSizes();
    if (failed.count(size)) {
        return nullptr;  // Already known to fail — don't spam logs.
    }

    // Resolve relative paths via TrussC's data path convention.
    // Absolute paths (e.g. /usr/share/fonts/...) pass through unchanged.
    const std::string& path = currentFontPath();
    std::string resolved =
        (!path.empty() && path[0] == '/') ? path : tc::getDataPath(path);

    tc::Font f;
    if (!f.load(resolved, size)) {
        tc::logError() << "[tcxLabel] Failed to load font: "
                       << resolved << " @ " << size << "px";
        failed.insert(size);
        return nullptr;
    }
    auto [inserted, _] = cache.emplace(size, std::move(f));
    return &inserted->second;
}

} // namespace detail

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

// Set the active font. Clears the size cache so subsequent draws use the new file.
inline void setFont(const std::string& fontPath, int defaultSize = 16) {
    detail::currentFontPath() = fontPath;
    detail::currentSize()     = defaultSize;
    detail::fontCache().clear();
    detail::failedSizes().clear();
}

inline void setFontSize(int size) {
    detail::currentSize() = size;
}

inline int getFontSize() {
    return detail::currentSize();
}

// -----------------------------------------------------------------------------
// Drawing
// -----------------------------------------------------------------------------

// Plain text at (x, y), default size, top-left aligned.
inline void drawText(const std::string& text, float x, float y) {
    if (auto* f = detail::getFont(0)) {
        f->drawString(text, x, y);
    }
}

// With explicit size.
inline void drawText(const std::string& text, float x, float y, int size) {
    if (auto* f = detail::getFont(size)) {
        f->drawString(text, x, y);
    }
}

// With alignment.
inline void drawText(const std::string& text, float x, float y,
                     tc::Direction hAlign, tc::Direction vAlign) {
    if (auto* f = detail::getFont(0)) {
        f->drawString(text, x, y, hAlign, vAlign);
    }
}

// Full style.
inline void drawText(const std::string& text, float x, float y, const TextStyle& style) {
    auto* f = detail::getFont(style.size);
    if (!f) return;

    // Background first if visible.
    if (style.bgColor.a > 0.0f) {
        tc::Rect bb = f->getBBox(text);
        // Adjust for alignment so the box wraps the actually-drawn glyphs.
        float bx = x + bb.x - style.padding;
        float by = y + bb.y - style.padding;
        float bw = bb.width  + style.padding * 2;
        float bh = bb.height + style.padding * 2;

        tc::pushStyle();
        tc::setColor(style.bgColor);
        tc::drawRect(bx, by, bw, bh);
        tc::popStyle();
    }

    tc::pushStyle();
    tc::setColor(style.textColor);
    f->drawString(text, x, y, style.hAlign, style.vAlign);
    tc::popStyle();
}

// Convenience: text on a colored rectangle.
inline void drawTextWithBackground(const std::string& text, float x, float y,
                                    tc::Color textColor, tc::Color bgColor,
                                    float padding = 6.0f) {
    TextStyle s;
    s.textColor = textColor;
    s.bgColor   = bgColor;
    s.padding   = padding;
    drawText(text, x, y, s);
}

// -----------------------------------------------------------------------------
// Measurement
// -----------------------------------------------------------------------------

// Bounding box of text at the default size.
inline tc::Rect getTextBBox(const std::string& text) {
    if (auto* f = detail::getFont(0)) {
        return f->getBBox(text);
    }
    return tc::Rect{0, 0, 0, 0};
}

inline tc::Rect getTextBBox(const std::string& text, int size) {
    if (auto* f = detail::getFont(size)) {
        return f->getBBox(text);
    }
    return tc::Rect{0, 0, 0, 0};
}

} // namespace label
} // namespace tcx
