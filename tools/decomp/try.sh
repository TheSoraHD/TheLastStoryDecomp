#!/bin/bash
# usage: try.sh <file.cpp>  -> compiles standalone with game flags and prints disassembly
# Compilers: $COMPILERS, else build/compilers (PC), else ../compilers, else /home/claude/tls-build/build/compilers
# (cloud, docs/entorno-nube.md). sjiswrap: $SJISWRAP, else build/tools/sjiswrap.exe or the one of tls-build.
f=$(realpath "$1")
cd ${CMP_ROOT:-$(dirname "$0")/../..}
B=/home/claude/tls-build/build
C=${COMPILERS:-}
for d in build/compilers ../compilers $B/compilers; do [ -z "$C" ] && [ -d "$d/Wii/1.3" ] && C=$d; done
S=${SJISWRAP:-}
for s in build/tools/sjiswrap.exe $B/tools/sjiswrap.exe; do [ -z "$S" ] && [ -f "$s" ] && S=$s; done
build/tools/wibo $S $C/Wii/1.3/mwcceppc.exe -nodefaults -proc gekko -align powerpc -enum int -fp hardware -Cpp_exceptions off -O4,p -inline auto -pragma "cats off" -pragma "warn_notinlined off" -maxerrors 1 -nosyspath -fp_contract on -i include -i build/SLSP01/include -DBUILD_VERSION=0 -DVERSION_SLSP01 -DNDEBUG=1 -enc SJIS -RTTI on -str reuse,readonly -sdata 4 -sdata2 4 -func_align 4 -use_lmw_stmw on -i libs/boost/msl_shim -i libs/boost/include -i libs/PowerPC_EABI_Support/include/stl -i libs/PowerPC_EABI_Support/include -ipa file -lang=c++ -c "$f" -o ${TRY_OUT:-/tmp/try_$$.o} 2>&1 | grep -v "^$" | head -20
build/binutils/powerpc-eabi-objdump -dr --no-show-raw-insn ${TRY_OUT:-/tmp/try_$$.o} | grep -v "^$\|file format\|Disassembly"
