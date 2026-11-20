# Week 12 프로젝트 분석

분석 대상은 배포 폴더의 문서·설정·소스·헤더 18개다. `.vs/`는 IDE 데이터이며 `out/`은 빌드 산출물과 SFML·FreeType 의존성을 담는다. 기존 CMake 캐시는 현재 프로젝트 경로, Ninja, Visual Studio Community MSVC x64를 가리킨다. 확인 시 캐시 폴더 바로 아래에 게임 실행 파일이 없었다.

| 파일 | 역할 |
| --- | --- |
| `README.md` | Elice 코드 복사, 컴파일러별 빌드 절차, PA별 실행 결과, 조작법 및 문제 해결 |
| `CMakeLists.txt` | CMake 3.28 이상·C++17. FetchContent로 SFML 3.0.2를 가져와 `SFML::Graphics`에 연결. 오디오·네트워크 꺼짐 |
| `CMakeSettings.json` | Visual Studio의 `x64-Debug` 설정. MSVC x64 환경과 Ninja, `out/build/x64-Debug` 사용 |
| `PA1.cpp`, `headers/PA1.hpp` | 플레이어, 투사체, 발사 간격과 제공된 Weapon. 헤더에 상수와 API 정의 |
| `PA2.cpp`, `headers/PA2.hpp` | 상속과 가상 함수로 적 종류 구현, 편대 배치와 이동. 헤더에 기본 진형과 좌표·점수 상수 정의 |
| `headers/common.hpp` | 800×600과 `Rect{x,y,w,h}`. 오른쪽·아래쪽이 양수 |
| `visual/main.cpp` | 이벤트 처리, 프레임 시간 측정(최대 0.05초), 게임 업데이트, 렌더링 |
| `visual/Action.hpp`, `Input.hpp`, `Input.cpp` | 키 입력을 이동 방향과 발사 여부로 변환. 포커스를 잃으면 눌린 키 상태 초기화 |
| `visual/Game.hpp`, `Game.cpp` | SFML에 독립적인 통합 계층. 플레이어 이동·발사 → 편대 이동·적 발사 → 투사체 이동·정리 → 하단 도달 시 편대 재생성 |
| `visual/Draw.hpp`, `Draw.cpp` | bounds와 적 타입을 읽어 배경·개체·HUD 렌더링. 첫 피격 후 장갑 적을 더 어둡게 표시 |
| `visual/PixelFont.hpp`, `PixelFont.cpp` | 외부 폰트 없이 5×7 픽셀 글꼴과 사각형 렌더링 |

## PA1 및 PA2 검증 항목

현재 두 파일 모두 스타터다. PA1이 완료되면 플레이어가 움직이고 발사한다. PA2가 완료되면 적 편대가 나타나고 이동한다. 두 과제를 완료하면 적도 약 1초마다 무작위로 발사한다. 충돌과 점수는 PA3, 승패는 PA4 범위다.

| 그룹 | PA1 | PA2 |
| --- | --- | --- |
| 1 | 생성 시 시작 위치와 목숨. 방향 제한, 음수 dt 처리, 양쪽 이동 경계 | 적 초기 상태, 좌표 이동, bounds, 일반 적의 점수·타입·피격 |
| 2 | 피해를 반복해도 목숨이 0에서 멈추는지, 생존 판정과 사각형 | 빨간 적의 상속 및 오버라이드, 장갑 적이 두 번째 피격에서만 파괴되는지 |
| 3 | 아군은 위로, 적군은 아래로 이동. 죽은 탄 및 음수 dt 무시. 경계값 자체는 생존 | 36기 기본 편대, 행별 길이와 빈 칸, 행·열 좌표, 재생성 시 방향 초기화 |
| 4 | 발사 성공, 대기 중 거절, 양수 dt만 차감, 0에 도달하면 재발사 | 제공된 벽 처리 뒤 수평 이동, 사용자 설정 속도 적용, 빈 편대 및 최대 하단 좌표 |

`Fleet::build`는 진형을 행 우선으로 순회하고 `R`, `A`, `N`만 생성한다. `unique_ptr<Enemy>` 소유권을 유지한다. 벽 판정은 프레임 시작 위치 기준이며 방향 반전과 하강이 그 프레임의 수평 이동에 앞선다. `lowestY()`는 적의 `y + ENEMY_H` 중 최댓값을 사용하고 빈 편대에서 0이다.

## 실행 확인

Visual Studio에서 폴더를 열고 `x64-Debug`의 `space_invaders`를 빌드한다. 기존 Ninja 구성이 유효하고 MSVC 개발 환경이 로드된 터미널에서는 `cmake --build out/build/x64-Debug --parallel 4`를 사용한다. README의 별도 터미널 절차는 VS 2022 생성기와 `build-msvc` 경로를 사용한다. 첫 구성은 의존성을 내려받는다. README는 공백과 한글이 없는 짧은 경로를 권장하므로 실제 구성 오류가 나면 빌드 로그부터 확인한다.

왼쪽·오른쪽 또는 A·D로 이동, Space로 발사, H로 목숨 차감, R로 재시작, Esc로 종료. PA1 HUD는 새 플레이어의 목숨과 시작 x만, PA2 HUD는 기본 편대의 개수만 검사한다. HUD 상태는 Game 생성 시 계산한다. 프레임 코드가 점수와 최고 점수에 0을 넘기므로 Week 12에서는 두 숫자가 0으로 표시된다.
