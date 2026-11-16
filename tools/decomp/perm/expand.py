import sys
t=open(sys.argv[1]).read();o=[];i=0
while True:
    import re
    m=re.compile(r'\bPERM_(IGNORE|PRETEND)\(').search(t,i)
    if not m: o.append(t[i:]);break
    a=m.start()
    o.append(t[i:a]); k=t.index('(',a); name=t[a:k]; d=0;j=k
    while True:
        if t[j]=='(':d+=1
        elif t[j]==')':
            d-=1
            if d==0:break
        j+=1
    if name=='PERM_IGNORE': o.append(t[k+1:j])
    i=j+1
open(sys.argv[2],'w').write(''.join(o))
