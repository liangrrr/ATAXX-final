from ctypes import *
import json
dll = WinDLL(".\mcts.dll")
s = input()
print(dll.mcts(s)) 