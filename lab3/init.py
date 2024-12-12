import gdb
import re

R = {}

def stop_handler(event):
    if isinstance(event, gdb.StopEvent):
        regs = [
            line for line in 
                gdb.execute('info registers',
                            to_string=True).
                            strip().split('\n')
        ]
        for line in regs:
            parts = line.split()
            key = parts[0]

            if m := re.search(r'(\[.*?\])', line):
                val = m.group(1)
            else:
                val = parts[1]

            if key in R and R[key] != val:
                print(key, R[key], '->', val)
            R[key] = val

gdb.events.stop.connect(stop_handler)

gdb.execute('set confirm off')
gdb.execute('set architecture riscv:rv64')
gdb.execute('set riscv use-compressed-breakpoints yes')

gdb.execute("file kernel/kernel.elf")
gdb.execute("target remote localhost:1234")
# gdb.execute("b strap")
# # gdb.execute("b mtrap")
# gdb.execute("b trap_handler")
# gdb.execute("b syscall")
# gdb.execute("b sys_write")
# gdb.execute("b main")
gdb.execute("b main")
# gdb.execute("display *0x200BFF8")
# gdb.execute("display *0x2004000")
# gdb.execute("display/x $sstatus")
gdb.execute("layout split")
gdb.execute("layout src")
gdb.execute("c")