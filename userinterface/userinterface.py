# Andrew Lee
# 3 May 2021
# userinterface.py
"""
This python module interfaces with the C++ Chessbot Engine to play chess.
Or at least, it will eventually...
"""
import chessbot
import tkinter as tk
import tkinter.messagebox
from time import sleep
import sys

sizeOfBoard = 720
symbolSize = (sizeOfBoard / 8)
symbolThickness = 50
piecesAsUnicode = {0: "", 1: "\u2659", 2: "\u2658", 3:"\u2657", 5: "\u2656", 8: "\u2655", 9: "\u2654",
                   -1: "\u265F", -2: "\u265E", -3: "\u265D", -5: "\u265C", -8: "\u265B", -9: "\u265A"}
whiteToMove = 0x01

class Chess:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Chessbot (Alpha)")
        self.canvas = tk.Canvas(self.window, width=sizeOfBoard, height=sizeOfBoard)
        self.canvas.pack()
        self.window.bind("<Button-1>", self.click)
        self.window.protocol("WM_DELETE_WINDOW", self.onClose)
        self.board = chessbot.getCurrentBoard()
        self.gameCtrlFlags = chessbot.getGameCtrlFlags()
        self.pieceHandles = []
        self.initializeBoard()
        self.window.update()
        
        self.resetBoard = False
        self.whiteWins = False
        self.blackWins = False
        self.AIisWhite = True
        self.AIisBlack = True
        self.windowClosed = False

    def mainloop(self):
        while not self.windowClosed:
            if self.gameCtrlFlags & whiteToMove:
                if self.AIisWhite:
                    chessbot.makeAutomaticMove(4)
                    self.board = chessbot.getCurrentBoard()
                    self.gameCtrlFlags = chessbot.getGameCtrlFlags()
                else:
                    pass # process player move here...
            else:
                if self.AIisBlack:
                    chessbot.makeAutomaticMove(4)
                    self.board = chessbot.getCurrentBoard()
                    self.gameCtrlFlags = chessbot.getGameCtrlFlags()
                else:
                    pass
            
            self.drawBoard()
            self.window.update_idletasks()
            self.window.update()
            sleep(0.25)
    
    def initializeBoard(self):
        color = "white"
        for y in range(8):
            for x in range(8):
                x1 = x*sizeOfBoard/8
                y1 = y*sizeOfBoard/8
                x2 = x1+sizeOfBoard/8
                y2 = y1+sizeOfBoard/8
                self.canvas.create_rectangle((x1, y1, x2, y2), fill=color)
                if color == "white":
                    color = "blue"
                else:
                    color = "white"
            if color == "white":
                color = "blue"
            else:
                color = "white"

        for row in range(8):
            for col in range(8):
                c = getSpace(row, col, self.board)
                self.pieceHandles.append(self.canvas.create_text(col*sizeOfBoard/8 + sizeOfBoard/16, (7-row)*sizeOfBoard/8 + sizeOfBoard/16, font="* 60", text=piecesAsUnicode[c]))

    def drawBoard(self):
        for p in self.pieceHandles:
            self.canvas.delete(p)
        self.pieceHandles = []

        for row in range(8):
            for col in range(8):
                c = getSpace(row, col, self.board)
                self.pieceHandles.append(self.canvas.create_text(col*sizeOfBoard/8 + sizeOfBoard/16, (7-row)*sizeOfBoard/8 + sizeOfBoard/16, font="* 60", text=piecesAsUnicode[c]))
                
    def click(self, event):
        gridPosition = [event.x, event.y]

    def onClose(self):
        self.window.destroy()
        self.windowClosed = True
    
    def convertGridToLogicalPosition(self, gridPosition):
        pass


def getSpace(row: int, col: int, boardarr: list)->int:
    if( row >= 0 and row < 8 and col >= 0 and col < 8 ):
        return boardarr[ col + 8 * row ]
    else:
        return -128

def printBoardToTerminal(board: list):
    piecesAsUnicode = {1: "\u2659", 2: "\u2658", 3:"\u2657", 5: "\u2656", 8: "\u2655", 9: "\u2654",
                       -1: "\u265F", -2: "\u265E", -3: "\u265D", -5: "\u265C", -8: "\u265B", -9: "\u265B",
                       0: "  "}
    boardAsStr = "";
    for i in range(7, -1, -1):
        for j in range(8):
            c = getSpace(i,j, board)
            print(f"{c:3}", end="")
        print()

def printBoardToWindow(board: list, window: tk.Tk):
    bts_frame = tk.Frame()

def main():
    game = Chess()
    game.mainloop()
   
if __name__ == "__main__":
    main()



"""
    board = chessbot.getCurrentBoard()
    gamestate = chessbot.getGameCtrlFlags()
    for i in range(10):
        chessbot.makeAutomaticMove(4)
        board = chessbot.getCurrentBoard()
        flags = chessbot.getGameCtrlFlags();
        printBoardToTerminal(board)
        if not flags & 0x01:
            print(f"White moved {i}")
        else:
            print(f"Black moved {i}")
"""