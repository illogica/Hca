var
  i = open("list.txt")
  o = open("list.cpp", fmWrite)

for line in i.lines:
  o.writeln("emojyList.append(\"qrc:/emojione/", line, "\");")

i.close()
o.close()
