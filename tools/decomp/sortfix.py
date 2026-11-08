#!/usr/bin/env python3
# sortfix.py file.cpp : applies the MSL __sort register fixes (s12h-20261108) to a Loader-style Sort_unk
import sys
p=sys.argv[1]; s=open(p).read()
R=[("""    T* result = first;
    if (first != last) {
        for (T* it = first + 1; it != last; it++) {""","""    T* it;
    T* result = first;
    if (first != last) {
        for (it = first + 1; it != last; it++) {"""),
("""        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        T* i = first;
        j = pivot;""","""        T* i;
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        i = first;
        j = pivot;"""),
("for (T* end = last - 1; first != end; first++) {","for (; first != last - 1; first++) {"),
]
for o,n in R:
    c=s.count(o); print(c, o.split('\n')[0][:50]); s=s.replace(o,n)
import re
s2,k=re.subn(r"T\* b = first \+ \(n \* 3 / 4 \+ (\w+) % 5\);", r"int n3 = n * 3 / 4;\n        T* b = first + (n3 + \1 % 5);", s)
print(k,'n3')
open(p,'w').write(s2)
