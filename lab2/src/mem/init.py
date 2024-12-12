import gdb

gdb.execute("set pagination off")
gdb.execute("break main")
gdb.execute("layout src")
gdb.execute("run")