from ctypes import *
import ctypes
import json
import tkinter as tk
from core import game

dll = WinDLL(".\mcts.dll")

print(dll.mcts(s)) 