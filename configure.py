#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "SLSP01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-2"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.3"
config.objdiff_tag = "v3.6.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

# Game flags: SJIS encoding for the game code (-enc is not supported by GC/1.x compilers)
# RTTI is on: game vtables start with a typeinfo pointer (e.g. vtable at 0x80775A20)
cflags_game = [
    *(f for f in cflags_base if f not in ("-multibyte", "-RTTI off", "-str reuse")),
    "-enc SJIS",
    "-RTTI on",
    "-str reuse,readonly",  # RTTI names and literals live in .rodata
    "-sdata 4",             # 8-byte RTTI objects and doubles are outside the small-data sections
    "-sdata2 4",
    "-func_align 4",
    "-use_lmw_stmw on",     # game code uses stmw/lmw (seen at 0x80496430); _savegpr still appears per function
    # Boost subset (boost::function/bind, see libs/boost/README.md) and the MSL headers it needs
    "-i libs/boost/msl_shim",
    "-i libs/boost/include",
    "-i libs/PowerPC_EABI_Support/include/stl",
    "-i libs/PowerPC_EABI_Support/include",
]
# The units at 0x8068B60C-0x8069512C were matched with these flags before -inline deferred
cflags_game_nodefer = list(cflags_game)
# Game code is compiled with file-level IPA: functions are emitted in source order, can inline
# functions defined later in the file (MapPartsColliTree at 0x80496420) and non-inline function
# templates such as boost::bind (0x801B2A80) and the boost::function setter at 0x8049103C, which
# -inline deferred never inlines.
cflags_game = cflags_game + ["-ipa file"]

config.linker_version = "GC/1.3.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "progress_category": "game",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Runtime.PPCEABI.H",
        # mwcc 4.1/4.2 (GC/3.0a*): GC/1.x-2.x emit cmplwi instead of cmpwi in __destroy_global_chain
        "mw_version": "GC/3.0a5.2",
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
#            Object(NonMatching, "__init_cpp_exceptions.cpp"),
        ],
    },
]

config.libs.append({
    "lib": "game",
    # Wii/1.3 with -func_align 4: GC/3.0a* schedules some code differently (e.g.
    # Event::PostSequence::~PostSequence), and Wii/1.0-1.1 do not reload a member pointer after an
    # inline-asm store through it (chr::ControlSitting at 0x801C9B24); 1.5/1.7 give the same code as 1.3.
    "mw_version": "Wii/1.3",
    "cflags": cflags_game,
    "progress_category": "game",
    "objects": [
        Object(NonMatching, "Arms.cpp"),
        Object(NonMatching, "auto_03_803B27AC_text.cpp"),
        Object(NonMatching, "auto_fn_801F1EA0_text.cpp"),
        Object(NonMatching, "auto_fn_8030DA1C_text.cpp"),
    ],
})


config.libs.append({
    "lib": "game_nodefer",
    "mw_version": "Wii/1.3",
    "cflags": cflags_game_nodefer,
    "progress_category": "game",
    "objects": [
        Object(Matching, "auto_03_8068B60C_text.cpp"),
        Object(Matching, "auto_03_8068D7AC_text.cpp"),
        Object(Matching, "auto_03_8068DCF0_text.cpp"),
        Object(Matching, "auto_03_80690D60_text.cpp"),
        Object(Matching, "auto_03_80691B44_text.cpp"),
        Object(Matching, "auto_03_80691B48_text.cpp"),
        Object(Matching, "auto_03_80691C90_text.cpp"),
        Object(Matching, "auto_03_80691CFC_text.cpp"),
        Object(Matching, "auto_03_80692BFC_text.cpp"),
        Object(Matching, "auto_03_806929FC_text.cpp"),
        Object(Matching, "auto_03_8069512C_text.cpp"),
    ],
})


# Reference library units (SDK, NW4R, MSL, Runtime, MetroTRK, NdevExi2A).
# Sources live in libs/ and come from xbret/xenoblade (CC0).
# Per-object compiler and flags are listed in config/<version>/libs_ref.json.
def load_ref_libs() -> List[Dict[str, Any]]:
    import json

    path = Path("config") / config.version / "libs_ref.json"
    if not path.exists():
        return []
    libs: Dict[Any, Dict[str, Any]] = {}
    for unit in json.loads(path.read_text()):
        key = (unit["category"], unit["mw_version"], tuple(unit["cflags"]))
        if key not in libs:
            libs[key] = {
                "lib": f"ref_{unit['category']}_{len(libs)}",
                "mw_version": unit["mw_version"],
                "src_dir": "libs",
                "cflags": unit["cflags"],
                "progress_category": "sdk" if unit["category"] != "nw4r" else "nw4r",
                "objects": [],
            }
        # GameSpy units (gamespy/<dir>/<file>.c) live in libs/gamespy/src/<dir>/<file>.c
        options = {}
        if unit["name"].startswith("gamespy/"):
            options["source"] = "gamespy/src/" + unit["name"][len("gamespy/"):]
        libs[key]["objects"].append(
            Object(Matching if unit.get("matching") else NonMatching, unit["name"], **options)
        )
    return list(libs.values())


config.libs.extend(load_ref_libs())


# Game translation units inferred from RTTI/vtable layout and data references
# (see docs/game_units.md). File names are inferred from the main class of each unit.
def load_game_units() -> List[Any]:
    import json

    path = Path("config") / config.version / "game_units.json"
    if not path.exists():
        return []
    return [
        Object(Matching if u.get("matching") else NonMatching, u["name"])
        for u in json.loads(path.read_text())
    ]


for lib in config.libs:
    if lib["lib"] == "game":
        lib["objects"].extend(load_game_units())


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects


# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
    ProgressCategory("nw4r", "NW4R Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
