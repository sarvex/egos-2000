import re
import os

files = os.listdir("build/debug")

instructions = set()
for file in files:
    print("processing {}".format(file))
    f = open("build/debug/{}".format(file), "r")
    for line in f:
        l = re.split('\t|\n', line)
        if len(l) > 2 and len(l[2]) > 0:
    	    instructions.add(l[2])


print("\033[1;32m[INFO] In total, there are {} instructions.\033[0m".format(len(instructions)))
print(sorted(list(instructions)))

exist = {"csrr", "csrw", 
         "add", "sub", "mul", "and", "or", "xor", "slt", "sltu", "sra", "srl", "sll",
         "addi", "ori", "andi", "xori", "slti", "sltiu", "srai", "srli", "slli", "lui", "auipc",
         "lw", "sw",
         "jal", "jalr",
         "bne", "beq", "blt", "bltu", "bge", "bgeu"}

print("\033[1;32m[INFO] There exist {} instructions.\033[0m".format(len(exist)))
print(sorted(list(exist)))

rest = instructions - exist
print("\033[1;32m[INFO] There are {} more instructions to be implemented.\033[0m".format(len(rest)))
print(sorted(list(rest)))

f.close()