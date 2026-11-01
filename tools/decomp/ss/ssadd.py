# ssadd.py <file.cpp in libs/ss_nw4r/src/snd> "<sec> <start> <end>" ... : addsplit.py + libs_ref entry with
# the lib-zone NW4R flags (Wii/1.0, as snd_AxManager) and the libs/ss_nw4r include tree.
import sys, json, subprocess, os
os.chdir(os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..')))
f = sys.argv[1]; name = 'ss_nw4r/src/' + (f if '/' in f else 'snd/' + f)
r = subprocess.run(['python3', 'tools/decomp/addsplit.py', '--like', 'snd/snd_AxManager.cpp', name] + sys.argv[2:],
                   capture_output=True, text=True, env=dict(os.environ, MW='Wii/1.0'))
print(r.stdout, r.stderr[-500:])
D = 'libs/ss_nw4r'
cf = ['-nodefaults', '-proc gekko', '-align powerpc', '-enum int', '-fp hardware', '-Cpp_exceptions off', '-O4,p',
      '-inline auto', '-pragma "cats off"', '-pragma "warn_notinlined off"', '-maxerrors 1', '-nosyspath', '-RTTI off',
      '-str reuse', '-enc SJIS', '-DNDEBUG=1', '-ipa file', '-fp_contract off', '-func_align 16',
      '-i ' + D + '/include', '-i ' + D + '/msl/MSL_C++/MSL_Common/Include', '-i ' + D + '/msl/MSL_C/MSL_Common/Include',
      '-i ' + D + '/msl/MSL_C/PPC_EABI/Include', '-i ' + D + '/msl/MSL_C/MSL_Common_Embedded/Math/Include', '-lang=c++']
p = 'config/SLSP01/libs_ref.json'; d = json.load(open(p))
for u in d:
    if u['name'] == name:
        u['cflags'] = cf; u['mw_version'] = 'Wii/1.0'; u['category'] = 'nw4r'
        u['matching'] = os.environ.get('MATCH', '1') == '1'
open(p, 'w').write(json.dumps(d, indent=1, ensure_ascii=False))
