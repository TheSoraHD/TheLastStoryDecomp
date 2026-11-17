# fixlbl.py: re-label by address the PacketPack objects that pd.sh leaves unpaired (VO strings, tables, bss, jumptables)
import subprocess,re
o='build/SLSP01/src/atn/PacketPack.o'
rows=[l.split() for l in subprocess.run(['nm','-S','-n',o],capture_output=True,text=True).stdout.splitlines()]
rows=[r for r in rows if len(r)==4]
rod=[r for r in rows if r[2]=='r']; bss=[r for r in rows if r[2]=='b']; dat=[r for r in rows if r[2]=='d']
args=[]
t28=[r for r in rod if r[1]=='00000028']
args.append('0x807596F0='+t28[0][3])
vo=[r for r in rod if r[1]=='0000000d' and r[3].startswith('@')]
# the 35 VO strings are the consecutive run of 35 13-byte strings
nums=[int(r[3][1:]) for r in vo]
start=None
for i in range(len(nums)-44):
    if nums[i+44]-nums[i]==44: start=i;break
if start is not None:
    # the 10 VO_ARCOM strings of lbl_80790FF0 come first (escribir-20261116 qG3)
    for k in range(10): args.append('0x%08X=%s'%(0x80759810+16*k,vo[start+k][3]))
    start+=10
else:
    for i in range(len(nums)-34):
        if nums[i+34]-nums[i]==34: start=i;break
for k in range(35): args.append('0x%08X=%s'%(0x807598B0+16*k,vo[start+k][3]))
b=[r for r in bss if r[1]=='0000000c'][:3]
for a,r in zip([0x807C8AE8,0x807C8AF4,0x807C8B00],b): args.append('0x%08X=%s'%(a,r[3]))
for size,addr in (('00000058',0x807911B4),('000000a4',0x8079120C),('00000104',0x807912DC)):
    j=[r for r in dat if r[1]==size and r[3].startswith('@')]
    if len(j)==1: args.append('0x%08X=%s'%(addr,j[0][3]))
# jump table of fn_804EB934 (0x2C bytes): the one right before the table of fn_804EE040 (escribir-20261116 qG3)
dd=sorted(dat,key=lambda r:int(r[0],16))
for k in range(1,len(dd)):
    if dd[k][1]=='00000104' and dd[k-1][1]=='0000002c' and dd[k-1][3].startswith('@'):
        args.append('0x807912B0='+dd[k-1][3])
subprocess.run(['python3','/home/claude/agentes/bin/pdfix.py']+args,stdout=subprocess.DEVNULL)
print(len(args),'relabelled')
