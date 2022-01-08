import numpy as np
# import torch
import json
import time
import ctypes

delta = [ [ 1,1 ],[ 0,1 ],[ -1,1 ],[ -1,0 ],
[ -1,-1 ],[ 0,-1 ],[ 1,-1 ],[ 1,0 ],
[ 2,0 ],[ 2,1 ],[ 2,2 ],[ 1,2 ],
[ 0,2 ],[ -1,2 ],[ -2,2 ],[ -2,1 ],
[ -2,0 ],[ -2,-1 ],[ -2,-2 ],[ -1,-2 ],
[ 0,-2 ],[ 1,-2 ],[ 2,-2 ],[ 2,-1 ] ]

def step_conv(stp:int):
	x0 = stp&0x7
	y0 = (stp&0x38)>>3
	x1 = (stp&0x1c0)>>6
	y1 = (stp&0xe00)>>9
	return x0,y0,x1,y1

class game:
	bot_color = 0	# black 1, white -1, black first !
	riv_color = 0
	js = {}
	terminated = False

	def __init__(self) -> None:
		self.grid = np.zeros((7,7))
		self.grid[0][0] = self.grid[6][6] = 1
		self.grid[6][0] = self.grid[0][6] = -1
		self.js={'requests':[],'responses':[]}


	def wait_for_human(self):
		pass
		
	def init_game_man(self, human):
		self.riv_color = human
		self.bot_color = -human
		if human == -1:
			self.js['requests'].append({"x0": -1, "x1": -1, "y0": -1, "y1": -1})
			self.bot_decision()
		else: self.wait_for_human()

	def bot_decision(self):
		dll = ctypes.WinDLL('mcts.dll')
		g0, g1 = self.throw_to_c()
		g0 = ctypes.c_ulonglong(g0)
		g1 = ctypes.c_ulonglong(g1)
		st = dll.mcts(g0,g1,self.bot_color)
		x0,y0,x1,y1 = step_conv(st)
		self.proceed(self.bot_color, x0,y0,x1,y1)
		self.js['responses'].append({'x0':x0, 'y0':y0, 'x1':x1, 'y1':y1})
	

	# scores: return black score, white score
	def scores(self) :
		a = self.grid.sum()
		b = np.abs(self.grid).sum()
		return (a+b)//2,(b-a)//2

	def ingrid(self,x,y) -> bool:
		return x in range(0,7) and y in range(0,7)

	def dead(self, color) -> int:
		for i in range(7):
			for j in range(7):
				if self.grid[i][j] == color:
					for dx,dy in delta:
						if self.ingrid(i+dx,j+dy) and self.grid[i+dx][j+dy] == 0 :
							return False
		self.terminated = True
		return True

	def proceed_check(self, color, origx, origy, newx, newy) -> bool:
		if not self.ingrid(newx, newy) or not self.ingrid(origx, origy) : return False
		if self.grid[origx][origy] != color: return False
		if origx == newx and origy == newy: return False
		if abs(origx - newx) > 2 or abs(origy - newy) > 2 : return False		
		return True

	def proceed(self, color, origx, origy, newx, newy):
		if self.proceed_check(color, origx, origy, newx, newy):
			self.grid[newx][newy] = color
			if abs(origx - newx) > 1 or abs(origy - newy) > 1:
				self.grid[origx][origy] = 0
			for de in delta[:8]:
				if self.ingrid (newx + de[0], newy + de[1]) and self.grid[newx + de[0]][newy + de[1]] != 0 :
					self.grid[newx + de[0]][newy + de[1]] = color
			return True
		else : 
			print('pcheck err')
			return False

	def replay(self, history) -> None:
		if self.bot_color != 0:
			print ("init error.")
			return

		self.js = history
		if history["requests"][0]["x0"] < 0:
			self.bot_color = 1
			self.riv_color = -1
		else :
			self.bot_color = -1
			self.riv_color = 1
		grid = np.zeros((7,7))
		grid[0][0] = grid[6][6] = 1
		grid[6][0] = grid[0][6] = -1
		turns = len(history["responses"])
		for i in range(turns):
			reql = history["requests"][i]
			if reql["x0"]>=0:
				self.proceed(self.riv_color, reql["x0"], reql["y0"], reql["x1"], reql["y1"])
			resl = history["responses"][i]
			if resl["x0"]>=0:
				self.proceed(self.bot_color, resl["x0"], resl["y0"], resl["x1"], resl["y1"])
		if len(history["requests"]) > turns:
			reql = history["requests"][turns]
			if reql["x0"]>=0:
				self.proceed(self.riv_color, reql["x0"], reql["y0"], reql["x1"], reql["y1"])

	
	def throw_to_c(self) :
		white = 0
		black = 0
		exp = 1
		for i in range(7):
			for j in range(7):
				if self.grid[i][j] == -1: white += exp
				elif self.grid[i][j] == 1: black += exp
				exp <<= 1
		return white, black

	def save(self):
		localtime = time.asctime( time.localtime(time.time()) )
		dir = localtime.replace(':','-') + '.json'
		nf = open('save\\' + dir, mode = 'x')
		json.dump(self.js, nf)
		nf.close()	
		return dir

	def load(self, dir:str) :
		nf = open(dir, mode = 'r')
		g = game()
		g.replay(json.load(nf))
		return g

	def withdraw(self) :
		humansz = len(self.js['requests'])
		botsz = len(self.js['responses'])
		if humansz > 0 and self.js["requests"][humansz - 1]['x0'] >= 0:
			self.js['requests'].pop()
		else : return False

		if len(self.js['requests']) == 0:
			g = game()
			g.init_game_man(1)
			return g

		if botsz > 0 and self.js['responses'][botsz - 1]['x0'] >= 0:
			self.js['responses'].pop()
		g = game()
		g.replay(self.js)
		return g

	def dbg_print(self):
		for i in range(7):
			for j in range(7):
				if self.grid[i][j]==1: print('X', end='')
				elif self.grid[i][j]==-1: print('O', end='')
				else: print(' ',end='')
			print('')

