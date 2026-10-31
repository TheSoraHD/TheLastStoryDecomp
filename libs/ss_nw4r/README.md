# libs/ss_nw4r

NW4R sound sources and headers taken from the Skyward Sword decompilation
(`zeldaret/ss`, commit 96f2dd5, CC0, see `LICENSE`). That version of NW4R
(2010) is the one linked in the game area of The Last Story
(`nw4r::snd::SoundArchivePlayer` at 0x8059E7EC).

Only the include closure of `src/snd/snd_SoundArchivePlayer.cpp` is copied:
`include/` (nw4r, rvl, common.h, decomp.h) and the MSL headers in `msl/`.

Changes for The Last Story:
- Header files renamed without the `snd_`/`ut_`/`math_` prefix (`LinkList.h`,
  `SoundInstanceManager.h`, ...): the assert strings of the game use those
  `__FILE__` names.
- `ut/LinkList.h`: asserts of the 2010 template (`operator->` line 403,
  `GetFront` 497, `GetNodeFromPointer` 563, `GetPointerFromNode` 573).
- `snd/SoundArchivePlayer.h`: `detail_ConvertLabelStringToSoundId` assert at
  line 201.

The unit is compiled with the game flags plus `-DNW4R_ASSERT` (see
`config/SLSP01/libs_ref.json`).
