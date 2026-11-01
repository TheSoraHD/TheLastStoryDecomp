#!/usr/bin/env python3
# tomethod.py FN file...: turns extern "C" calls FN(obj, args) into (obj)->FN(args) and drops the
# extern "C" declarations (rename FN in symbols.txt to the method name afterwards)
# tomethod.py FN file...: replace extern "C" calls fn(obj, args) by (obj)->fn(args) and drop the extern decl
import re,sys
fn=sys.argv[1]
def split_args(s):
    out=[];depth=0;cur=''
    for ch in s:
        if ch in '([{': depth+=1
        elif ch in ')]}': depth-=1
        if ch==',' and depth==0: out.append(cur);cur=''
        else: cur+=ch
    out.append(cur); return out
for path in sys.argv[2:]:
    s=open(path).read(); o=s
    # drop declarations (possibly multi-line)
    s=re.sub(r'extern "C" [^;{]*?\b'+fn+r'\([^;]*?\);[^\n]*\n','',s)
    res='';i=0
    pat=re.compile(r'(?<![\w>.:])'+fn+r'\(')
    while True:
        m=pat.search(s,i)
        if not m: res+=s[i:];break
        j=m.end();depth=1;k=j
        while depth:
            if s[k]=='(':depth+=1
            elif s[k]==')':depth-=1
            k+=1
        args=split_args(s[j:k-1])
        obj=args[0].strip(); rest=','.join(args[1:]).strip()
        if re.fullmatch(r'&(\w+(\[[^\]]*\])?|\w+->\w+)',obj): pre=obj[1:]+'.'
        elif re.fullmatch(r'[\w.]+(->\w+)*(\(\))?',obj): pre=obj+'->'
        else: pre='('+obj+')->'
        res+=s[i:m.start()]+pre+fn+'('+rest+')'
        i=k
    if res!=o: open(path,'w').write(res); print('changed',path)
