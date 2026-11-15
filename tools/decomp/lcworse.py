# lcworse.py <lc now> <rep now> <ref dir>: units worse than the reference in labelcheck (missing count) or rep100 (count)
import sys,collections
def lc(f):
    d={}
    for l in open(f):
        p=l.split()
        if len(p)>=4 and p[2]=='missing': d[p[0]]=int(p[3])
    return d
def rep(f):
    d=collections.Counter()
    for l in open(f):
        p=l.split()
        if len(p)>=3 and p[0].startswith('REP') is False and '/' in p[0]: d[p[0]]+=1
    return d
a,b,r=sys.argv[1:4]
L,Lr=lc(a),lc(r+'/labelcheck.txt'); R,Rr=rep(b),rep(r+'/rep100.txt')
w=sorted(set(u for u in L if L[u]>Lr.get(u,0))|set(u for u in R if R[u]>Rr.get(u,0)))
for u in w: print(u, L.get(u,0), Lr.get(u,0), R.get(u,0), Rr.get(u,0))
