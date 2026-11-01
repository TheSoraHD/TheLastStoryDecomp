# vendor.py <ss src rel to ref_ss/src/nw4r, e.g. snd/snd_BasicSound.cpp> : copy a SS-decomp source and the
# headers it needs (not yet vendored) into libs/ss_nw4r, dropping the snd_/ut_/math_ prefix of the nw4r
# headers and rewriting the includes. Existing vendored files are never overwritten.
import sys, os, re, subprocess, shutil
R = os.environ.get('SS_REF', os.environ.get('SS_REF', '/home/claude/ref_ss')); D = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', 'libs', 'ss_nw4r'))
rel = sys.argv[1]
src = R + '/src/nw4r/' + rel
M = R + '/src/PowerPC_EABI_Support/MSL'
inc = ['-i', R + '/include', '-i', M + '/MSL_C++/MSL_Common/Include', '-i', M + '/MSL_C/MSL_Common/Include',
       '-i', M + '/MSL_C/PPC_EABI/Include', '-i', M + '/MSL_C/MSL_Common_Embedded/Math/Include', '-i', R + '/src']
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
if os.path.exists('/tmp/vend.d'): os.remove('/tmp/vend.d')
_r = subprocess.run(['build/tools/wibo', 'build/tools/sjiswrap.exe', 'build/compilers/Wii/1.0/mwcceppc.exe', '-nodefaults', '-nosyspath', '-align', 'powerpc', '-enum', 'int', '-fp', 'hardware', '-Cpp_exceptions', 'off', '-RTTI', 'off',
                '-proc', 'gekko', '-lang=c++', '-enc', 'SJIS', '-MD', '-gccdep'] + inc +
               ['-c', src, '-o', '/tmp/vend.o'], capture_output=True, text=True); print(_r.stdout[-800:]) if not os.path.exists('/tmp/vend.d') else None
deps = open('/tmp/vend.d').read().replace('\\\n', ' ').split()[1:]
pat = re.compile(r'(#\s*include\s*[<"])(nw4r/(?:snd|ut|math)/)((?:snd|ut|math)_)?([^">]+)([">])')
def fix(s):
    s = pat.sub(lambda m: m.group(1) + m.group(2) + m.group(4) + m.group(5), s)
    return s.replace('nw4r/ut/Lock.h', 'nw4r/ut/lock.h')
def dest(p):
    p = p.replace('\\', '/')
    if p.startswith('Z:'): p = p[2:]
    if not p.startswith(R + '/'): return None
    r = p[len(R) + 1:]
    if r.startswith('include/'):
        m = re.match(r'include/nw4r/(snd|ut|math)/(?:snd|ut|math)_(.+)$', r)
        if m: return D + '/include/nw4r/%s/%s' % (m.group(1), m.group(2))
        return D + '/' + r
    if r.startswith('src/PowerPC_EABI_Support/MSL/'): return D + '/msl/' + r[len('src/PowerPC_EABI_Support/MSL/'):]
    if r.startswith('src/nw4r/'): return D + '/src/' + r[len('src/nw4r/'):]
    return None
n = 0
for p in deps + [src]:
    p = p.replace('\\', '/')
    if p.startswith('Z:'): p = p[2:]
    d = dest(p)
    if d is None: print('skip', p); continue
    # lock.h case
    if d.endswith('/ut/Lock.h'): d = d[:-6] + 'lock.h'
    if os.path.exists(d): continue
    os.makedirs(os.path.dirname(d), exist_ok=True)
    s = open(p, encoding='latin-1').read()
    open(d, 'w', encoding='latin-1').write(fix(s)); n += 1
    print('new', d[len(D) + 1:])
print(n, 'files')
