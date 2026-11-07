#!/bin/bash
# fixpd.sh: undoes the labels pd.sh/pdbatch.sh rename wrongly (lbl_8087D6E8/EC and lbl_80775AA4 become
# @STRING@ names and the link fails with "multiply-defined"), then reconfigures.
cd "$(dirname "$0")/../.."
B=${B:-/home/claude/tls-build}; T=${T:-/home/claude/tools}
sed -i 's/^@STRING@Resize_unk__Q23atn14SimpleArray<i>Fii@0 = .sdata:0x8087D6E8;/lbl_8087D6E8 = .sdata:0x8087D6E8;/; s/^@STRING@Resize_unk__Q23atn14SimpleArray<i>Fii = .sdata:0x8087D6EC;/lbl_8087D6EC = .sdata:0x8087D6EC;/; s/^@STRING@__allocate__Q23std20vector<10ColorF_unk>FUi = .data:0x80775AA4;/lbl_80775AA4 = .data:0x80775AA4;/' config/SLSP01/symbols.txt
python3 configure.py --compilers $B/build/compilers --dtk $T/dtk --objdiff $T/objdiff-cli --sjiswrap $B/build/tools/sjiswrap.exe --wrapper $T/wibo >/dev/null 2>&1
