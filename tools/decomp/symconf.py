# symconf.py: with symbols.txt in conflict, counts the conflict blocks and prints the lines that differ in a non-@N name
import re,sys
s=open('config/SLSP01/symbols.txt').read()
blocks=re.findall(r'<<<<<<< [^\n]*\n(.*?)=======\n(.*?)>>>>>>> [^\n]*\n',s,re.S)
non=0
for a,b in blocks:
  if len(a.splitlines())!=len(b.splitlines()): print('LEN',len(a.splitlines()),len(b.splitlines()))
  for x,y in zip(a.splitlines(),b.splitlines()):
    nx=x.split(' = ')[0]; ny=y.split(' = ')[0]
    if x!=y and not (re.match(r'^@\d+$',nx) and re.match(r'^@\d+$',ny)): non+=1; print('NON@',x[:90],'|',y[:60])
print(len(blocks),'blocks; non-@',non)
