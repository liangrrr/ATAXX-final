import os
import tkinter as tk
from tkinter import font
import tkinter.messagebox as msg
from tkinter.constants import BOTH, DISABLED, LEFT, NORMAL, RIGHT, TOP
from PIL import Image, ImageTk
from numpy import polyadd

from core import game	

class gui_gaming:
	playing : game
	drawn = False
	def __init__(self):
		self.playing = game()
		self.wind = tk.Tk()
		# self.wind.title = '同化棋'
		self.wind.geometry('350x425')
		self.header = tk.Frame(self.wind)
		self.board = tk.Frame(self.wind)
		self.bb = tk.Canvas(self.wind)
		# self.bb.
		self.b_start = tk.Button(self.header, text="新游戏", command=self.newgame)
		self.b_load = tk.Button(self.header, text="读取", command=self.g_load)
		self.b_save = tk.Button(self.header, text="保存", command=self.g_save, state=DISABLED)
		self.b_regret = tk.Button(self.header, text="悔棋", command=self.g_withdraw, state=DISABLED)
		self.b_quit = tk.Button(self.header, text="退出", command=self.quit)
		backgg = Image.open('pics\\board.png')
		self.backg = ImageTk.PhotoImage(image=backgg)
		# backg = tk.PhotoImage('pics\\board.png')
		self.blackch =  ImageTk.PhotoImage(image=Image.open('pics\\black.png'))
		self.whitech =  ImageTk.PhotoImage(image=Image.open('pics\\white.png'))
		self.blackcic =  ImageTk.PhotoImage(image=Image.open('pics\\blackpotential.png'))
		self.whitecic =  ImageTk.PhotoImage(image=Image.open('pics\\whitepotential.png'))
		self.header.pack(side=TOP)
		self.b_start.pack(side = LEFT, padx=10)
		self.b_load.pack(side=LEFT, padx=10)
		self.b_save.pack(side=LEFT, padx = 10)
		self.b_regret.pack(side=LEFT, padx = 10)
		self.b_quit.pack(side=LEFT, padx=10)
		self.wind.protocol("WM_DELETE_WINDOW",self.quit)
		self.wind.mainloop()
		


	def drawings(self):
		if self.drawn:
			self.scoreboard.destroy()
			self.board.destroy()
		
		sc = self.playing.scores()

		blackcap = '● '+str(int(sc[0]))
		if self.playing.bot_color == 1 : blackcap += ' AI'
		else :blackcap += ' 你'

		if self.playing.bot_color == -1 : whitecap = 'AI '
		else :whitecap = '你 '
		whitecap += str(int(sc[1])) + ' ○'


		self.scoreboard = tk.Frame(self.wind)
		self.blb = tk.Label(self.scoreboard, text=blackcap, font=("黑体",12))
		self.wlb = tk.Label(self.scoreboard, text=whitecap, font=("黑体",12))

		self.blb.pack(side=LEFT,padx = 30)
		self.wlb.pack(side=RIGHT,padx = 30)
		self.scoreboard.pack(pady=5)

		self.board = tk.Canvas(self.wind)
		self.board.create_image(175,175,image = self.backg)
		for i in range(7):
			for j in range(7):
				if self.playing.grid[i][j] == 1:
					self.board.create_image(i*50+25, j*50+25, image=self.blackch)
				elif self.playing.grid[i][j] == -1:
					self.board.create_image(i*50+25, j*50+25, image=self.whitech)
		self.board.pack(expand=True, fill=BOTH)
		self.drawn = True

	def newgame(self):
		self.playing = game()
		self.b_save.config(state=DISABLED)
		self.b_regret.config(state=DISABLED)
		ans = msg.askyesno("新游戏", '选择你的角色', detail=['是：黑方 先手','否：白方 后手'])
		if ans: color = 1
		else : color = -1
		self.playing.init_game_man(color)
		self.b_save.config(state=NORMAL)
		self.b_regret.config(state=NORMAL)
		self.drawings()
		self.board.bind("<Button-1>", self.handler)
		# self.header.destroy()


	def restart(self):
		self.board.destroy()
		self.newgame()

	def handler(self,e):
		x0 = int(e.x//50)
		y0 = int(e.y//50)
		if not self.playing.ingrid(x0,y0):
			return
		if self.playing.grid[x0][y0] != -self.playing.bot_color:
			return
		self.selected=x0,y0
		if self.playing.bot_color == 1:
			self.cic = self.board.create_image(x0*50+25,y0*50+25,image=self.blackcic)
		else :
			self.cic = self.board.create_image(x0*50+25,y0*50+25,image=self.whitecic)
		self.board.bind("<Button-1>", self.handler2)


	def handler2(self,e):
		self.board.unbind("<Button-1>")
		x1 = int(e.x//50)
		y1 = int(e.y//50)
		if not self.playing.ingrid(x1,y1):
			self.board.delete(self.cic)
			self.board.bind("<Button-1>", self.handler)
			return
		x0,y0 = self.selected
		if self.playing.grid[x1][y1] != 0:
			self.board.delete(self.cic)
			self.board.bind("<Button-1>", self.handler)
			return
		if abs(x1-x0)>2 or abs(y1-y0)>2:
			self.board.delete(self.cic)
			self.board.bind("<Button-1>", self.handler)
			return
		self.playing.proceed(-self.playing.bot_color, x0,y0,x1,y1)
		self.playing.js['requests'].append({'x0':x0, 'y0':y0, 'x1':x1, 'y1':y1})
		self.drawings()

		if self.playing.dead(self.playing.bot_color):
			sc = self.playing.scores()
			if (sc[0]-sc[1]) * self.playing.bot_color < 0:
				self.board.create_text(175,175,text='你赢了',font=('黑体',24),fill='red')
			else :
				self.board.create_text(175,175,text='你输了',font=('黑体',24),fill='red')
			return

		self.b_regret.config(state=DISABLED)
		self.b_save.config(state=DISABLED)
		self.playing.bot_decision()
		self.b_regret.config(state=NORMAL)
		self.b_save.config(state=NORMAL)
		self.drawings()
		
		if self.playing.dead(-self.playing.bot_color):
			sc = self.playing.scores()
			if (sc[0]-sc[1]) * self.playing.bot_color < 0:
				self.board.create_text(175,175,text='你赢了',font=('黑体',24),fill='red')
			else :
				self.board.create_text(175,175,text='你输了',font=('黑体',24),fill='red')
			return

		self.board.bind("<Button-1>", self.handler)


	def g_load(self):
		self.wind.withdraw()
		self.nw = tk.Toplevel(self.wind)
		self.lb = tk.Listbox(self.nw)
		for file in os.listdir('save'):
			self.lb.insert("end",file)
		bs = tk.Frame(self.nw)
		b_ld = tk.Button(bs, text="读取", command=self.g_load_load)
		b_del = tk.Button(bs, text="删除", command=self.g_load_del)
		self.lb.pack(side=TOP)
		bs.pack(side=TOP)
		b_ld.pack(side=LEFT)
		b_del.pack(side=LEFT)
		self.nw.protocol("WM_DELETE_WINDOW",self.wrapper)
		self.nw.mainloop()
		self.wind.deiconify()

	def wrapper(self):
		self.wind.deiconify()
		self.nw.destroy()
	
	def g_load_del(self):
		if self.lb.curselection():
			os.remove('save\\'+self.lb.get(self.lb.curselection()))
			self.lb.delete(self.lb.curselection())
			# self.nw.destroy()

	def g_load_load(self):
		if self.lb.curselection():
			self.playing = self.playing.load('save\\'+self.lb.get(self.lb.curselection()))
			self.nw.destroy()
			self.wind.deiconify()
			self.drawings()
		if self.playing.dead(self.playing.bot_color) or self.playing.dead(-self.playing.bot_color):
			sc = self.playing.scores()
			if (sc[0]-sc[1]) * self.playing.bot_color < 0:
				self.board.create_text(175,175,text='你赢了',font=('黑体',24),fill='red')
			else :
				self.board.create_text(175,175,text='你输了',font=('黑体',24),fill='red')
			self.board.unbind("<Button-1>")
		self.board.bind("<Button-1>", self.handler)
		
		
	
	def g_save(self):
		dir = self.playing.save()
		msg.showinfo("保存成功","存档于"+dir)
	
	def g_withdraw(self):
		print(self.playing.js)
		self.playing = self.playing.withdraw()
		self.drawings()
		self.board.bind("<Button-1>", self.handler)
	
	def quit(self):
		ans = msg.askokcancel("退出","确定要退出吗？所有未保存的进度将丢失。")
		if ans : exit()

gui_gaming()