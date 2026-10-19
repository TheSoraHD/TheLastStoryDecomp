# Reference libraries

Sources for Nintendo/Metrowerks libraries (RVL SDK, NW4R, MSL, Runtime, MetroTRK, NdevExi2A)
copied from [xbret/xenoblade](https://github.com/xbret/xenoblade) (CC0 1.0, see LICENSE.xenoblade).
The Last Story uses a later SDK build (Aug 2010, `0x4302_145`) than Xenoblade (Feb 2009), so some
files differ; only units listed in `config/SLSP01/libs_ref.json` are built.

## Network SDK and DWC sources

- `RVL_SDK/src/revolution/ssl/ssl.c`: from [koopthekoopa/wii-ipl](https://github.com/koopthekoopa/wii-ipl)
  (CC0 1.0), `libs/RevoEX/src/ssl/ssl_api.c` + `ssl_mutex.c`, commit `f4c09f4001c5a0b9af734ad1e3f80a54c5e5723c`.
  `include/revolution/SSL.h` comes from the same repository.
- `RVL_SDK/src/revolution/so/SOBasic.c` and `include/revolution/SO.h`: from
  [doldecomp/mkw](https://github.com/doldecomp/mkw) (CC0 1.0), `lib/rvl/so/soCommon.c` and `so.h`,
  commit `8b9b5ee653da28663e498039e52f5d19f929a011`; `SOInit` was written from the target assembly.
- `dwc/src/base/dwc_error.c`, `dwc_init.c`, `dwc_memfunc.c` and `dwc/include/dwc/`: based on
  doldecomp/mkw (CC0 1.0), `lib/dwc/common/`, same commit, updated to the Aug 2010 DWC build.
- Compilers: the RevoEX libraries carry `0x4199_60831` (= `GC/3.0a5.2`, mwcceppc 4.1 build 60831);
  DWC carries `0x4302_172` (= `Wii/1.3`, 4.3 build 172).
