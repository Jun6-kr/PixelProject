# PixelProject Development Log

PixelProject를 진행하며 수행한 작업과 문제 해결 과정을 날짜별로 기록합니다.

## Structure

```text
docs/devlog/
├─ README.md
├─ TEMPLATE.md
└─ daily/
   └─ YYYY-MM-DD.md
```

## Categories

- **Work**: 수행하거나 변경한 작업
- **Solved**: 해결한 문제와 핵심 해결 방법
- **Decision**: 결정한 내용과 이유
- **Issue**: 남아 있는 문제, 위험 요소 또는 확인할 사항
- **Next**: 다음에 수행할 구체적인 행동

## Update workflow

1. `TEMPLATE.md`를 복사해 `daily/YYYY-MM-DD.md` 형식으로 저장합니다.
2. 같은 날짜에는 새 파일을 만들지 않고 기존 파일에 항목을 추가합니다.
3. 기존 기록은 보존하고 같은 의미의 항목은 합칩니다.
4. 코드 변경과 관련된 기록에는 가능하면 브랜치, 커밋 또는 관련 파일을 함께 적습니다.
5. 인증 정보, 개인정보, 공개하면 안 되는 내부 정보는 기록하지 않습니다.

## Evidence and review markers

- 일반 항목: 커밋 메시지 또는 diff에서 직접 확인된 사실
- **`[추론]`**: 코드 구조를 바탕으로 해석했지만 당시 의도는 확인되지 않은 내용
- **`Review Checklist`**: 작성자가 나중에 확인하거나 수정해야 하는 항목
- 체크가 끝난 항목은 `[ ]`를 `[x]`로 바꾸고 필요한 설명을 본문에 반영합니다.

커밋 기반 로그는 변경 이력을 요약한 문서이며 전체 소스 코드 감사 결과가 아닙니다. 특히 대형 커밋에서는 GitHub가 일부 diff를 기본적으로 숨길 수 있으므로, 세부 구현은 관련 파일을 추가로 확인해야 합니다.

## ChatGPT update request

대화가 끝난 뒤 다음과 같이 요청하면 기록 형식을 유지하기 쉽습니다.

> 오늘 대화에서 PixelProject 관련 Work, Solved, Decision, Issue, Next를 추출해 `docs/devlog/daily/YYYY-MM-DD.md`에 중복 없이 추가해 줘. 기존 내용은 보존하고 같은 의미의 항목은 합쳐 줘. 추측한 내용은 기록하지 말고, 확인된 사실만 반영해 줘.

## Daily logs

- [2026-09-03](daily/2026-09-03.md) — 초기 프로젝트 구성, 디버그 모드, 충돌 및 레벨 흐름
- [2026-09-04](daily/2026-09-04.md) — 카메라/충돌 디버그 수정과 디버그 명령 구조
- [2026-09-07](daily/2026-09-07.md) — 캐릭터 리소스 개편, A* 적·발사체 및 디버그 시각화
- [2026-09-08 ~ 2026-09-09](daily/2026-09-09.md) — Airman 보스 패턴, AirShooter, 문과 충돌 구조
- [2026-09-10](daily/2026-09-10.md)
- [2026-09-17](daily/2026-09-17.md) — 록맨 모작 전환, QuadTree 충돌, HUD와 Horn 기반 작업
- [2026-09-24 ~ 2026-09-25](daily/2026-09-25.md) — QuadTree 재구성, Horn/HornPlatform 및 플레이어 피격 효과
- [2026-10-04](daily/2026-10-04.md) — Airman Phase 공격 흐름, AirShooter 배치 콜백 및 반복 발사 구조
- [2026-11-18](daily/2026-11-18.md) — Airman 반복 공격·점프, 체력 HUD 연동, 충돌 판정 통합 및 8방향 A*
