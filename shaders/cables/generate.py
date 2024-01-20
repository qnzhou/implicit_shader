#!/usr/bin/env python

with open("cable_0.wgsl", 'r') as fin:
    code = fin.read()

for i in range(1, 50):
    with open(f"cable_{i}.wgsl", 'w') as fout:
        fout.write(code.replace("let i = 0", f"let i = {i}"))
