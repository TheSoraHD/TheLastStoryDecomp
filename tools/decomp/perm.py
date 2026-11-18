#!/usr/bin/env python3
# perm.py <unit> <symbol> <start> <end> [--apply]: order of local declarations (lines start..end of src/<unit>.cpp,
# all "<type> <name> = <expr>;"); wrapper of tools/decomp/declperm.py of your copy (run from its root).
import sys,subprocess
sys.exit(subprocess.call(['python3','tools/decomp/declperm.py']+sys.argv[1:]))
