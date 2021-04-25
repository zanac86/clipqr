import os, sys, glob

s1='<RCC>\n<qresource prefix="/">\n'
s2='    <file>images/%s</file>'
s3='\n</qresource>\n</RCC>\n'

files=glob.glob("*.png")

f=open("clipqr.qrc", "wt")
f.write(s1)
f.write("\n".join([s2 % fn for fn in files]))
f.write(s3)
f.close()
