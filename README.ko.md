# tcxLabel

> [English](README.md) | **한국어**

TrussC의 비트맵 폰트를 대체하는, label로 쉽게 사용할 수 있도록 도와주는 애드온입니다.
TrussC `tc::Font` 위에 얹은 헤더파일로만 구성된 래퍼 클래스로, 사이즈별 폰트 캐싱 / 정렬 / 배경 박스 그리기를 제공합니다.

## Features

- 헤더파일 (`#include <tcxLabel.h>` 만으로 사용)
- 사이즈별 `tc::Font` 자동 로드및 캐싱됩니다.
- 한·중·일(CJK) + Nerd Font 아이콘 + monospace를 한 폰트 (Sarasa Fixed K Nerd Font 번들)로 처리합니다.
- CMake configure 단계에서 폰트 파일을 `bin/data/fonts/` 로 자동 복사합니다.
- 정렬, 배경 박스, 패딩 등 `TextStyle` 기반 스타일링을 지원합니다.

## Bundled Font

[Sarasa Fixed K Nerd Font](https://github.com/jonz94/Sarasa-Gothic-Nerd-Fonts) — Sarasa Gothic의 한국어(K) 고정폭 변형에 Nerd Font 아이콘을 패치한 버전입니다.

Sarasa Gothic은 CJK Unified Ideographs 전체를 포함하므로 **한국어 / 중국어 (간체·번체) / 일본어** 모두 렌더링됩니다.
"K" 변형은 한·중·일이 공유하는 한자(Han Unification)에 대해 **한국식 자형을 우선**으로 사용합니다 — 한국어 위주의 UI라면 그대로 쓰면 되고, 중국어·일본어가 메인이라면 [Sarasa-Gothic-Nerd-Fonts 릴리스](https://github.com/jonz94/Sarasa-Gothic-Nerd-Fonts/releases)에서 J(일본) / SC(간체) / TC(번체) 변형을 받아 [`tcx::label::setFont()`](src/tcxLabel.h)로 교체하면 됩니다.

번들 파일: [data/fonts/sarasa-fixed-k-regular-nerd-font.ttf](data/fonts/sarasa-fixed-k-regular-nerd-font.ttf)

## Usage

```cpp
#include <tcxLabel.h>

void setup() override {
    tcx::label::setFont(TCX_LABEL_FONT_DEFAULT, 16);
}

void draw() override {
    tcx::label::drawText("Hello 한글", 20, 30);

    tcx::label::drawTextWithBackground(
        "status: OK", 20, 60,
        colors::white, colors::darkGreen, /*padding*/ 6.0f);
}
```

스타일 기반:

```cpp
tcx::label::TextStyle s;
s.textColor = colors::white;
s.bgColor   = colors::black;
s.padding   = 8.0f;
s.size      = 24;
s.hAlign    = tc::Center;
tcx::label::drawText("centered", 100, 100, s);
```

## CMake

애드온을 사용하는 프로젝트에서:

```cmake
add_subdirectory(addons/tcxLabel)
target_link_libraries(<your_target> PRIVATE tcxLabel)
```

configure 시점에 `data/fonts/*.ttf` 가 `${CMAKE_SOURCE_DIR}/bin/data/fonts/` 로 복사됩니다.

## License

애드온 코드: **MIT** — [LICENSE](LICENSE)를 읽어주세요..

번들 폰트: **SIL Open Font License 1.1** — see [data/fonts/OFL.txt](data/fonts/OFL.txt), [data/fonts/NOTICE.md](data/fonts/NOTICE.md).

CMake 설정 시 OFL 전문과 NOTICE가 폰트 파일과 함께 `bin/data/fonts/`로 복사되므로, 애드온을 사용하는 프로젝트도 라이선스 의무를 자동으로 충족합니다.
