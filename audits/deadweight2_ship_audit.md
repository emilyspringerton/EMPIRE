# DEADWEIGHT_2 ship audit (kanban #580, EMPIRE NORTHSTAR open Q2)

**Question:** what is DEADWEIGHT_2 missing to ship CRASH_OVERRIDE v0.3.0?

**Answer:** nothing. v0.3.0 has already shipped, and the project is on v0.6.0. The premise in the card and in
NORTHSTAR's old "built, not shipped" note is out of date.

## Evidence (checked 2026-10-08 against `DEADWEIGHT_2` git and GitHub)

| Tag | Released (GitHub) | Assets |
|---|---|---|
| v0.1.0 | 2026-09-25 13:57 | release |
| v0.2.0 | 2026-09-25 14:15 | release |
| v0.3.0 | 2026-09-25 20:56 | `dw2_client_linux_x86_64`, `dw2_client_windows_x86_64.exe`, `SDL2.dll`, `dw2_local_linux_x86_64`, `dw2_server_linux_x86_64`, `D2_CONSTRUCT.txt` |
| v0.4.0 | 2026-09-25 20:58 | release (changelog note on the live-verified v0.3.0 release) |
| v0.5.0 | 2026-09-25 22:19 | release (Windows client bundled as one zip, matching DEADWEIGHT) |
| v0.6.0 | 2026-10-01 02:17 | **Latest.** Same Linux binaries, `dw2_client_windows.zip`, D2_CONSTRUCT |

- CI (`.github/workflows/ci.yml`) builds and releases on tag. The v0.3.0 CHANGELOG entry (Apple #20874) records a
  green end-to-end CI run that produced the real GitHub Release.
- Commits since v0.3.0 are fixes and docs, not missing features.

## What this changes

- **Card #580's premise is stale.** There is no ship blocker for v0.3.0 to find.
- **NORTHSTAR Q2 and the §1 DEADWEIGHT row** said "built, not shipped." That is wrong and has been corrected.
- **Real open items, not ship blockers:** the NORTHSTAR's own V0 gaps (guest identity in IDUNA, per the earlier
  note) are about the *product* beyond the shipped release. They were not re-audited here.

## Recommendation

Close #580 as "already shipped" with this audit as the reference. Do not spend build time on a ship blocker that
does not exist.
